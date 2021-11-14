#include "headers/instruction.h"
#include "headers/functions.h"
#include "headers/global_vars.h"
#include "headers/utilities.h"
#include <stack>
#include <string>

Instruction::Instruction(int addr, uint OP, Builder* Maker)
  : Maker(Maker)
  , addr_instr(addr)
  , OPCode(OP) {}

Instruction::Instruction(int addr, std::string name, uint OP, Builder* Maker)
  : Maker(Maker)
  , addr_instr(addr)
  , OPCode(OP)
  , name(std::move(name)) {}

Instruction::~Instruction() = default;
Instruction::Instruction(int& addr, int idx_row, QXlsx::Document& excelScenarioSheet, std::string name, uint OP, Builder* Maker)
  : Maker(Maker)
  , addr_instr(addr)
  , OPCode(OP)
  , name(std::move(name)) {

    std::stack<std::pair<size_t, uint8_t>> stack_of_headers;

    if (OPCode <= 0xFF) addr++;

    int idx_column = 3;
    std::string type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    while (!type.empty()) {

        QByteArray Value;
        operande op;
        if (type == "Group ID") {
            stack_of_headers.push(std::make_pair(0, addr - 1)); // the count includes OPCode thus -1

            int OperandeGrp = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            idx_column++;
            uint8_t OperandeLength = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            // If the byte for length is 0xFF we don't calculate it
            if (OperandeLength != 0xFF) stack_of_headers.top().first = get_Nb_operandes();
            Value = GetBytesFromInt(OperandeGrp + (OperandeLength << 0x18));
            op = operande(addr, "header", Value);
            this->AddOperande(op);
            addr = addr + op.getLength();

        }
        else if (type == "Start Condition") {

            operande condition = operande(addr, "condition", QByteArray());
            while (type != "End Condition") {
                op = operande(type, addr, idx_row,excelScenarioSheet, idx_column);
                condition.AddOperande(op);
                idx_column++;
                type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
            }
            this->AddOperande(condition);
        }
        else if (type == "End Instruction") {
            if (!stack_of_headers.empty()) {
                if (stack_of_headers.top().first != 0) {

                    uint32_t new_header_value =
                      ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                      (this->get_operande(static_cast<int>(stack_of_headers.top().first)).getIntegerValue() & 0xFFFFFF);
                    QByteArray header_bytes = GetBytesFromInt(new_header_value);
                    this->get_operande(static_cast<int>(stack_of_headers.top().first)).setValue(header_bytes);
                    stack_of_headers.pop();
                }
            }

        }
        else { //classic types
            op = operande(type, addr, idx_row,excelScenarioSheet, idx_column);
            this->AddOperande(op);

        }
        idx_column++;
        type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    }
    if (!stack_of_headers.empty()) {
        if (stack_of_headers.top().first != 0) {
            uint32_t new_header_value = ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                                        (this->get_operande(static_cast<int>(stack_of_headers.top().first)).getIntegerValue() & 0xFFFFFF);
            QByteArray header_bytes = GetBytesFromInt(new_header_value);
            this->get_operande(static_cast<int>(stack_of_headers.top().first)).setValue(header_bytes);
            stack_of_headers.pop();
        }
    }
}
int Instruction::get_Nb_operandes() const { return static_cast<int>(operandes.size()); }
operande Instruction::get_operande(int i) const { return operandes[i]; }

int Instruction::get_addr_instr() const { return this->addr_instr; }
void Instruction::WriteDat() {}
int Instruction::WriteXLSX(QXlsx::Document& excelScenarioSheet, std::vector<function> funs, int row, int& col) {
    QXlsx::Format FormatInstr;
    QXlsx::Format FormatType;
    QXlsx::Format FormatOP;
    QXlsx::Format FormatStartEnd;
    auto ColorBg = QColor(qRgb(255, 230, 210));
    FormatType.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    FormatType.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    FormatType.setTextWrap(true);
    FormatInstr.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    FormatInstr.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    FormatType.setBottomBorderStyle(QXlsx::Format::BorderThin);
    FormatType.setLeftBorderStyle(QXlsx::Format::BorderThin);
    FormatType.setRightBorderStyle(QXlsx::Format::BorderThin);
    FormatType.setTopBorderStyle(QXlsx::Format::BorderThin);
    FormatType.setPatternBackgroundColor(ColorBg);

    FormatInstr.setBottomBorderStyle(QXlsx::Format::BorderThin);
    FormatInstr.setLeftBorderStyle(QXlsx::Format::BorderThin);
    FormatInstr.setRightBorderStyle(QXlsx::Format::BorderThin);
    FormatInstr.setTopBorderStyle(QXlsx::Format::BorderThin);

    FormatOP.setBottomBorderStyle(QXlsx::Format::BorderThin);
    FormatOP.setLeftBorderStyle(QXlsx::Format::BorderThin);
    FormatOP.setRightBorderStyle(QXlsx::Format::BorderThin);
    FormatOP.setTopBorderStyle(QXlsx::Format::BorderThin);

    auto colorRed = QColor(qRgb(255, 0, 0));

    FormatStartEnd.setPatternBackgroundColor(colorRed);
    FormatStartEnd.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    FormatStartEnd.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    FormatStartEnd.setTextWrap(true);
    FormatType.setFontBold(true);
    QColor color;

    color = QColor::fromHsl((int)OPCode, 255, 185, 255);
    FormatOP.setPatternBackgroundColor(color);

    if (error) {
        excelScenarioSheet.write(row, col + 1, "ERROR!", FormatType);
    }
    excelScenarioSheet.write(row, col + 2, "OP Code", FormatType);
    excelScenarioSheet.write(row + 1, col + 2, OPCode, FormatOP);
    int col_cnt = 0;
    for (auto& operande : operandes) {

        operande.write_xlsx(excelScenarioSheet, row, col, col_cnt,
                            FormatInstr,
                            FormatType,
                            FormatOP,
                            FormatStartEnd,
                            funs,
                            this->Maker
                            );
    }

    return col_cnt;
}
void Instruction::set_addr_instr(int i) { addr_instr = i; }
/*This version of AddOperande is supposed to check if a string contain illegal xml characters,
but I didn't finish it yet.
If there is any illegal xml character, every string in the sheet disappears and the file can't be decompiled,
this is a problem for some broken files that we would want to restore (example is ply000 from CS3)*/
void Instruction::AddOperande(operande op) {
    QByteArray value = op.getValue();

    if (op.getType() == "string") {
        QTextCodec::ConverterState state;

        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());
        const QString text = codec->toUnicode(value, value.size(), &state);

        if ((state.invalidChars > 0) || text.contains('\x0B') || text.contains('\x06') || text.contains('\x07') || text.contains('\x08') ||
            text.contains('\x05') || text.contains('\x04') || text.contains('\x03') || text.contains('\x02')) {
            op.setValue(QByteArray(nullptr));

            // operandes.push_back(op);
        } else {
            operandes.push_back(op);
        }
    } else {
        operandes.push_back(op);
    }
}

int Instruction::get_length_in_bytes() { return getBytes().size(); }

uint Instruction::get_OP() const { return OPCode; }
QByteArray Instruction::getBytes() {
    QByteArray bytes;
    if (OPCode <= 0xFF) bytes.push_back((char)OPCode);
    for (auto& it : operandes) {

        QByteArray op_bytes = it.getValue();
        for (auto&& op_byte : op_bytes) {
            bytes.push_back(op_byte);
        }
    }
    return bytes;
}

std::string Instruction::to_string(){
    std::string str = this->name + "(";

    for (auto op : this->operandes)
        str = str + op.to_string(this->Maker) + ",";

    if (!this->operandes.empty())
        str.pop_back();

    str = str + ")";
    return str;
}
std::string Instruction::get_name(){
    return this->name;
}

