#include "headers/instruction.h"
#include "headers/global_vars.h"
#include <string>

#include <QTextCodec>

Instruction::Instruction(int& addr, std::string name, uint32_t OP, Builder* Maker)
  : maker(Maker)
  , addr_instr(addr)
  , opcode(OP)
  , name(std::move(name)) {}

Instruction::~Instruction() = default;

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
