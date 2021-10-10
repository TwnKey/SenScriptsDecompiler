#ifndef OPERANDE_H
#define OPERANDE_H
#include "headers/utilities.h"

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
    operande(Destination pointer, int position, std::string type, ssd::Buffer value)
      : Dest(pointer)
      , Position(position)
      , Type(std::move(type))
      , Value(std::move(value)) {}

    operande(int position, std::string type, ssd::Buffer value)
      : Position(position)
      , Type(std::move(type))
      , Value(std::move(value)) {}

    operande(int position, std::string type, std::vector<uint8_t> value)
      : Position(position)
      , Type(std::move(type)) {

        Value = ssd::Buffer(begin(value), end(value));
    }

    [[nodiscard]] bool IsPointer() const { return !((Dest.FunctionID == -1) || (Dest.InstructionID == -1)); }

    ssd::Buffer getValue() { return Value; }
    void setValue(ssd::Buffer v) { Value = std::move(v); }
    int getIntegerValue() { return ReadIntegerFromByteArray(0, Value); }
    void setBytesToFill(int b) { bytes_to_fill = b; }
    [[nodiscard]] int getBytesToFill() const { return bytes_to_fill; }
    int getLength() { return static_cast<int>(std::ssize(Value)); }
    [[nodiscard]] uint getAddr() const { return Position; }
    std::string getType() { return Type; }
    void setDestination(int ID_fun, int ID_instr) { Dest = Destination(ID_fun, ID_instr); }
    Destination getDestination() { return Dest; }

  private:
    Destination Dest = Destination(-1, -1);
    int Position = 0;
    std::string Type;
    ssd::Buffer Value;
    int bytes_to_fill = 0;
};
#endif // OPERANDE_H
