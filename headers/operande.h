#ifndef OPERANDE_H
#define OPERANDE_H
#include "headers/utilities.h"
#include "headers/global_vars.h"

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

    std::string to_string() {
        if (Type == "int") {
            return std::to_string(getIntegerValue());

        } else if (Type == "header") {
            uint32_t header_value = ReadIntegerFromByteArray(0, Value);
            uint8_t length_byte = header_value >> 0x18;
            uint32_t group_id = header_value & 0xFFFFFF;
            //for those, they don't affect anything in game afaik
            return std::to_string(group_id) + ", " + std::to_string(length_byte);

        } else if (Type == "float") {
            float value = QByteArrayToFloat(Value);
            return std::to_string(value);

        } else if (Type == "short") {
            uint16_t value = ReadShortFromByteArray(0,Value);
            return std::to_string(value);

        } else if (Type == "byte") {
            uint8_t value = Value[0];
            return std::to_string(value);

        }
        else if (Type == "instruction"){
            /*
             * shouldn't happen because the only instruction with another instruction as operand,
             * OPCode5, will have its own implementation of to_string to help readability
             * I keep it here for the moment to keep that in mind though
            */

        }
        else if (Type == "fill") {
            /*those are skipped and taken care of during the recompilation (all of the instruction using a fill
            will have their own implementation of the recompilation, with hardcoded length*/

        }

        else if ((Type == "string") || (Type == "dialog")) {

            QByteArray value_ = Value;
            if (value_[value_.size()-1] == '\0') value_.chop(1);
            value_.replace(1, "\n");

            QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());
            QString string = "\""+codec->toUnicode(value_)+"\"";


            return string.toStdString();
        } else if (Type == "bytearray") {
            std::string str;
            for (uint8_t x : Value){
                str = str + std::to_string(x) + ",";
            }
            str.pop_back();
            return str;
        } else if (Type == "pointer") {
            //those are skipped and taken care of during the decompilation, by
            //setting up labels and goto (as in EDDecompiler/Python)

        }

    return "";
    }

    void setDestination(int ID_fun, int ID_instr) { Dest = Destination(ID_fun, ID_instr); }
    Destination getDestination() { return Dest; }

  private:
    Destination Dest = Destination(-1, -1);
    int Position = 0;
    std::string Type;
    QByteArray Value;
    int bytes_to_fill = 0;
};
#endif // OPERANDE_H
