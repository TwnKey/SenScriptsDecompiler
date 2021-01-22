#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include "instruction.h"
#include "translationfile.h"
class Builder{
public:
    Builder(){

    }
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row)=0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int addr)=0;
    virtual bool CreateHeaderFromDAT(TranslationFile &TF, QByteArray &dat_content)=0;
    virtual bool CreateHeaderFromXLSX(TranslationFile &TF)=0;
};
#endif // INSTRUCTIONBUILDER_H
