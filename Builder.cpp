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

    qDebug() << "we're going to swap the order of the functions";
    qDebug() << "current_order:";
    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++) qDebug() << it->name;
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
    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++) qDebug() << it->name;
    //here the PreInit should be the first function to be parsed and the Init should follow.

    for (std::vector<function>::iterator it = FunctionsToParse.begin(); it != FunctionsToParse.end(); it++){
        qDebug() << "Trying to decompile function named " << it->name << " located at " << hex << it->actual_addr;
        ReadIndividualFunction(*it,dat_content);
        //once a function is read, it should be removed from the FunctionsToParse vector and added to the FunctionsParsed
        FunctionsParsed.push_back(*it);
        //FunctionsToParse.erase(it);

    }
    std::sort(FunctionsParsed.begin(), FunctionsParsed.end());

}
int Builder::ReadIndividualFunction(function &fun,QByteArray &dat_content){
    int current_position = fun.actual_addr;


    std::shared_ptr<Instruction> instr;
    do{
        qDebug() << "POS: " << hex << current_position;
        instr = CreateInstructionFromDAT(current_position, dat_content);
        qDebug() << "OP CODE: " << hex << instr->get_OP();
        fun.AddInstruction(instr);

    }
    while(instr->get_OP()!=1);
    return current_position;
}
