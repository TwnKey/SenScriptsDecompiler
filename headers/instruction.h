#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <QString>
#include <QDebug>
#include "headers/operande.h"
#include "headers/Builder.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

class Instruction{
    public:
        Instruction(int addr, int OP, Builder *Maker);
        Instruction(int addr, QString name, int OP,Builder *Maker);
        Instruction(int &addr, int idx_row, QXlsx::Document  &excelScenarioSheet, QString name, int OP,Builder *Maker);
        ~Instruction();
        virtual void WriteXLSX(QXlsx::Document &excelScenarioSheet, std::vector<function> funs, int row);
        virtual void WriteDat();
        void AddOperande(operande op);
        int get_length_in_bytes();
        int get_Nb_operandes();
        operande get_operande(int i);
        int get_OP();
        int get_addr_instr();
        void set_addr_instr(int i);
        QByteArray getBytes();
        Builder *Maker;
    protected:
        int addr_instr;
        int OPCode;
        QString name;
        std::vector<operande> operandes;
        int length_in_bytes;

};


#endif // INSTRUCTION_H
