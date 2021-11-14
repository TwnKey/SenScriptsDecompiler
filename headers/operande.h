#ifndef OPERANDE_H
#define OPERANDE_H
#include "headers/utilities.h"
#include "headers/global_vars.h"
#include "qxlsx/headers/xlsxdocument.h"

class function;
class Builder;
class Destination {
  public:
    Destination(int FID, int IID)
      : FunctionID(FID)
      , InstructionID(IID) {}
    int FunctionID;
    int InstructionID;
};
class operande {

  public:
    operande() = default;
    operande(Destination pointer, int position, std::string type, QByteArray value)
      : Dest(pointer)
      , Position(position)
      , Type(std::move(type))
      , Value(std::move(value)) {}

    operande(int position, std::string type, QByteArray value)
      : Position(position)
      , Type(std::move(type))
      , Value(std::move(value)) {}

    operande(int position, std::string type, std::vector<uint8_t> value)
      : Position(position)
      , Type(std::move(type)) {

        Value = QByteArray(reinterpret_cast<const char*>(value.data()), static_cast<int>(value.size()));
    }

    //from xlsx sheet:
    operande(std::string type, int& addr, int idx_row, QXlsx::Document& excelScenarioSheet, int &idx_column);

    [[nodiscard]] bool IsPointer() const { return !((Dest.FunctionID == -1) || (Dest.InstructionID == -1)); }

    QByteArray getValue() {
        if (Type == "condition")
        {
            QByteArray result = {};
            for (auto op: operandes){
                result.push_back(op.getValue());
            }
            return result;
        }
        return Value; }




    void setValue(QByteArray v) { Value = std::move(v); }

    void AddOperande(operande op){
        operandes.push_back(op);
    }

    int getIntegerValue() { return ReadIntegerFromByteArray(0, Value); }
    void setBytesToFill(int b) { bytes_to_fill = b; }
    [[nodiscard]] int getBytesToFill() const { return bytes_to_fill; }
    int getLength() {
        int size = Value.size();
        return size;
    }
    [[nodiscard]] uint getAddr() const { return Position; }
    std::string getType() { return Type; }


    virtual bool write_xlsx(QXlsx::Document& excelScenarioSheet, int row, int& col, int &col_cnt,
                    QXlsx::Format FormatInstr,
                    QXlsx::Format FormatType,
                    QXlsx::Format FormatOP,
                    QXlsx::Format FormatStartEnd,
                    std::vector<function> funs,
                    Builder *b

                    );
    std::string to_string();

    void setDestination(int ID_fun, int ID_instr) { Dest = Destination(ID_fun, ID_instr); }
    Destination getDestination() { return Dest; }

  private:
    std::vector<operande> operandes; //only used for "condition" operands
    Destination Dest = Destination(-1, -1);
    int Position = 0;
    std::string Type;
    QByteArray Value;
    int bytes_to_fill = 0;
};




#endif // OPERANDE_H
