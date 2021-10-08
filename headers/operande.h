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
class Operande {

  public:
    Operande() = default;
    Operande(Destination pointer, int position, std::string type, ssd::Buffer value)
      : destination(pointer)
      , position(position)
      , type(std::move(type))
      , value(std::move(value)) {}

    Operande(int position, std::string type, ssd::Buffer value)
      : position(position)
      , type(std::move(type))
      , value(std::move(value)) {}

    Operande(int position, std::string type, std::vector<uint8_t> unsigned_value)
      : position(position)
      , type(std::move(type)) {

        value = ssd::Buffer(begin(unsigned_value), end(unsigned_value));
    }

    [[nodiscard]] bool is_pointer() const { return !((destination.FunctionID == -1) || (destination.InstructionID == -1)); }

    Destination get_destination() { return destination; }
    void set_destination(int function_id, int instruction_id) { destination = Destination(function_id, instruction_id); }

    [[nodiscard]] uint get_addr() const { return position; }

    std::string get_type() { return type; }

    ssd::Buffer get_value() { return value; }
    void set_value(ssd::Buffer v) { value = std::move(v); }
    int get_integer_value() { return ReadIntegerFromByteArray(0, value); }

    [[nodiscard]] int get_bytes_to_fill() const { return bytes_to_fill; }
    void set_bytes_to_fill(int b) { bytes_to_fill = b; }

    int get_length() { return static_cast<int>(std::ssize(value)); }

  private:
    Destination destination = Destination(-1, -1);
    int position = 0;
    std::string type;
    ssd::Buffer value;
    int bytes_to_fill = 0;
};
#endif // OPERANDE_H
