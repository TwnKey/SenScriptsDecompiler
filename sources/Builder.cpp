#include "headers/Builder.h"
#include "headers/functions.h"
Builder::Builder()
{

}
void Builder::ReadFunctionsXLSX(QXlsx::Document &doc){
    int first_row = 4;
    int last_row = doc.dimension().lastRow();
    int addr = 0;
    int ID_fun = 0;
    function current_fun;
    for (int idx_row = first_row; idx_row < last_row; idx_row++){
        QString content_first_cell = doc.read(idx_row, 1).toString();
        if (content_first_cell == "FUNCTION"){ //We start a new function

            if (ID_fun!=0) FunctionsParsed.push_back(current_fun);
            current_fun.name  = doc.read(idx_row, 2).toString();
            current_fun.ID = ID_fun;
            current_fun.actual_addr = addr;
            current_fun.declr_position = 0;
            current_fun.XLSX_row_index = idx_row;

            ID_fun++;
        }
        else{
                std::shared_ptr<Instruction> instr = CreateInstructionFromXLSX(addr, idx_row, doc);
                current_fun.AddInstruction(instr);
                idx_row++;
                current_fun.length_in_bytes = current_fun.length_in_bytes + instr->get_length_in_bytes();
        }
        FunctionsParsed.push_back(current_fun);
    }
    UpdatePointersXLSX();

}

void Builder::ReadFunctionsDAT(QByteArray &dat_content){
    //From what I've seen, some functions in the file don't use OP Codes and it's not very explicit
    //The game first calls the Init and PreInit functions of the script file if they exist.
    //One of them should call essential functions that can pinpoint which functions are using OP Codes.
    //An example is the "PutMonster" function I encountered. It is called by the Init function,
    //it is using OP Codes, and its instructions can identify the functions we need to skip that are not using OP Codes.
    //Each time we completely parsed a function, we should make sure we never parse it twice.

    qDebug() << "we're going to swap the order of the functions";
    qDebug() << "current_order:";
    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++) qDebug() << it->name << " id: " << hex << it->ID;
    std::vector<function>::iterator InitIt = find_function_by_name(FunctionsToParse, "Init");
    qDebug() << "Init found!";
    if (InitIt != FunctionsToParse.end()){
        std::iter_swap(InitIt, FunctionsToParse.begin());
        qDebug() << "Swapped Init!";
        std::vector<function>::iterator PreInitIt = find_function_by_name(FunctionsToParse, "PreInit");
        if (PreInitIt != FunctionsToParse.end()){
            qDebug() << "PreInit found!";
            std::iter_swap(PreInitIt, FunctionsToParse.begin());
            std::iter_swap(PreInitIt, ++FunctionsToParse.begin());
        }
    }
    int actual_index = 0;
    for (int i = 0; i<FunctionsToParse.size(); i++){

        if (FunctionsToParse[actual_index].name == ""){
            function blank = FunctionsToParse[actual_index];
            FunctionsToParse.erase(FunctionsToParse.begin()+actual_index);

            FunctionsToParse.push_back(blank);

        }
        else actual_index++;
    }

    //here the PreInit should be the first function to be parsed and the Init should follow.

    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++){
        if (!std::count(FunctionsParsed.begin(), FunctionsParsed.end(), *it)){
            qDebug() << "Trying to decompile function named " << it->name << " located at " << hex << it->actual_addr;

            ReadIndividualFunction(*it,dat_content);
        //once a function is read, it should be removed from the FunctionsToParse vector and added to the FunctionsParsed
            FunctionsParsed.push_back(*it);
        }else {qDebug() << "Huh?";}


    }
    std::sort(FunctionsParsed.begin(), FunctionsParsed.end());
    UpdatePointersDAT();
    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++) qDebug() << it->name << " addr: "<< hex << it->actual_addr;


}
int Builder::ReadIndividualFunction(function &fun,QByteArray &dat_content){
    int current_position = fun.actual_addr;

    int function_type = 0;
    if (fun.name == "") function_type = 1;
    else if (fun.name.startsWith("_")) function_type = 2;

    std::shared_ptr<Instruction> instr;

    while((unsigned char)dat_content[current_position] != 0x1){

        instr = CreateInstructionFromDAT(current_position, dat_content, function_type);

        fun.AddInstruction(instr);
    }


    instr = CreateInstructionFromDAT(current_position, dat_content, 0);

    fun.AddInstruction(instr);
    return current_position;
}
bool Builder::UpdatePointersDAT(){
    int nb_pointers = pointers.size();
    for (int idx_ptr = 0; idx_ptr < nb_pointers; idx_ptr++){
        int addr_ptr = pointers[idx_ptr]->getIntegerValue();
        function fun = find_function(addr_ptr);
        int id_instr = find_instruction(addr_ptr,fun);
        int id_op = find_operande(addr_ptr,*fun.InstructionsInFunction[id_instr]);
        pointers[idx_ptr]->setDestination(fun.ID, id_instr, id_op);
    }
    return true;

}
bool Builder::UpdatePointersXLSX(){
    //really not satisfied with that...
    int nb_pointers = pointers.size();
    for (int idx_ptr = 0; idx_ptr < nb_pointers; idx_ptr++){
        int idx_row_ptr = pointers[idx_ptr]->getIntegerValue();
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
        pointers[idx_ptr]->setValue(GetBytesFromInt(addr_pointed));

    }
    return true;

}
function Builder::find_function(int addr){
    function result = FunctionsParsed[0];
    bool success = false;
    for (int idx_fun = 0; idx_fun < FunctionsParsed.size(); idx_fun++){
        int fun_addr = FunctionsParsed[idx_fun].actual_addr;
        if (addr<fun_addr) {

            result = FunctionsParsed[idx_fun-1];
            success = true;
            break;
        }

    }
    if (!success) qDebug() << "Couldn't find a function! ";
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
    if (!success) {
        qDebug() << "addr ptr: " << hex << addr;
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


