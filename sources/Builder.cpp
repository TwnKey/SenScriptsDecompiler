#include "headers/Builder.h"
#include "headers/functions.h"
#include <string>

void Builder::ReadFunctionsXLSX(QXlsx::Document& xls_content) {
    SceneName = xls_content.read(2, 1).toString().toStdString();
    int first_row = 4;
    int last_row = xls_content.dimension().lastRow();
    int ID_fun = 0;

    QString content_first_cell = xls_content.read(first_row, 1).toString();
    if (content_first_cell == "FUNCTION") {

        function current_fun;
        current_fun.name = xls_content.read(first_row, 2).toString().toStdString();
        current_fun.ID = ID_fun;
        int addr_instr = 0;
        current_fun.declr_position = 0;
        current_fun.XLSX_row_index = first_row;
        current_fun.InstructionsInFunction.clear();
        current_fun.actual_addr = 0;

        for (int idx_row = first_row + 1; idx_row < last_row; idx_row++) {

            content_first_cell = xls_content.read(idx_row, 1).toString();
            if (content_first_cell == "FUNCTION") { // We start a new function

                std::string next_fun_name = xls_content.read(idx_row, 2).toString().toStdString();

                addr_instr = 0;
                FunctionsParsed.push_back(current_fun);
                current_fun.name = next_fun_name;
                current_fun.ID = ID_fun;
                current_fun.declr_position = 0;
                current_fun.XLSX_row_index = idx_row;
                current_fun.InstructionsInFunction.clear();
                ID_fun++;

            } else {

                std::shared_ptr<Instruction> instr = CreateInstructionFromXLSX(addr_instr, idx_row, xls_content);
                current_fun.AddInstruction(instr);

                idx_row++;
            }
        }
        int addr_fun = 0;
        FunctionsParsed.push_back(current_fun);

        QByteArray header = CreateHeaderBytes();

        int start_header = header.size();
        addr_fun = start_header;
        for (uint idx_fun = 0; idx_fun < FunctionsParsed.size() - 1; idx_fun++) {
            FunctionsParsed[idx_fun].actual_addr = addr_fun;

            int length = FunctionsParsed[idx_fun].get_length_in_bytes();

            FunctionsParsed[idx_fun].end_addr = length + FunctionsParsed[idx_fun].actual_addr;
            addr_fun = length + FunctionsParsed[idx_fun].actual_addr;
            int multiple = 4;
            if (isMultiple0x10(FunctionsParsed[idx_fun + 1].name)) multiple = 0x10;

            int padding = (((int)std::ceil((float)addr_fun / (float)multiple))) * multiple - addr_fun;

            addr_fun = addr_fun + padding;

            for (uint idx_instr = 0; idx_instr < FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++) {
                FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->set_addr_instr(
                  FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->get_addr_instr() + FunctionsParsed[idx_fun].actual_addr);
            }
        }
        FunctionsParsed[FunctionsParsed.size() - 1].actual_addr = addr_fun;
        int length = FunctionsParsed[FunctionsParsed.size() - 1].get_length_in_bytes();
        FunctionsParsed[FunctionsParsed.size() - 1].end_addr = length + FunctionsParsed[FunctionsParsed.size() - 1].actual_addr;
        for (uint idx_instr = 0; idx_instr < FunctionsParsed[FunctionsParsed.size() - 1].InstructionsInFunction.size(); idx_instr++) {
            FunctionsParsed[FunctionsParsed.size() - 1].InstructionsInFunction[idx_instr]->set_addr_instr(
              FunctionsParsed[FunctionsParsed.size() - 1].InstructionsInFunction[idx_instr]->get_addr_instr() +
              FunctionsParsed[FunctionsParsed.size() - 1].actual_addr);
        }

        UpdatePointersXLSX();
    }

    display_text("XLSX file read.");
}

void Builder::ReadFunctionsDAT(QByteArray& dat_content) {
    // From what I've seen, some functions in the file don't use OP Codes and it's not very explicit
    if (!FunctionsToParse.empty()) {
        for (auto& it : FunctionsToParse) {
            if (std::count(FunctionsParsed.begin(), FunctionsParsed.end(), it) == 0) {
                qDebug() << "Reading function " << QString::fromStdString(it.name);

                auto itt = find_function_by_ID(FunctionsParsed, it.ID);
                if (itt == FunctionsParsed.end()) { // if we never read it, we'll do that
                    idx_current_fun = it.ID;
                    guess_type_by_name(it);
                    ReadIndividualFunction(it, dat_content);
                    FunctionsParsed.push_back(it);
                }
            }
        }

        std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

        UpdatePointersDAT();
        int current_addr = FunctionsParsed[0].actual_addr; // first function shouldn't have changed
        for (uint idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++) {

            current_addr = current_addr + FunctionsParsed[idx_fun - 1].get_length_in_bytes();
            int multiple = 4;
            if (isMultiple0x10(FunctionsParsed[idx_fun].name)) multiple = 0x10;

            int padding = (((int)std::ceil((float)current_addr / (float)multiple))) * multiple - current_addr;
            current_addr = current_addr + padding;
            FunctionsParsed[idx_fun].SetAddr(current_addr);
        }
    }
}
std::vector<int> Builder::guess_type_by_name(function& fun) {
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
        QRegExp rx("BookData(\\d+[A-Z]?)_(\\d+)");

        rx.indexIn(QString::fromStdString(fun.name), 0);
        int Nb_Data = rx.cap(2).toInt();

        if (Nb_Data == 99) {
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

int Builder::attempts_at_reading_function(function& fun, QByteArray& dat_content, const std::vector<int>& fallback_types) {
    int current_position = fun.actual_addr;
    this->goal = fun.end_addr;
    uint latest_op_code = 1;
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
        try{
            if (function_type == 0) {
                while (current_position < this->goal) {
                    std::shared_ptr<Instruction> instr = CreateInstructionFromDAT(current_position, dat_content, function_type);

                    fun.AddInstruction(instr);

                    latest_op_code = instr->get_OP();
                }
                if (current_position != this->goal) throw ssd::exceptions::unspecified_recoverable();
            } else {
                std::shared_ptr<Instruction> instr = CreateInstructionFromDAT(current_position, dat_content, function_type);
                fun.AddInstruction(instr);
                std::shared_ptr<Instruction> return_instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                fun.AddInstruction(return_instr);
                while (current_position < this->goal) {
                    if ((uint8_t)dat_content[current_position] != 0) throw ssd::exceptions::unspecified_recoverable();
                    current_position++;
                }
            }
            return current_position;
        } catch (const ssd::exceptions::recoverable& e) {
            // we retry with another function type candidate
            fun.InstructionsInFunction.clear();
            current_position = fun.actual_addr;
            fails++;
        }
    }
    if (fails == fallback_types.size()) {
        latest_op_code = 1;

        //The first parsing attempt failed, so we need interpret it as OPCode function
        fun.InstructionsInFunction.clear();
        current_position = fun.actual_addr;
        while (current_position < this->goal) {
            std::shared_ptr<Instruction> instr;
            try {
                instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                if ((instr->get_OP() == 0) && ((latest_op_code != 1) && (latest_op_code != 0))) {
                    throw ssd::exceptions::unspecified_recoverable();
                }
                fun.AddInstruction(instr);

                latest_op_code = instr->get_OP();
            } catch (const ssd::exceptions::recoverable& e) {
                if (instr != nullptr) {
                    instr->error = true;
                    fun.AddInstruction(instr);
                    latest_op_code = instr->get_OP();
                }

                display_text("Incorrect instruction read at " + std::to_string(current_position) + ". Skipped.");
                current_position++;
            }
        }
    }
    return current_position;
}
int Builder::ReadIndividualFunction(function& fun, QByteArray& dat_content) {

    std::vector<int> fallback_types = guess_type_by_name(fun);
    int current_position = attempts_at_reading_function(fun, dat_content, fallback_types);

    return current_position;
}

bool Builder::UpdatePointersDAT() {

    for (auto& idx_fun : FunctionsParsed) {

        std::vector<std::shared_ptr<Instruction>> instructions = idx_fun.InstructionsInFunction;
        for (auto& idx_instr : idx_fun.InstructionsInFunction) {

            for (auto& operande : idx_instr->operandes) {
                if (operande.getType() == "pointer") {

                    int addr_ptr = operande.getIntegerValue();
                    int idx_fun_ = find_function(addr_ptr);
                    if (idx_fun_ != -1) {
                        function fun = FunctionsParsed[idx_fun_];
                        int id_instr = find_instruction(addr_ptr, fun);
                        if (id_instr != -1) {
                            operande.setDestination(fun.ID, id_instr);
                        } else {
                            operande.setDestination(fun.ID, 0);
                        }
                    } else {
                        operande.setDestination(0, 0);
                    }
                }
            }
        }
    }
    qDebug() << "Done";
    return true;
}
bool Builder::Reset() {

    FunctionsParsed.clear();
    FunctionsToParse.clear();
    return true;
}
bool Builder::UpdatePointersXLSX() {

    for (auto& idx_fun : FunctionsParsed) {

        std::vector<std::shared_ptr<Instruction>> instructions = idx_fun.InstructionsInFunction;
        for (auto& idx_instr : idx_fun.InstructionsInFunction) {

            for (uint idx_operand = 0; idx_operand < idx_instr->operandes.size(); idx_operand++) {
                if (idx_instr->operandes[idx_operand].getType() == "pointer") {
                    int idx_row_ptr = idx_instr->operandes[idx_operand].getIntegerValue();
                    function current_fun = FunctionsParsed[0];

                    if (FunctionsParsed.size() > 1) {

                        function next_fun = FunctionsParsed[1];
                        for (uint idx_fun_next = 1; idx_fun_next < FunctionsParsed.size(); idx_fun_next++) {
                            if (idx_row_ptr < next_fun.XLSX_row_index) {
                                break;
                            }

                            current_fun = FunctionsParsed[idx_fun_next];
                            if ((idx_fun_next + 1) < FunctionsParsed.size()) next_fun = FunctionsParsed[idx_fun_next + 1];
                        }
                    }
                    int nb_instruction_inside_function = (idx_row_ptr - (current_fun.XLSX_row_index + 1)) / 2;
                    int addr_pointed = current_fun.InstructionsInFunction[nb_instruction_inside_function]->get_addr_instr();
                    idx_instr->operandes[idx_operand].setValue(GetBytesFromInt(addr_pointed));
                }
            }
        }
    }

    return true;
}
int Builder::find_function(int addr) {
    int result = -1;

    for (size_t idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++) {
        int fun_addr = FunctionsParsed[idx_fun].actual_addr;

        if (addr < fun_addr) {

            result = static_cast<int>(idx_fun) - 1;
            break;
        }
    }
    if ((result == -1) && (addr < FunctionsParsed[FunctionsParsed.size() - 1].end_addr)) {
        result = static_cast<int>(FunctionsParsed.size()) - 1;
    }

    return result;
}
int Builder::find_instruction(int addr, function fun) {
    int result = -1;
    size_t idx_instr = 0;
    bool success = false;
    for (; idx_instr < fun.InstructionsInFunction.size(); idx_instr++) {
        int instr_addr = fun.InstructionsInFunction[idx_instr]->get_addr_instr();
        if (addr == instr_addr) {
            success = true;
            result = static_cast<int>(idx_instr);

            break;
        }
    }

    if (!success) {
        qDebug() << Qt::hex << addr;
        display_text("Couldn't find an instruction!");
    }

    return result;
}

