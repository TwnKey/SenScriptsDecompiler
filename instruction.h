#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <QString>
#include <QDebug>
#include "operande.h"

class Instruction{
    public:
        Instruction(int OP);
        virtual void WriteXLSX();
        virtual void WriteDat();
        void AddOperande(operande op);
    private:
        int OPCode;
        QString name;
        QVector<operande> operandes;
};


#endif // INSTRUCTION_H
