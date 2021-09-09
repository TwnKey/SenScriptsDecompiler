#ifndef OPERANDE_H
#define OPERANDE_H
#include "headers/utilities.h"

class Destination {
  public:
    Destination(int FID, int IID, int OID) {
        FunctionID = FID;
        InstructionID = IID;
        OperandeID = OID;
    }
    int FunctionID;
    int InstructionID;
    int OperandeID;
};
class operande {

  public:
    operande() = default;
    operande(Destination pointer, int position, std::string type, QByteArray value) {
        Dest = pointer;
        Position = position;
        Type = std::move(type);
        Value = std::move(value);
    }
    operande(int position, std::string type, QByteArray value) {
        Position = position;
        Type = std::move(type);
        Value = std::move(value);
    }

    operande(int position, std::string type, std::vector<uint8_t> value) {
        Position = position;
        Type = std::move(type);
        Value = QByteArray(reinterpret_cast<const char*>(value.data()), static_cast<int>(value.size()));
    }

    [[nodiscard]] bool IsPointer() const { return !((Dest.FunctionID == -1) || (Dest.InstructionID == -1)); }

    QByteArray getValue() { return Value; }
    void setValue(QByteArray v) { Value = std::move(v); }
    int getIntegerValue() { return ReadIntegerFromByteArray(0, Value); }
    void setBytesToFill(int b) { bytes_to_fill = b; }
    [[nodiscard]] int getBytesToFill() const { return bytes_to_fill; }
    int getLength() {
        int size = Value.size();
        return size;
    }
    [[nodiscard]] uint getAddr() const { return Position; }
    std::string getType() { return Type; }
    void setDestination(int ID_fun, int ID_instr, int ID_operande) { Dest = Destination(ID_fun, ID_instr, ID_operande); }
    Destination getDestination() { return Dest; }

  private:
    Destination Dest = Destination(-1, -1, -1);
    int Position = 0;
    std::string Type;
    QByteArray Value;
    int bytes_to_fill = 0;
};
#endif // OPERANDE_H
