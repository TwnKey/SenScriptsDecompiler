#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "headers/Builder.h"
#include "headers/operande.h"
#include <QDebug>
#include <cmath>

#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxchartsheet.h"
#include "xlsxdocument.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

class Instruction {
  public:
    Instruction(int addr, uint OP, Builder* Maker);
    Instruction(int addr, std::string name, uint OP, Builder* Maker);
    Instruction(int& addr, int idx_row, QXlsx::Document& excelScenarioSheet, std::string name, uint OP, Builder* Maker);
    virtual ~Instruction();
    virtual int WriteXLSX(QXlsx::Document& excelScenarioSheet, std::vector<function> funs, int row, int& col);
    virtual void WriteDat();
    void AddOperande(operande op);
    int get_length_in_bytes();
    [[nodiscard]] int get_Nb_operandes() const;
    [[nodiscard]] operande get_operande(int i) const;
    [[nodiscard]] uint get_OP() const;
    [[nodiscard]] int get_addr_instr() const;
    void set_addr_instr(int i);
    virtual std::string to_string();
    std::string get_name();
    QByteArray getBytes();
    Builder* Maker;
    std::vector<operande> operandes;
    bool error = false;

  protected:
    int addr_instr;
    uint OPCode;
    std::string name;
    int length_in_bytes = 0;
};

#endif // INSTRUCTION_H
