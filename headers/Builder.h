#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include <QByteArray>
#include <QVector>
#include "headers/operande.h"
#include "xlsxdocument.h"
class function;
class Instruction;

class Builder{
public:
    Builder();
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int &addr, int row, QXlsx::Document &xls_content)=0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content, int function_type)=0;
    virtual bool CreateHeaderFromDAT(QByteArray &dat_content)=0; //Header will probably be game specific
    virtual QByteArray CreateHeaderBytes()=0;

    virtual void ReadFunctionsDAT(QByteArray &dat_content); //Clearly this one is supposed to be generic (assuming the game format doesn't change)
    virtual void ReadFunctionsXLSX(QXlsx::Document &xls_content);
    bool flag_monsters = false;
    function find_function(int addr);
    int find_instruction(int addr, function fun);
    int find_operande(int addr, Instruction instr);
    std::vector<function> FunctionsToParse;
    std::vector<function> FunctionsParsed;
    QString SceneName;
    int ReadIndividualFunction(function &fun,QByteArray &dat_content);
    bool UpdatePointersXLSX();
    bool UpdatePointersDAT();
    bool Reset();
    int goal;
    bool error = false; //flag to tell if an incorrect OP code was found (to help identify monster functions
    bool Passed_Monster_Functions = false;
};
#endif // INSTRUCTIONBUILDER_H
