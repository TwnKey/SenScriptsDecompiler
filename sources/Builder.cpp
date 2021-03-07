#include "headers/Builder.h"
#include "headers/functions.h"
Builder::Builder()
{

}
void Builder::ReadFunctionsXLSX(QXlsx::Document &doc){
    SceneName = doc.read(2, 1).toString();
    int first_row = 4;
    int last_row = doc.dimension().lastRow();
    int ID_fun = 0;

    QString content_first_cell = doc.read(first_row, 1).toString();
    if (content_first_cell == "FUNCTION"){
        function current_fun;
        current_fun.name  = doc.read(first_row, 2).toString();
        current_fun.ID = ID_fun;
        int addr_instr = 0;
        current_fun.declr_position = 0;
        current_fun.XLSX_row_index = first_row;
        current_fun.InstructionsInFunction.clear();
        current_fun.actual_addr = 0;

        for (int idx_row = first_row+1; idx_row < last_row; idx_row++){

            QString content_first_cell = doc.read(idx_row, 1).toString();
            if (content_first_cell == "FUNCTION"){ //We start a new function

                QString next_fun_name = doc.read(idx_row, 2).toString();
                addr_instr = 0;
                FunctionsParsed.push_back(current_fun);
                current_fun.name  = next_fun_name;
                current_fun.ID = ID_fun;
                current_fun.declr_position = 0;
                current_fun.XLSX_row_index = idx_row;
                current_fun.InstructionsInFunction.clear();
                ID_fun++;

            }
            else{

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
        for (uint idx_fun = 0; idx_fun<FunctionsParsed.size()-1; idx_fun++){
            FunctionsParsed[idx_fun].actual_addr = addr_fun;

            int length = FunctionsParsed[idx_fun].get_length_in_bytes();

            FunctionsParsed[idx_fun].end_addr = length + FunctionsParsed[idx_fun].actual_addr;
            addr_fun =  length + FunctionsParsed[idx_fun].actual_addr;
            int multiple = 4;
            if (isMultiple0x10(FunctionsParsed[idx_fun+1].name)) multiple = 0x10;

            int padding = (((int) ceil((float)addr_fun/multiple)))*multiple - addr_fun;

            addr_fun = addr_fun + padding;


            for (uint idx_instr = 0; idx_instr<FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){
               FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->set_addr_instr(FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->get_addr_instr()+ FunctionsParsed[idx_fun].actual_addr);
            }
        }
        FunctionsParsed[FunctionsParsed.size()-1].actual_addr = addr_fun;
        int length = FunctionsParsed[FunctionsParsed.size()-1].get_length_in_bytes();
        FunctionsParsed[FunctionsParsed.size()-1].end_addr = length + FunctionsParsed[FunctionsParsed.size()-1].actual_addr;
        for (uint idx_instr = 0; idx_instr<FunctionsParsed[FunctionsParsed.size()-1].InstructionsInFunction.size(); idx_instr++){
           FunctionsParsed[FunctionsParsed.size()-1].InstructionsInFunction[idx_instr]->set_addr_instr(FunctionsParsed[FunctionsParsed.size()-1].InstructionsInFunction[idx_instr]->get_addr_instr()+ FunctionsParsed[FunctionsParsed.size()-1].actual_addr);
        }


        UpdatePointersXLSX();
    }

    display_text("XLSX file read.");
}

void Builder::ReadFunctionsDAT(QByteArray &dat_content){
    //From what I've seen, some functions in the file don't use OP Codes and it's not very explicit
    if (FunctionsToParse.size()>0){
        for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++){
            if (!std::count(FunctionsParsed.begin(), FunctionsParsed.end(), *it)){
                qDebug() << "Reading function " << it->name << "at addr " << hex << it->actual_addr << " and ending at " << hex << it->end_addr;
                std::vector<function>::iterator itt = find_function_by_ID(FunctionsParsed, it->ID);
                if (itt == FunctionsParsed.end()){ //if we never read it, we'll do that
                    ReadIndividualFunction(*it,dat_content);
                    FunctionsParsed.push_back(*it);
                }
            }


        }

        std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

        UpdatePointersDAT();
qDebug() << "DONE ";
        int current_addr = FunctionsParsed[0].actual_addr; //first function shouldn't have changed
        for (uint idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++){


            current_addr = current_addr + FunctionsParsed[idx_fun-1].get_length_in_bytes();
            int multiple = 4;
            if (isMultiple0x10(FunctionsParsed[idx_fun].name)) multiple = 0x10;

            int padding = (((int) ceil((float)current_addr/multiple)))*multiple - current_addr;
            current_addr = current_addr + padding;
            FunctionsParsed[idx_fun].SetAddr(current_addr);
        }


    }


}
int Builder::ReadIndividualFunction(function &fun,QByteArray &dat_content){
    int current_position = fun.actual_addr;
    goal = fun.end_addr;
    std::shared_ptr<Instruction> instr;
    int latest_op_code = 1;


    int function_type = 0;
    if (fun.name == "ActionTable"){
        function_type = 3;
    }
    else if (fun.name == "AlgoTable"){
        function_type = 4;
    }
    else if (fun.name == "WeaponAttTable"){
        function_type = 5;
    }
    else if (fun.name == "BreakTable"){
        function_type = 6;
    }
    else if (fun.name == "SummonTable"){
        function_type = 7;
    }
    else if (fun.name == "ReactionTable"){
        function_type = 8;
    }
    else if (fun.name == "PartTable"){
        function_type = 9;
    }
    else if (fun.name == "AnimeClipTable"){
        function_type = 10;
    }
    else if (fun.name == "FieldMonsterData"){
        function_type = 11;
    }
    else if (fun.name == "FieldFollowData"){
        function_type = 12;
    }
    else if (fun.name.startsWith("FC_auto")){ //14025d625 Only present in face.dat...
        function_type = 13;
    }
    else if (fun.name.startsWith("BookData")){ //Book: the first short read is crucial I think. 0 = text incoming; not zero =
        QRegExp rx("BookData(\\d+[A-Z]?)_(\\d+)");
        std::vector<int> result;
        int Nb_Book = rx.cap(1).toInt();
        qDebug() << Nb_Book;

        rx.indexIn(fun.name, 0);
        int Nb_Data = rx.cap(2).toInt();
        qDebug() << "nb_data = " << Nb_Data;
        qDebug() << "nb_book = " << Nb_Book;
        if (Nb_Data == 99) function_type = 14; //DATA, the 99 is clearly hardcoded; The behaviour is: 99=> two shorts (probably number of pages) and that's it

        else function_type = 15;

    }
    else if (fun.name.startsWith("_")) {
        function_type = 2;

    }
    else if (fun.name == "AddCollision"){
        function_type = 16;
    }
    else if (fun.name == "ConditionTable"){
        function_type = 17;
    }


    if (function_type == 0){ //we use OP codes
        //First we check if it's really using OP Code (might be a monster function)
        //There is more chance to fail when interpreting a monster function as an op code function
        //than the opposite
        //Although we'll also check if its a valid monster function afterwards
        //if it fails both checks, wtf. There is something wrong with the function, and we will skip the incorrect instruction (Hopefully it will fix the file)
        //it will also crash if there is a monster function at the end of the file cause I didn't put protections when parsing
        //the last function as an OP code, I mean it might go beyond the size of the dat content when reading the instructions (But I think it will fail
        //before that most of the time)

        while(current_position<goal){



            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);

            if ((error)||((instr->get_OP() != 0)&&(latest_op_code == 0))){ //this clearly means the function is incorrect or is a monster function.
                qDebug() << "it's incorrect or a monster function";
                error = false;
                fun.InstructionsInFunction.clear();
                current_position = fun.actual_addr;

                instr = CreateInstructionFromDAT(current_position, dat_content, 1);
                if (flag_monsters) {
                    fun.AddInstruction(instr);
                    instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                    fun.AddInstruction(instr);
                    return current_position;
                }
                else{
                    qDebug() << "not a monster function";
                    //the function is incorrect, therefore, we parse it again as an OP Code function but remove the part that is incorrect
                    //qDebug() << "Fail. There is a problem with this function at offset " << hex << current_position;
                    current_position = fun.actual_addr;
                    while(current_position<goal){
                        instr = CreateInstructionFromDAT(current_position, dat_content, 0);
                        if (error){
                            error = false;
                            qDebug() << hex << current_position;
                            qFatal("ERROR!"); //remove at release
                        }
                        else fun.AddInstruction(instr);
                    }
                    return current_position;
                }
            }
            else fun.AddInstruction(instr);
            latest_op_code = instr->get_OP();

        }
    }
    else
    {
        while(current_position<goal){

            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);
            fun.AddInstruction(instr);
            function_type = 0;

        }

    }

    return current_position;
}

bool Builder::UpdatePointersDAT(){

    for (uint idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++){

        std::vector<std::shared_ptr<Instruction>> instructions = FunctionsParsed[idx_fun].InstructionsInFunction;
        for (uint idx_instr = 0; idx_instr < FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){

            for (uint idx_operand = 0; idx_operand < FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes.size(); idx_operand++){
                if (FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getType()=="pointer"){
                    int addr_ptr = FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getIntegerValue();

                    function fun = find_function(addr_ptr);

                    int id_instr = find_instruction(addr_ptr,fun);

                    int id_op = find_operande(addr_ptr,*fun.InstructionsInFunction[id_instr]);

                    FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].setDestination(fun.ID, id_instr, id_op);

                }
            }
        }
    }
    return true;

}
bool Builder::Reset(){

    FunctionsParsed.clear();
    FunctionsToParse.clear();
    return true;
}
bool Builder::UpdatePointersXLSX(){


    for (uint idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++){

        std::vector<std::shared_ptr<Instruction>> instructions = FunctionsParsed[idx_fun].InstructionsInFunction;
        for (uint idx_instr = 0; idx_instr < FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){

            for (uint idx_operand = 0; idx_operand < FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes.size(); idx_operand++){
                if (FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getType()=="pointer"){
                    int idx_row_ptr = FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getIntegerValue();
                    function current_fun = FunctionsParsed[0];
                    if (FunctionsParsed.size()>1){

                        function next_fun = FunctionsParsed[1];
                        for (uint idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++){
                            if (idx_row_ptr<next_fun.XLSX_row_index){
                                break;
                            }

                            current_fun = FunctionsParsed[idx_fun];
                            if ((idx_fun+1)<FunctionsParsed.size()) next_fun = FunctionsParsed[idx_fun+1];
                        }

                       }
                    int nb_instruction_inside_function = (idx_row_ptr-(current_fun.XLSX_row_index+1))/2;
                    int addr_pointed = current_fun.InstructionsInFunction[nb_instruction_inside_function]->get_addr_instr();
                    FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].setValue(GetBytesFromInt(addr_pointed));
                }
            }
        }
    }



    return true;

}
function Builder::find_function(int addr){
    function result = FunctionsParsed[0];
    bool success = false;
    int fun_addr = FunctionsParsed[0].actual_addr;
    uint idx_fun = 0;
    for (; idx_fun < FunctionsParsed.size(); idx_fun++){
        fun_addr = FunctionsParsed[idx_fun].actual_addr;

        if (addr<fun_addr) {


            success = true;
            break;
        }

    }
    result = FunctionsParsed[idx_fun-1];

    return result;
}
int Builder::find_instruction(int addr, function fun){
    uint idx_instr = 0;
    bool success = false;
    for (;idx_instr < fun.InstructionsInFunction.size(); idx_instr++){
        int instr_addr = fun.InstructionsInFunction[idx_instr]->get_addr_instr();
        if (addr==instr_addr) {
            success = true;

            break;
        }

    }

    if (!success) {
        qDebug() << hex << addr;
        display_text("Couldn't find an instruction!");
    }
    return idx_instr;
}
int Builder::find_operande(int addr, Instruction instr){//NOT USEFUL! Since we should point towards OP codes exclusively

    int idx_operande = 0, result = -1;
    bool success = false;
    for (;idx_operande < instr.get_Nb_operandes(); idx_operande++){

        operande ope = instr.get_operande(idx_operande);
        int ope_addr = ope.getAddr();
        if (addr==ope_addr) {
            success = true;
            result = idx_operande;
            break;
        }
    }
    return idx_operande;
}


