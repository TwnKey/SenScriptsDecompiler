#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "instruction.h"
#include "instructionbuilder.h"
class OPCode0 : public Instruction
{
    public:
    OPCode0():Instruction(0){

    }
    void WriteXLSX(){
        qDebug() << "WriteXLSX test";
    }

};
class CS3InstructionBuilder : public InstructionBuilder
{
    public:
    CS3InstructionBuilder():InstructionBuilder(){

    }
    std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row){
        return std::make_shared<OPCode0>();
    }
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int addr){
        return std::make_shared<OPCode0>();
    }

};


#endif // CS3INSTRUCTIONSSET_H
