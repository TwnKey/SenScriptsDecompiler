#include "headers/instruction.h"
#include "headers/functions.h"
#include "headers/global_vars.h"
#include "headers/utilities.h"
#include <stack>
#include <string>

#include "xlsxdocument.h"
#include <QTextCodec>

Instruction::Instruction(int addr, uint32_t OP, Builder* Maker)
  : maker(Maker)
  , addr_instr(addr)
  , opcode(OP) {}

Instruction::Instruction(int addr, std::string name, uint32_t OP, Builder* Maker)
  : maker(Maker)
  , addr_instr(addr)
  , opcode(OP)
  , name(std::move(name)) {}

Instruction::~Instruction() = default;

Instruction::Instruction(int& addr, int idx_row, QXlsx::Document& excelScenarioSheet, std::string name, uint32_t OP, Builder* Maker)
  : maker(Maker)
  , addr_instr(addr)
  , opcode(OP)
  , name(std::move(name)) {

    std::stack<std::pair<size_t, uint8_t>> stack_of_headers;

    if (opcode <= 0xFF) addr++;

    int idx_column = 3;
    std::string type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    while (!type.empty()) {

        ssd::Buffer Value;
        Operande operande;
        if (type == "int") {
            int op = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            Value = GetBytesFromInt(op);
            operande = Operande(addr, "int", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "Group ID") {
            stack_of_headers.push(std::make_pair(0, addr - 1)); // the count includes OPCode thus -1

            int operande_grp = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            idx_column++;
            uint8_t operande_length = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            // If the byte for length is 0xFF we don't calculate it
            if (operande_length != 0xFF) stack_of_headers.top().first = get_nb_operandes();
            Value = GetBytesFromInt(operande_grp + (operande_length << 0x18));
            operande = Operande(addr, "header", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();

        } else if (type == "float") {

            float op = excelScenarioSheet.read(idx_row + 1, idx_column).toFloat();

            Value = GetBytesFromFloat(op);
            operande = Operande(addr, "float", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "short") {
            int16_t op = static_cast<int16_t>(excelScenarioSheet.read(idx_row + 1, idx_column).toInt());
            Value = GetBytesFromShort(op);
            operande = Operande(addr, "short", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();

        } else if ((type == "byte") || (type == "OP Code")) {
            OP = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            if (OP <= 0xFF) { // actually the opposite never happens.
                auto op = static_cast<char>(((OP)&0x000000FF));
                Value.push_back(op);
                operande = Operande(addr, "byte", Value);
                this->add_operande(operande);
                addr = addr + operande.get_length();
            }
        } else if (type == "End") {
            if (!stack_of_headers.empty()) {
                if (stack_of_headers.top().first != 0) {

                    uint32_t new_header_value =
                      ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                      (this->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
                    ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
                    this->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
                    stack_of_headers.pop();
                }
            }

        } else if (type == "fill") {
            QString operande_prev = excelScenarioSheet.read(idx_row + 1, idx_column - 1).toString();
            QString op = excelScenarioSheet.read(idx_row + 1, idx_column).toString();
            QString str_max_length = op.mid(1, op.indexOf('-') - 1);

            for (int id = 0; id < str_max_length.toInt() - operande_prev.toUtf8().size() - 1; id++) {
                Value.push_back('\0');
            }

            operande = Operande(addr, "fill", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();

        }

        else if ((type == "string") || (type == "dialog")) {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());
            QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(OutputDatFileEncoding).toUtf8());
            auto q_byte_array = codec->fromUnicode(op);
            Value = ssd::Buffer(std::begin(q_byte_array), std::end(q_byte_array));
            if (type == "string") Value.push_back('\0');
            Value.replace('\n', 1);

            operande = Operande(addr, type, Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "bytearray") {

            while (type == "bytearray") {
                auto op = static_cast<char>(((excelScenarioSheet.read(idx_row + 1, idx_column).toInt()) & 0x000000FF));
                Value.push_back(op);
                idx_column++;
                type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
            }
            operande = Operande(addr, "bytearray", Value);
            idx_column--;
            this->add_operande(operande);
            addr = addr + operande.get_length();
        } else if (type == "pointer") {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());

            QString RowPointedStr = op.right(op.size() - 2);

            int actual_row = RowPointedStr.toInt();

            Value = GetBytesFromInt(actual_row);

            auto operande = Operande(addr, "pointer", Value);
            this->add_operande(operande);
            addr = addr + operande.get_length();
        }

        idx_column++;
        type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    }
    if (!stack_of_headers.empty()) {
        if (stack_of_headers.top().first != 0) {
            uint32_t new_header_value = ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                                        (this->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
            ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
            this->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
            stack_of_headers.pop();
        }
    }
}
int Instruction::get_nb_operandes() const { return static_cast<int>(operandes.size()); }
Operande Instruction::get_operande(int i) const { return operandes[i]; }

int Instruction::get_addr_instr() const { return this->addr_instr; }

void Instruction::set_addr_instr(int i) { addr_instr = i; }
/*This version of add_operande is supposed to check if a string contain illegal xml characters,
but I didn't finish it yet.
If there is any illegal xml character, every string in the sheet disappears and the file can't be decompiled,
this is a problem for some broken files that we would want to restore (example is ply000 from CS3)*/
void Instruction::add_operande(Operande op) {

    ssd::Buffer value = op.get_value();

    if (op.get_type() == "string") {
        QTextCodec::ConverterState state;

        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());
        const QString text = codec->toUnicode(reinterpret_cast<char*>(value.data()), static_cast<int>(std::ssize(value)), &state);

        if ((state.invalidChars > 0) || text.contains('\x0B') || text.contains('\x06') || text.contains('\x07') || text.contains('\x08') ||
            text.contains('\x05') || text.contains('\x04') || text.contains('\x03') || text.contains('\x02')) {
            op.set_value(ssd::Buffer(0));

            // operandes.push_back(op);
        } else {
            operandes.push_back(op);
        }
    } else {
        operandes.push_back(op);
    }
}

int Instruction::get_length_in_bytes() { return static_cast<int>(std::size(get_bytes())); }

uint32_t Instruction::get_opcode() const { return opcode; }
ssd::Buffer Instruction::get_bytes() {
    ssd::Buffer bytes;
    if (opcode <= 0xFF) bytes.push_back((char)opcode);
    for (auto& it : operandes) {

        ssd::Buffer op_bytes = it.get_value();
        for (auto&& op_byte : op_bytes) {
            bytes.push_back(op_byte);
        }
    }
    return bytes;
}
