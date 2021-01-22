#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include "instruction.h"
class InstructionBuilder{
public:
    InstructionBuilder(){

    }
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row)=0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int addr)=0;
};
#endif // INSTRUCTIONBUILDER_H
