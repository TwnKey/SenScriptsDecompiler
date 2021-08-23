#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <QString>
#include <QDebug>
#include <cmath>
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
        Instruction(int addr, uint OP, Builder *Maker);
        Instruction(int addr, QString name, uint OP,Builder *Maker);
        Instruction(int &addr, int idx_row, QXlsx::Document  &excelScenarioSheet, QString name, uint OP,Builder *Maker);
        virtual ~Instruction();
        virtual int WriteXLSX(QXlsx::Document &excelScenarioSheet, std::vector<function> funs, int row, int &col);
        virtual void WriteDat();
        void AddOperande(operande op);
        int get_length_in_bytes();
        int get_Nb_operandes();
        operande get_operande(int i);
        uint get_OP();
        int get_addr_instr();
        void set_addr_instr(int i);
        QByteArray getBytes();
        Builder *Maker;
        std::vector<operande> operandes;
        bool error = false;
    protected:
        int addr_instr;
        uint OPCode;
        QString name;
        int length_in_bytes;

};


#endif // INSTRUCTION_H
