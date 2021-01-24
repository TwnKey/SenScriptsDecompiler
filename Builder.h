#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include <QByteArray>
#include <QVector>
class function;
class Instruction;

class Builder{
public:
    Builder();
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row)=0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content)=0;
    virtual bool CreateHeaderFromDAT(QByteArray &dat_content)=0; //Header will probably be game specific
    virtual bool CreateHeaderFromXLSX()=0;
    virtual void ReadFunctions(QByteArray &dat_content); //Clearly this one is supposed to be generic (assuming the game format doesn't change)
    std::vector<function> FunctionsToParse;
    std::vector<function> FunctionsParsed;
    QString SceneName;
    int ReadIndividualFunction(function &fun,QByteArray &dat_content);

};
#endif // INSTRUCTIONBUILDER_H
