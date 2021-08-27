#include "headers/Builder.h"
#include "headers/functions.h"
Builder::Builder() = default;
void Builder::ReadFunctionsXLSX(QXlsx::Document& doc) {
    SceneName = doc.read(2, 1).toString();
    int first_row = 4;
    int last_row = doc.dimension().lastRow();
    int ID_fun = 0;

    QString content_first_cell = doc.read(first_row, 1).toString();
    if (content_first_cell == "FUNCTION") {

        function current_fun;
        current_fun.name = doc.read(first_row, 2).toString();
        current_fun.ID = ID_fun;
        int addr_instr = 0;
        current_fun.declr_position = 0;
        current_fun.XLSX_row_index = first_row;
        current_fun.InstructionsInFunction.clear();
        current_fun.actual_addr = 0;

        for (int idx_row = first_row + 1; idx_row < last_row; idx_row++) {

            QString content_first_cell = doc.read(idx_row, 1).toString();
            if (content_first_cell == "FUNCTION") { // We start a new function

                QString next_fun_name = doc.read(idx_row, 2).toString();

                addr_instr = 0;
                FunctionsParsed.push_back(current_fun);
                current_fun.name = next_fun_name;
                current_fun.ID = ID_fun;
                current_fun.declr_position = 0;
                current_fun.XLSX_row_index = idx_row;
                current_fun.InstructionsInFunction.clear();
                ID_fun++;

            } else {

                std::shared_ptr<Instruction> instr = CreateInstructionFromXLSX(addr_instr, idx_row, doc);
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

            int padding = (((int)ceil((float)addr_fun / multiple))) * multiple - addr_fun;

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
                qDebug() << "Reading function "
                         << it.name; // << "at addr " << hex << it->actual_addr << " and ending at " << hex << it->end_addr;

                std::vector<function>::iterator itt = find_function_by_ID(FunctionsParsed, it.ID);
                if (itt == FunctionsParsed.end()) { // if we never read it, we'll do that
                    idx_current_fun = it.ID;
                    ReadIndividualFunction(it, dat_content);
                    FunctionsParsed.push_back(it);
                }
                previous_fun_name = it.name;
            }
        }

        std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

        UpdatePointersDAT();
        int current_addr = FunctionsParsed[0].actual_addr; // first function shouldn't have changed
        for (uint idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++) {

            current_addr = current_addr + FunctionsParsed[idx_fun - 1].get_length_in_bytes();
            int multiple = 4;
            if (isMultiple0x10(FunctionsParsed[idx_fun].name)) multiple = 0x10;

            int padding = (((int)ceil((float)current_addr / multiple))) * multiple - current_addr;
            current_addr = current_addr + padding;
            FunctionsParsed[idx_fun].SetAddr(current_addr);
        }
    }
}
int Builder::ReadIndividualFunction(function& fun, QByteArray& dat_content) {
    int current_position = fun.actual_addr;
    goal = fun.end_addr;
    std::shared_ptr<Instruction> instr;
    int latest_op_code = 1;

    int function_type = 0;
    if (fun.name == "ActionTable") {
        function_type = 3;
    } else if (fun.name == "AlgoTable") {
        function_type = 4;
    } else if (fun.name == "WeaponAttTable") {
        function_type = 5;
    } else if (fun.name == "BreakTable") {
        function_type = 6;
    } else if (fun.name == "SummonTable") {
        function_type = 7;
    } else if (fun.name == "ReactionTable") {
        function_type = 8;
    } else if (fun.name == "PartTable") {
        function_type = 9;
    } else if (fun.name == "AnimeClipTable") {
        function_type = 10;
    } else if (fun.name == "FieldMonsterData") {
        function_type = 11;
    } else if (fun.name == "FieldFollowData") {
        function_type = 12;
    } else if (fun.name.startsWith("FC_auto")) {
        function_type = 13;
    } else if (fun.name.startsWith("BookData")) { // Book: the first short read is crucial I think. 0 = text incoming; not zero =
        QRegExp rx("BookData(\\d+[A-Z]?)_(\\d+)");
        std::vector<int> result;

        rx.indexIn(fun.name, 0);
        int Nb_Data = rx.cap(2).toInt();

        if (Nb_Data == 99) {
            function_type = 14; // DATA, the 99 is clearly hardcoded; The behaviour is: 99=> two shorts (probably number
                                // of pages) and that's it

        } else {
            function_type = 15;
        }

    } else if (fun.name.startsWith("_")) {
        if ((fun.name != "_" + previous_fun_name) ||
            fun.end_addr == static_cast<uint>(dat_content.size())) // last one is for btl1006, cs3; not cool but I'm starting to feel
                                                                   // like the "_" functions are just not supposed to exist, so this
                                                                   // hack only helps me checking the integrity of the files
        {
            function_type = 2;
        }

    } else if (fun.name == "AddCollision") {
        function_type = 16;
    } else if (fun.name == "ConditionTable") {
        function_type = 17;
    } else if (fun.name.startsWith("StyleName")) {
        function_type = 18;
    }

    if (function_type == 0) { // we use OP codes

        while (current_position < goal) {

            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);

            if ((error) || ((instr->get_OP() != 0) &&
                            (latest_op_code == 0))) { // this clearly means the function is incorrect or is a monster function.

                error = false;
                fun.InstructionsInFunction.clear();
                current_position = fun.actual_addr;
                if (fun.name.startsWith("_")) {
                    instr = CreateInstructionFromDAT(current_position, dat_content, 2);
                    fun.AddInstruction(instr);
                    instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                    fun.AddInstruction(instr);
                    return current_position;

                } else {
                    instr = CreateInstructionFromDAT(current_position, dat_content, 1);

                    if (flag_monsters) {
                        fun.AddInstruction(instr);
                        instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                        fun.AddInstruction(instr);
                        return current_position;
                    } else {

                        // the function is incorrect, therefore, we parse it again as an OP Code function but remove the
                        // part that is incorrect qDebug() << "Fail. There is a problem with this function at offset " <<
                        // hex << current_position;
                        current_position = fun.actual_addr;
                        while (current_position < goal) {
                            instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                            if (error) {
                                // qFatal("error");
                                display_text("Incorrect instruction read at " + QString::number(current_position) + ". Skipped.");
                                error = false;

                            } else {
                                fun.AddInstruction(instr);
                            }
                        }
                        return current_position;
                    }
                }

            } else {
                fun.AddInstruction(instr);
            }
            latest_op_code = instr->get_OP();
        }
    } else {
        while (current_position < goal) {

            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);
            fun.AddInstruction(instr);
            function_type = 0;
        }
    }

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
                        int id_op = 0;
                        if (id_instr != -1) {
                            id_op = find_operande(addr_ptr, *fun.InstructionsInFunction[id_instr]);
                            operande.setDestination(fun.ID, id_instr, id_op);
                        } else {
                            operande.setDestination(fun.ID, 0, 0);
                        }
                    } else {
                        operande.setDestination(0, 0, 0);
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
                        for (uint idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++) {
                            if (idx_row_ptr < next_fun.XLSX_row_index) {
                                break;
                            }

                            current_fun = FunctionsParsed[idx_fun];
                            if ((idx_fun + 1) < FunctionsParsed.size()) next_fun = FunctionsParsed[idx_fun + 1];
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
int Builder::find_function(uint addr) {
    int result = -1;

    for (uint idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++) {
        uint fun_addr = FunctionsParsed[idx_fun].actual_addr;

        if (addr < fun_addr) {

            result = idx_fun - 1;
            break;
        }
    }
    if ((result == -1) && (addr < FunctionsParsed[FunctionsParsed.size() - 1].end_addr)) result = FunctionsParsed.size() - 1;

    return result;
}
int Builder::find_instruction(uint addr, function fun) {
    uint result = -1;
    uint idx_instr = 0;
    bool success = false;
    for (; idx_instr < fun.InstructionsInFunction.size(); idx_instr++) {
        uint instr_addr = fun.InstructionsInFunction[idx_instr]->get_addr_instr();
        if (addr == instr_addr) {
            success = true;
            result = idx_instr;

            break;
        }
    }

    if (!success) {
        qDebug() << Qt::hex << addr;
        display_text("Couldn't find an instruction!");
    }

    return result;
}
int Builder::find_operande(uint addr,
                           Instruction instr) { // NOT USEFUL! Since we should point towards OP codes exclusively

    int idx_operande = 0;
    for (; idx_operande < instr.get_Nb_operandes(); idx_operande++) {

        operande ope = instr.get_operande(idx_operande);
        uint ope_addr = ope.getAddr();
        if (addr == ope_addr) {
            break;
        }
    }
    return idx_operande;
}
