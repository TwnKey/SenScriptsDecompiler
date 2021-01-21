#include "instruction.h"

Instruction::Instruction(int OP)
{
            OPCode = OP;
}
void Instruction::WriteDat()
{
            qDebug() << "WriteDat test";
}
void Instruction::WriteXLSX()
{
            qDebug() << "WriteXLSX test";
}
void Instruction::AddOperande(operande op){
    operandes.push_back(op);
}
