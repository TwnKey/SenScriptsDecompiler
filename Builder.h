#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include <QByteArray>
#include <QVector>
#include "operande.h"
class function;
class Instruction;

class Builder{
public:
    Builder();
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row)=0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content, int function_type)=0;
    virtual bool CreateHeaderFromDAT(QByteArray &dat_content)=0; //Header will probably be game specific
    virtual bool CreateHeaderFromXLSX()=0;
    virtual void ReadFunctions(QByteArray &dat_content); //Clearly this one is supposed to be generic (assuming the game format doesn't change)
    function find_function(int addr);
    int find_instruction(int addr, function fun);
    int find_operande(int addr, Instruction instr);
    std::vector<function> FunctionsToParse;
    std::vector<function> FunctionsParsed;
    QString SceneName;
    int ReadIndividualFunction(function &fun,QByteArray &dat_content);
    bool UpdatePointers();
    std::vector<operande *> pointers;
};
#endif // INSTRUCTIONBUILDER_H
