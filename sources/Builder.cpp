#include "headers/Builder.h"
#include "headers/functions.h"
Builder::Builder()
{

}
void Builder::ReadFunctionsXLSX(QXlsx::Document &doc){
    SceneName = doc.read(2, 1).toString();
    int first_row = 4;
    int last_row = doc.dimension().lastRow();

    int addr = 0;
    int ID_fun = 0;
    function current_fun;
    for (int idx_row = first_row; idx_row < last_row; idx_row++){

        QString content_first_cell = doc.read(idx_row, 1).toString();
        if (content_first_cell == "FUNCTION"){ //We start a new function

            if (ID_fun!=0){
                int length = current_fun.get_length_in_bytes();
                addr =  length + current_fun.actual_addr;
                current_fun.end_addr = addr;
                FunctionsParsed.push_back(current_fun);
            }
            current_fun.name  = doc.read(idx_row, 2).toString();
            current_fun.ID = ID_fun;
            current_fun.actual_addr = addr;
            current_fun.declr_position = 0;
            current_fun.XLSX_row_index = idx_row;
            current_fun.InstructionsInFunction.clear();

            ID_fun++;

        }
        else{

                std::shared_ptr<Instruction> instr = CreateInstructionFromXLSX(addr, idx_row, doc);
                current_fun.AddInstruction(instr);
                idx_row++;

        }

    }
    //PadWithZeros(4,current_fun);
    FunctionsParsed.push_back(current_fun);
    qDebug() << " Updating addresses...";
    QByteArray header = CreateHeaderBytes();
    int start_header = header.size();
    for (int idx_fun = 0; idx_fun<FunctionsParsed.size(); idx_fun++){
        FunctionsParsed[idx_fun].actual_addr += start_header;
        for (int idx_instr = 0; idx_instr<FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){
           FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->set_addr_instr(FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->get_addr_instr()+start_header);
        }
    }
    qDebug() << " Updating pointers...";
    UpdatePointersXLSX();

}

void Builder::ReadFunctionsDAT(QByteArray &dat_content){
    //From what I've seen, some functions in the file don't use OP Codes and it's not very explicit

    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++) qDebug() << it->name << " id: " << hex << it->ID << " addr " << it->actual_addr << " ending at " << it->end_addr;


    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++){
        if (!std::count(FunctionsParsed.begin(), FunctionsParsed.end(), *it)){

            //qDebug() << "Function " << it->name << " at addr " << hex << it->actual_addr;
            ReadIndividualFunction(*it,dat_content);

        //once a function is read, it should be removed from the FunctionsToParse vector and added to the FunctionsParsed
            FunctionsParsed.push_back(*it);

        }


    }

    std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

    UpdatePointersDAT();
    int current_addr = FunctionsParsed[0].actual_addr; //first function shouldn't have changed
    for (int idx_fun = 1; idx_fun < FunctionsParsed.size()-1; idx_fun++){

        current_addr = current_addr + FunctionsParsed[idx_fun-1].get_length_in_bytes();
        FunctionsParsed[idx_fun].SetAddr(current_addr);
    }


}
int Builder::ReadIndividualFunction(function &fun,QByteArray &dat_content){
    int current_position = fun.actual_addr;

    int function_type = 0;
    if (!Passed_Monster_Functions){
        std::vector<function>::iterator InitIt = find_function_by_name(FunctionsToParse, "Init");
        if ((InitIt==FunctionsToParse.end())||((fun.name == "PreInit")||(fun.name == "Init"))) Passed_Monster_Functions = true;
    }


    if (fun.name.startsWith("_")) {
        function_type = 2;

    }
    else if (!Passed_Monster_Functions) function_type = 1;

    std::shared_ptr<Instruction> instr;
    if (function_type == 0){ //we use OP codes
        while(current_position<fun.end_addr){
            //qDebug() << " OP: " << hex << (unsigned char)dat_content[current_position] << " at " << hex << current_position;
            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);

            fun.AddInstruction(instr);

        }
    }
    else
    {
        if ((unsigned char)dat_content[current_position]!=0x1){

            instr = CreateInstructionFromDAT(current_position, dat_content, function_type);
            fun.AddInstruction(instr);
        }
        while(current_position<fun.end_addr){

            instr = CreateInstructionFromDAT(current_position, dat_content, 0);
            fun.AddInstruction(instr);

        }

    }

    return current_position;
}

bool Builder::UpdatePointersDAT(){

    for (int idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++){

        std::vector<std::shared_ptr<Instruction>> instructions = FunctionsParsed[idx_fun].InstructionsInFunction;
        for (int idx_instr = 0; idx_instr < FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){

            for (int idx_operand = 0; idx_operand < FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes.size(); idx_operand++){
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


    for (int idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++){

        std::vector<std::shared_ptr<Instruction>> instructions = FunctionsParsed[idx_fun].InstructionsInFunction;
        for (int idx_instr = 0; idx_instr < FunctionsParsed[idx_fun].InstructionsInFunction.size(); idx_instr++){

            for (int idx_operand = 0; idx_operand < FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes.size(); idx_operand++){
                if (FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getType()=="pointer"){
                    int idx_row_ptr = FunctionsParsed[idx_fun].InstructionsInFunction[idx_instr]->operandes[idx_operand].getIntegerValue();
                    function current_fun = FunctionsParsed[0];
                    if (FunctionsParsed.size()>1){

                        function next_fun = FunctionsParsed[1];
                        for (int idx_fun = 1; idx_fun < FunctionsParsed.size(); idx_fun++){
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
    int idx_fun = 0;
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
    int idx_instr = 0;
    bool success = false;
    //qDebug() << "function addr: " << hex << fun.actual_addr;
    for (;idx_instr < fun.InstructionsInFunction.size(); idx_instr++){
        int instr_addr = fun.InstructionsInFunction[idx_instr]->get_addr_instr();
        //qDebug() << "pp addr: " << hex << instr_addr << " OP : " <<fun.InstructionsInFunction[idx_instr]->get_OP() << " addr " << hex << addr;
        if (addr==instr_addr) {
            success = true;

            break;
        }

    }
    //qDebug() << "Adresse pointée : " << hex << addr << " function: " << fun.actual_addr;
    if (!success) {

        qDebug() << "Couldn't find an instruction! ";
    }
    return idx_instr;
}
int Builder::find_operande(int addr, Instruction instr){//NOT USEFUL!
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


