#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <QString>
#include <QDebug>
#include "operande.h"
#include "Builder.h"

class Instruction{
    public:
        Instruction(int addr, int OP, Builder *Maker);
        Instruction(int addr, QString name, int OP,Builder *Maker);
        ~Instruction();
        virtual void WriteXLSX();
        virtual void WriteDat();
        void AddOperande(operande op);
        int get_length_in_bytes();
        int get_Nb_operandes();
        operande get_operande(int i);
        int get_OP();
        int get_addr_instr();
        QByteArray getBytes();
        Builder *Maker;
    private:
        int addr_instr;
        int OPCode;
        QString name;
        std::vector<operande> operandes;
        int length_in_bytes;

};


#endif // INSTRUCTION_H
