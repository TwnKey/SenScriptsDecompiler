#include "headers/Builder.h"
#include "headers/functions.h"
#include "headers/global_vars.h"
#include <stack>
#include <string>

#include <QTextCodec>

Builder::Builder() = default;

void Builder::read_functions_xlsx(QXlsx::Document& xls_content) {
    scene_name = xls_content.read(2, 1).toString().toStdString();
    int first_row = 4;
    int last_row = xls_content.dimension().lastRow();
    int fun_id = 0;

    std::string content_first_cell = xls_content.read(first_row, 1).toString().toStdString();
    if (content_first_cell == "FUNCTION") {

        auto fun_name = xls_content.read(first_row, 2).toString().toStdString();
        Function current_fun(fun_id, fun_name, 0, 0, 0);
        current_fun.xlsx_row_index = first_row;
        int addr_instr = 0;

        for (int idx_row = first_row + 1; idx_row < last_row; idx_row++) {
            content_first_cell = xls_content.read(idx_row, 1).toString().toStdString();
            if (content_first_cell == "FUNCTION") { // We start a new function
                addr_instr = 0;
                functions_parsed.push_back(current_fun);

                std::string next_fun_name = xls_content.read(idx_row, 2).toString().toStdString();
                current_fun = Function(fun_id, next_fun_name, 0, 0, 0);
                current_fun.xlsx_row_index = idx_row;
                fun_id++;
            } else {
                int opcode = xls_content.read(idx_row + 1, 2).toInt();
                std::shared_ptr<Instruction> instr = create_instruction_from_xlsx(addr_instr, opcode, scene_name);
                if (instr != nullptr) {
                    auto modified_instr = read_instruction_xlsx(addr_instr, idx_row, xls_content, instr);
                    instr.swap(modified_instr);
                } else {
                    error = true;
                    ssd::spdlog::err("Opcode {#04x} is not defined", opcode);
                }
                current_fun.add_instruction(instr);
                idx_row++;
            }
        }
        functions_parsed.push_back(current_fun);

        ssd::Buffer header = create_header_bytes();

        int start_header = static_cast<int>(std::ssize(header));
        int fun_addr = start_header;

        for (size_t idx_fun = 0; idx_fun < functions_parsed.size() - 1; idx_fun++) {
            auto& fun = functions_parsed[idx_fun];
            fun.set_addr(fun_addr);

            int length = fun.get_length_in_bytes();

            fun.end_addr = length + fun.actual_addr;
            fun_addr = length + fun.actual_addr;
            int multiple = 4;
            if (isMultiple0x10(functions_parsed[idx_fun + 1].name)) multiple = 0x10;

            int padding = (((int)std::ceil((float)fun_addr / (float)multiple))) * multiple - fun_addr;

            fun_addr += padding;

            for (auto& instr : fun.instructions) {
                instr->set_addr_instr(instr->get_addr_instr() + fun.actual_addr);
            }
        }
        auto& last_fun = functions_parsed.back();
        last_fun.actual_addr = fun_addr;
        int length = last_fun.get_length_in_bytes();
        last_fun.end_addr = length + last_fun.actual_addr;

        for (auto& last_fun_instr : last_fun.instructions) {
            last_fun_instr->set_addr_instr(last_fun_instr->get_addr_instr() + last_fun.actual_addr);
        }

        update_pointers_xlsx();
    }
    display_text("XLSX file read.");
}

std::shared_ptr<Instruction> Builder::read_instruction_xlsx(int& addr,
                                                            int idx_row,
                                                            QXlsx::Document& excelScenarioSheet,
                                                            const std::shared_ptr<Instruction>& instruction) {
    std::stack<std::pair<size_t, uint8_t>> stack_of_headers;

    if (instruction->get_opcode() <= 0xFF) addr++;

    int idx_column = 3;
    std::string type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();

    while (!type.empty()) {
        ssd::Buffer Value;
        Operande operande;
        if (type == "int") {
            int op = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            Value = GetBytesFromInt(op);
            operande = Operande(addr, "int", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "Group ID") {
            stack_of_headers.push(std::make_pair(0, addr - 1)); // the count includes OPCode thus -1

            int operande_grp = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            idx_column++;
            uint8_t operande_length = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            // If the byte for length is 0xFF we don't calculate it
            if (operande_length != 0xFF) {
                stack_of_headers.top().first = instruction->get_nb_operandes();
            }
            Value = GetBytesFromInt(operande_grp + (operande_length << 0x18));
            operande = Operande(addr, "header", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();

        } else if (type == "float") {
            float op = excelScenarioSheet.read(idx_row + 1, idx_column).toFloat();

            Value = GetBytesFromFloat(op);
            operande = Operande(addr, "float", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "short") {
            int16_t op = static_cast<int16_t>(excelScenarioSheet.read(idx_row + 1, idx_column).toInt());
            Value = GetBytesFromShort(op);
            operande = Operande(addr, "short", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if ((type == "byte") || (type == "OP Code")) {
            int opcode = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            if (opcode <= 0xFF) { // actually the opposite never happens.
                auto op = static_cast<char>(((opcode)&0x000000FF));
                Value.push_back(op);
                operande = Operande(addr, "byte", Value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            }
        } else if (type == "End") {
            if (!stack_of_headers.empty()) {
                if (stack_of_headers.top().first != 0) {
                    uint32_t new_header_value =
                      ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                      (instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
                    ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
                    instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
                    stack_of_headers.pop();
                }
            }
        } else if (type == "fill") {
            QString operande_prev = excelScenarioSheet.read(idx_row + 1, idx_column - 1).toString();
            QString op = excelScenarioSheet.read(idx_row + 1, idx_column).toString();
            QString str_max_length = op.mid(1, op.indexOf('-') - 1);

            for (int id = 0; id < str_max_length.toInt() - operande_prev.toUtf8().size() - 1; id++) {
                Value.push_back('\0');
            }
            operande = Operande(addr, "fill", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if ((type == "string") || (type == "dialog")) {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());
            QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(OutputDatFileEncoding).toUtf8());
            auto q_byte_array = codec->fromUnicode(op);
            Value = ssd::Buffer(std::begin(q_byte_array), std::end(q_byte_array));
            if (type == "string") Value.push_back('\0');
            Value.replace('\n', 1);

            operande = Operande(addr, type, Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "bytearray") {
            while (type == "bytearray") {
                auto op = static_cast<char>(((excelScenarioSheet.read(idx_row + 1, idx_column).toInt()) & 0x000000FF));
                Value.push_back(op);
                idx_column++;
                type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
            }
            operande = Operande(addr, "bytearray", Value);
            idx_column--;
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "pointer") {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());

            QString RowPointedStr = op.right(op.size() - 2);

            int actual_row = RowPointedStr.toInt();

            Value = GetBytesFromInt(actual_row);

            auto operande = Operande(addr, "pointer", Value);
            instruction->add_operande(operande);
            addr = addr + operande.get_length();
        }
        idx_column++;
        type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    }
    if (!stack_of_headers.empty()) {
        if (stack_of_headers.top().first != 0) {
            uint32_t new_header_value =
              ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
              (instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
            ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
            instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
            stack_of_headers.pop();
        }
    }
    return instruction;
}

void Builder::read_functions_dat(ssd::Buffer& dat_content) {
    // From what I've seen, some functions in the file don't use OP Codes and it's not very explicit
    if (!functions_to_parse.empty()) {
        for (auto& fun : functions_to_parse) {
            if (std::count(functions_parsed.begin(), functions_parsed.end(), fun) == 0) {
                ssd::spdlog::trace("Reading function {}", fun.name);

                auto found_fun = find_function_by_id(functions_parsed, fun.id);
                if (found_fun == functions_parsed.end()) { // if we never read it, we'll do that
                    idx_current_fun = fun.id;
                    guess_type_by_name(fun);
                    read_individual_function(fun, dat_content);
                    functions_parsed.push_back(fun);
                }
            }
        }
        std::sort(functions_parsed.begin(), functions_parsed.end());

        update_pointers_dat();

        int current_addr = functions_parsed.front().actual_addr; // first function shouldn't have changed
        for (size_t idx_fun = 1; idx_fun < functions_parsed.size(); idx_fun++) {

            current_addr = current_addr + functions_parsed[idx_fun - 1].get_length_in_bytes();
            int multiple = 4;
            if (isMultiple0x10(functions_parsed[idx_fun].name)) multiple = 0x10;

            int padding = (((int)std::ceil((float)current_addr / (float)multiple))) * multiple - current_addr;
            current_addr = current_addr + padding;
            functions_parsed[idx_fun].set_addr(current_addr);
        }
    }
}
std::vector<int> Builder::guess_type_by_name(Function& fun) {
    std::vector<int> result;
    if (fun.name == "ActionTable") {
        result.push_back(3);
    } else if (fun.name == "AlgoTable") {
        result.push_back(4);
    } else if (fun.name == "WeaponAttTable") {
        result.push_back(5);
    } else if (fun.name == "BreakTable") {
        result.push_back(6);
    } else if (fun.name == "SummonTable") {
        result.push_back(7);
    } else if (fun.name == "ReactionTable") {
        result.push_back(8);
    } else if (fun.name == "PartTable") {
        result.push_back(9);
    } else if (fun.name == "AnimeClipTable") {
        result.push_back(10);
    } else if (fun.name == "FieldMonsterData") {
        result.push_back(11);
    } else if (fun.name == "FieldFollowData") {
        result.push_back(12);
    } else if (fun.name.starts_with("FC_auto")) {
        result.push_back(13);
    } else if (fun.name.starts_with("BookData")) { // Book: the first short read is crucial I think. 0 = text incoming; not zero =
        if (fun.name.ends_with("99")) {
            result.push_back(14);
        } else {
            result.push_back(15);
        }
    } else if (fun.name.starts_with("_")) {
        result.push_back(0);
        result.push_back(2);
    } else if (fun.name == "AddCollision") {
        result.push_back(16);
    } else if (fun.name == "ConditionTable") {
        result.push_back(17);
    } else if (fun.name.starts_with("StyleName")) {
        result.push_back(18);
    } else {
        result.push_back(0);
        result.push_back(1);
        result.push_back(2);
    }
    return result;
}

int Builder::attempts_at_reading_function(Function& fun, ssd::Buffer& dat_content, const std::vector<int>& fallback_types) {
    int current_position = fun.actual_addr;
    this->goal = fun.end_addr;
    uint32_t latest_op_code = 1;
    size_t fails = 0;

    /*
     Possible problems when parsing an instruction:
      - the (wrongly interpreted) instruction go past the "goal" (end_addr)
      - We encounter an OP code that does not match any instruction
      - We go past dat_content size
      - The previous instruction had op code 0, and the current has something else (this shouldn't happen since 0 is for padding at
      the end of the function, but could technically happen if there is a return instruction in the middle of the function... let's hope not

      if there is one of those problems, the decompiler switches to the fallback functions (which have been decided based on name)

      if even the fallbacks fail, the decompiler concludes the function is an OP Code function with an incorrect instruction somewhere,
      it will generate the file but the function containing the instruction might be wrongly parsed (and will need to be fixed in xlsx)
    */
    for (const auto& function_type : fallback_types) {
        try {
            if (function_type == 0) {
                while (current_position < this->goal) {
                    std::shared_ptr<Instruction> instr = create_instruction_from_dat(current_position, dat_content, function_type);
                    fun.add_instruction(instr);
                    latest_op_code = instr->get_opcode();
                }
                if (current_position != this->goal) throw ssd::exceptions::unspecified_recoverable();
            } else {
                std::shared_ptr<Instruction> instr = create_instruction_from_dat(current_position, dat_content, function_type);
                fun.add_instruction(instr);
                std::shared_ptr<Instruction> return_instr = create_instruction_from_dat(current_position, dat_content, 0);
                fun.add_instruction(return_instr);
                while (current_position < this->goal) {
                    if ((uint8_t)dat_content[current_position] != 0) throw ssd::exceptions::unspecified_recoverable();
                    current_position++;
                }
            }
            return current_position;
        } catch (const ssd::exceptions::recoverable& e) {
            // we retry with another function type candidate
            fun.instructions.clear();
            current_position = fun.actual_addr;
            fails++;
        }
    }
    if (fails == fallback_types.size()) {
        latest_op_code = 1;

        //The first parsing attempt failed, so we need interpret it as OPCode function
        fun.instructions.clear();
        current_position = fun.actual_addr;
        while (current_position < this->goal) {
            std::shared_ptr<Instruction> instr;
            try {
                instr = create_instruction_from_dat(current_position, dat_content, 0);
                if ((instr->get_opcode() == 0) && ((latest_op_code != 1) && (latest_op_code != 0))) {
                    throw ssd::exceptions::unspecified_recoverable();
                }
                fun.add_instruction(instr);

                latest_op_code = instr->get_opcode();
            } catch (const ssd::exceptions::recoverable& e) {
                if (instr != nullptr) {
                    instr->error = true;
                    fun.add_instruction(instr);
                    latest_op_code = instr->get_opcode();
                }
                display_text("Incorrect instruction read at " + std::to_string(current_position) + ". Skipped.");
                current_position++;
            }
        }
    }
    return current_position;
}
int Builder::read_individual_function(Function& fun, ssd::Buffer& dat_content) {

    std::vector<int> fallback_types = guess_type_by_name(fun);
    int current_position = attempts_at_reading_function(fun, dat_content, fallback_types);

    return current_position;
}

bool Builder::update_pointers_dat() {
    for (auto& idx_fun : functions_parsed) {
        for (auto& idx_instr : idx_fun.instructions) {
            for (auto& operande : idx_instr->operandes) {
                if (operande.get_type() == "pointer") {
                    int addr_ptr = operande.get_integer_value();
                    int idx_fun_ = find_function(addr_ptr);
                    if (idx_fun_ != -1) {
                        Function fun = functions_parsed[idx_fun_];
                        int id_instr = fun.find_instruction_idx(addr_ptr);
                        if (id_instr != -1) {
                            operande.set_destination(fun.id, id_instr);
                        } else {
                            ssd::spdlog::warn("Couldn't find instruction for address: {:#04x}", addr_ptr);
                            display_text(fmt::format("Couldn't find instruction {:#04x}", addr_ptr));

                            operande.set_destination(fun.id, 0);
                        }
                    } else {
                        operande.set_destination(0, 0);
                    }
                }
            }
        }
    }
    ssd::spdlog::trace("Done");

    return true;
}
bool Builder::reset() {

    functions_parsed.clear();
    functions_to_parse.clear();
    return true;
}
bool Builder::update_pointers_xlsx() {
    for (auto& fun : functions_parsed) {
        for (auto& instr : fun.instructions) {
            for (auto& operande : instr->operandes) {
                if (operande.get_type() == "pointer") {
                    int idx_row_ptr = operande.get_integer_value();

                    Function current_fun = functions_parsed[0];

                    if (functions_parsed.size() > 1) {
                        Function next_fun = functions_parsed[1];
                        for (size_t idx_fun_next = 1; idx_fun_next < functions_parsed.size(); idx_fun_next++) {
                            if (idx_row_ptr < next_fun.xlsx_row_index) {
                                break;
                            }
                            current_fun = functions_parsed[idx_fun_next];
                            if ((idx_fun_next + 1) < functions_parsed.size()) {
                                next_fun = functions_parsed[idx_fun_next + 1];
                            }
                        }
                    }
                    int nb_instruction_inside_function = (idx_row_ptr - (current_fun.xlsx_row_index + 1)) / 2;
                    int addr_pointed = current_fun.instructions[nb_instruction_inside_function]->get_addr_instr();
                    operande.set_value(GetBytesFromInt(addr_pointed));
                }
            }
        }
    }

    return true;
}
int Builder::find_function(int addr) {
    int result = -1;

    for (size_t idx_fun = 0; idx_fun < functions_parsed.size(); idx_fun++) {
        int fun_addr = functions_parsed[idx_fun].actual_addr;

        if (addr < fun_addr) {
            result = static_cast<int>(idx_fun) - 1;
            break;
        }
    }
    if ((result == -1) && (addr < functions_parsed.back().end_addr)) {
        result = static_cast<int>(functions_parsed.size()) - 1;
    }

    return result;
}
