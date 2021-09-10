#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include "headers/operande.h"
#include "xlsxdocument.h"
#include <QByteArray>
class function;
class Instruction;

class Builder {
  public:
    Builder();
    virtual ~Builder() = default;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int& addr, QByteArray& dat_content, int function_type) = 0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int& addr, int row, QXlsx::Document& xls_content) = 0;

    virtual bool CreateHeaderFromDAT(QByteArray& dat_content) = 0; // Header will probably be game specific
    virtual QByteArray CreateHeaderBytes() = 0;

    virtual void ReadFunctionsDAT(
      QByteArray& dat_content); // Clearly this one is supposed to be generic (assuming the game format doesn't change)
    virtual void ReadFunctionsXLSX(QXlsx::Document& xls_content);

    int find_function(int addr);
    int find_instruction(int addr, function fun);
    
    std::vector<function> FunctionsToParse;
    std::vector<function> FunctionsParsed;
    std::string SceneName;
    std::vector<int> guess_type_by_name(function& fun, QByteArray& dat_content);

    int attempts_at_reading_function(function& fun, QByteArray& dat_content, std::vector<int> fallback_types);
    int read_individual_function(function& fun, QByteArray& dat_content);
    bool update_pointers_XLSX();
    bool update_pointers_DAT();
    bool reset();
    int goal = 0;
    bool flag_monsters = false;
    bool error = false;
    int idx_current_fun = 0;
    std::string previous_fun_name;
};
#endif // INSTRUCTIONBUILDER_H
