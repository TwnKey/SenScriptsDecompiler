#ifndef OPERANDE_H
#define OPERANDE_H
#include "headers/utilities.h"
#include <QString>
struct Destination {
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
    operande(Destination pointer, int position, QString type, QByteArray value) {
        Dest = pointer;
        Position = position;
        Type = type;
        Value = value;
    }
    operande(int position, QString type, QByteArray value) {
        Position = position;
        Type = type;
        Value = value;
    }
    [[nodiscard]] bool IsPointer() const {
        if ((Dest.FunctionID == -1) || (Dest.InstructionID == -1)) {
            return false;
        } else {
            return true;
        }
    }
    QByteArray getValue() { return Value; }
    void setValue(QByteArray v) { Value = v; }
    int getIntegerValue() { return ReadIntegerFromByteArray(0, Value); }
    void setBytesToFill(int b) { bytes_to_fill = b; }
    [[nodiscard]] int getBytesToFill() const { return bytes_to_fill; }
    int getLength() {
        int size = Value.size();
        if (Type == "string") size++;
        return size;
    }
    [[nodiscard]] uint getAddr() const { return Position; }
    QString getType() { return Type; }
    void setDestination(int ID_fun, int ID_instr, int ID_operande) { Dest = Destination(ID_fun, ID_instr, ID_operande); }
    Destination getDestination() { return Dest; }

  private:
    Destination Dest = Destination(-1, -1, -1);
    int Position;
    QString Type;
    QByteArray Value;
    int bytes_to_fill;
};
#endif // OPERANDE_H
