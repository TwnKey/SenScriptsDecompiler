#include "instruction.h"

Instruction::Instruction(int addr, int OP, Builder *Maker)
{
            OPCode = OP;
            this->Maker = Maker;
            this->addr_instr = addr;
}
Instruction::Instruction(int addr, QString name, int OP, Builder *Maker){
     this->addr_instr = addr;
     OPCode = OP;
     this->name = name;
     this->Maker = Maker;
}
Instruction::~Instruction(){

}
int Instruction::get_Nb_operandes(){
    return operandes.size();
}
operande Instruction::get_operande(int i){
    return operandes[i];
}

int Instruction::get_addr_instr(){
    return this->addr_instr;
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
    if (op.getType() == "pointer") Maker->pointers.push_back(&*(operandes.end()-1));
}
int Instruction::get_length_in_bytes(){
    int length_in_bytes = 0;
    for (std::vector<operande>::iterator it = operandes.begin(); it!=operandes.end();it++) length_in_bytes = length_in_bytes+it->getLength();
    return length_in_bytes;
}

int Instruction::get_OP(){
    return OPCode;
}
QByteArray Instruction::getBytes(){
    QByteArray bytes;
    for (std::vector<operande>::iterator it = operandes.begin(); it!=operandes.end();it++) {
        QByteArray op_bytes = it->getValue();
        for (int i = 0; i < op_bytes.size(); i++) bytes.push_back(op_bytes[i]);
    }
    return bytes;
}
