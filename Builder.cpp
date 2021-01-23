#include "Builder.h"
#include "functions.h"
Builder::Builder()
{

}
void Builder::ReadFunctions(QByteArray &dat_content){
    //From what I've seen, some functions in the file don't use OP Codes and it's not very explicit
    //The game first calls the Init and PreInit functions of the script file if they exist.
    //One of them should call essential functions that can pinpoint which functions are using OP Codes.
    //An example is the "PutMonster" function I encountered. It is called by the Init function,
    //it is using OP Codes, and its instructions can identify the functions we need to skip that are not using OP Codes.
    //Each time we completely parsed a function, we should make sure we never parse it twice.


    std::vector<function>::iterator InitIt = find_function_by_name(FunctionsToParse, "Init");
    if (InitIt != FunctionsToParse.end()){
        std::iter_swap(InitIt, FunctionsToParse.begin());
        std::vector<function>::iterator PreInitIt = find_function_by_name(FunctionsToParse, "PreInit");
        if (PreInitIt != FunctionsToParse.end()){
            std::iter_swap(PreInitIt, InitIt);
        }
    }
    //here the PreInit should be the first function to be parsed and the Init should follow.
    int nb_functions = FunctionsToParse.size();

    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++){
        int last_pos = ReadIndividualFunction(*it,dat_content);
        //once a function is read, it should be removed from the FunctionsToParse vector and added to the FunctionsParsed
        FunctionsParsed.push_back(*it);
        FunctionsToParse.erase(it);

    }
    std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

}
int Builder::ReadIndividualFunction(function &fun,QByteArray &dat_content){
    int current_position = fun.actual_addr;
    qDebug() << "Trying to decompile function named " << fun.name << " located at " << hex << fun.actual_addr;

    std::shared_ptr<Instruction> instr;
    do{
        instr = CreateInstructionFromDAT(current_position, dat_content);
        fun.AddInstruction(instr);
        current_position = current_position + instr->get_length_in_bytes();
    }
    while(instr->get_OP()!=1);
    return current_position;
}
