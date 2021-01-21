#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "instruction.h"
class OPCode0 : public Instruction
{
    public:
    OPCode0():Instruction(0){

    }
    void WriteXLSX(){
        qDebug() << "WriteXLSX test";
    }

};
#endif // CS3INSTRUCTIONSSET_H
