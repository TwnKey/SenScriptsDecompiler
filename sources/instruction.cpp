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

        ssd::Buffer Value;
        Operande operande;
        if (type == "int") {
            int op = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            Value = GetBytesFromInt(op);
            operande = Operande(addr, "int", Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();
        } else if (type == "Group ID") {
            stack_of_headers.push(std::make_pair(0, addr - 1)); // the count includes OPCode thus -1

            int operande_grp = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            idx_column++;
            uint8_t operande_length = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();
            // If the byte for length is 0xFF we don't calculate it
            if (operande_length != 0xFF) stack_of_headers.top().first = get_Nb_operandes();
            Value = GetBytesFromInt(operande_grp + (operande_length << 0x18));
            operande = Operande(addr, "header", Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();

        } else if (type == "float") {

            float op = excelScenarioSheet.read(idx_row + 1, idx_column).toFloat();

            Value = GetBytesFromFloat(op);
            operande = Operande(addr, "float", Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();
        } else if (type == "short") {
            int16_t op = static_cast<int16_t>(excelScenarioSheet.read(idx_row + 1, idx_column).toInt());
            Value = GetBytesFromShort(op);
            operande = Operande(addr, "short", Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();

        } else if ((type == "byte") || (type == "OP Code")) {
            OP = excelScenarioSheet.read(idx_row + 1, idx_column).toInt();

            if (OP <= 0xFF) { // actually the opposite never happens.
                auto op = static_cast<char>(((OP)&0x000000FF));
                Value.push_back(op);
                operande = Operande(addr, "byte", Value);
                this->AddOperande(operande);
                addr = addr + operande.getLength();
            }
        } else if (type == "End") {
            if (!stack_of_headers.empty()) {
                if (stack_of_headers.top().first != 0) {

                    uint32_t new_header_value =
                      ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                      (this->get_operande(static_cast<int>(stack_of_headers.top().first)).getIntegerValue() & 0xFFFFFF);
                    ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
                    this->get_operande(static_cast<int>(stack_of_headers.top().first)).setValue(header_bytes);
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
            this->AddOperande(operande);
            addr = addr + operande.getLength();

        }

        else if ((type == "string") || (type == "dialog")) {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());
            QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(OutputDatFileEncoding).toUtf8());
            auto q_byte_array = codec->fromUnicode(op);
            Value = ssd::Buffer(std::begin(q_byte_array), std::end(q_byte_array));
            if (type == "string") Value.push_back('\0');
            Value.replace('\n', 1);

            operande = Operande(addr, type, Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();
        } else if (type == "bytearray") {

            while (type == "bytearray") {
                auto op = static_cast<char>(((excelScenarioSheet.read(idx_row + 1, idx_column).toInt()) & 0x000000FF));
                Value.push_back(op);
                idx_column++;
                type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
            }
            operande = Operande(addr, "bytearray", Value);
            idx_column--;
            this->AddOperande(operande);
            addr = addr + operande.getLength();
        } else if (type == "pointer") {
            QString op = (excelScenarioSheet.read(idx_row + 1, idx_column).toString());

            QString RowPointedStr = op.right(op.size() - 2);

            int actual_row = RowPointedStr.toInt();

            Value = GetBytesFromInt(actual_row);

            auto operande = Operande(addr, "pointer", Value);
            this->AddOperande(operande);
            addr = addr + operande.getLength();
        }

        idx_column++;
        type = excelScenarioSheet.read(idx_row, idx_column).toString().toStdString();
    }
    if (!stack_of_headers.empty()) {
        if (stack_of_headers.top().first != 0) {
            uint32_t new_header_value = ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                                        (this->get_operande(static_cast<int>(stack_of_headers.top().first)).getIntegerValue() & 0xFFFFFF);
            ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
            this->get_operande(static_cast<int>(stack_of_headers.top().first)).setValue(header_bytes);
            stack_of_headers.pop();
        }
    }
}
int Instruction::get_Nb_operandes() const { return static_cast<int>(operandes.size()); }
Operande Instruction::get_operande(int i) const { return operandes[i]; }

int Instruction::get_addr_instr() const { return this->addr_instr; }
void Instruction::WriteDat() {}
int Instruction::WriteXLSX(QXlsx::Document& excelScenarioSheet, std::vector<Function> funs, int row, int& col) {
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

        QString type = QString::fromStdString(operande.getType());
        ssd::Buffer Value = operande.getValue();

        if (type == "int") {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, ReadIntegerFromByteArray(0, Value), FormatInstr);
            col_cnt++;

        } else if (type == "header") {
            uint32_t header_value = ReadIntegerFromByteArray(0, Value);
            uint8_t length_byte = header_value >> 0x18;
            uint32_t group_id = header_value & 0xFFFFFF;
            excelScenarioSheet.write(row, col + 3 + col_cnt, "Group ID", FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, group_id, FormatInstr);
            col_cnt++;
            excelScenarioSheet.write(row, col + 3 + col_cnt, "Length", FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, length_byte, FormatInstr);
            col_cnt++;
        } else if (type == "float") {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, QByteArrayToFloat(Value), FormatInstr);
            col_cnt++;
        } else if (type == "short") {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (uint16_t)ReadShortFromByteArray(0, Value), FormatInstr);
            col_cnt++;
        } else if (type == "byte") {

            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (unsigned char)Value[0], FormatInstr);
            col_cnt++;
        } else if (type == "fill") {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            excelScenarioSheet.write(row + 1,
                                     col + 3 + col_cnt,
                                     "=" + QString::number(operande.getBytesToFill()) + "-LENB(INDIRECT(ADDRESS(" +
                                       QString::number(row + 1) + "," + QString::number(3 + col_cnt - 1) + ")))",
                                     FormatInstr);
            col_cnt++;
        } else if (type == "bytearray") {

            for (auto&& idx_byte : Value) {
                excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
                excelScenarioSheet.write(row + 1, col + 3 + col_cnt, (unsigned char)idx_byte, FormatInstr);
                col_cnt++;
            }

        } else if (type == "instruction") {
            int addr = 0;
			excelScenarioSheet.write(row, col + 3 + col_cnt, "Start", FormatStartEnd);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
            col_cnt++;

            std::shared_ptr<Instruction> instr = Maker->CreateInstructionFromDAT(
              addr, Value, 0); // function type is 0 here because sub05 is only called by OP Code instructions.
            int column_instr = col + 3 + col_cnt - 2;
            int cnt = instr->WriteXLSX(excelScenarioSheet, funs, row, column_instr);
            col = col + cnt + 1;

            excelScenarioSheet.write(row, col + 3 + col_cnt, "End", FormatStartEnd);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
            col_cnt++;

        } else if ((type == "string") || (type == "dialog")) {

            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            ssd::Buffer value_ = Value;

            value_.replace(1, '\n');

            QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());

            auto q_byte_array = QByteArray(reinterpret_cast<const char*>(value_.data()), static_cast<int>(std::ssize(value_)));
            QString string = codec->toUnicode(q_byte_array);

            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, string, FormatInstr);
            col_cnt++;
        } else if (type == "pointer") {
            excelScenarioSheet.write(row, col + 3 + col_cnt, type, FormatType);
            int ID = funs[0].id;
            size_t nb_row = 3;
            int idx_fun = 0;
            while (ID != operande.getDestination().FunctionID) {

                nb_row = nb_row + 1; // row with function name
                nb_row = nb_row + 2 * funs[idx_fun].instructions.size();
                idx_fun++;
                ID = funs[idx_fun].id;
            }
            nb_row = nb_row + 1; // row with function name
            nb_row = nb_row + static_cast<uint64_t>(2 * (operande.getDestination().InstructionID + 1));
            QString ptrExcel = "=A" + QString::number((nb_row));

            QXlsx::Format format;
            format.setBottomBorderStyle(QXlsx::Format::BorderThin);
            format.setLeftBorderStyle(QXlsx::Format::BorderThin);
            format.setRightBorderStyle(QXlsx::Format::BorderThin);
            format.setTopBorderStyle(QXlsx::Format::BorderThin);
            format.setFontBold(true);
            auto FontColor = QColor(qRgb(255, 0, 0));
            format.setFontColor(FontColor);
            excelScenarioSheet.write(row + 1, col + 3 + col_cnt, ptrExcel, format);
            col_cnt++;
        }
    }

    return col_cnt;
}
void Instruction::set_addr_instr(int i) { addr_instr = i; }
/*This version of AddOperande is supposed to check if a string contain illegal xml characters,
but I didn't finish it yet.
If there is any illegal xml character, every string in the sheet disappears and the file can't be decompiled,
this is a problem for some broken files that we would want to restore (example is ply000 from CS3)*/
void Instruction::AddOperande(Operande op) {

    ssd::Buffer value = op.getValue();

    if (op.getType() == "string") {
        QTextCodec::ConverterState state;

        QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(InputDatFileEncoding).toUtf8());
        const QString text = codec->toUnicode(reinterpret_cast<char*>(value.data()), static_cast<int>(std::ssize(value)), &state);

        if ((state.invalidChars > 0) || text.contains('\x0B') || text.contains('\x06') || text.contains('\x07') || text.contains('\x08') ||
            text.contains('\x05') || text.contains('\x04') || text.contains('\x03') || text.contains('\x02')) {
            op.setValue(ssd::Buffer(0));

            // operandes.push_back(op);
        } else {
            operandes.push_back(op);
        }
    } else {
        operandes.push_back(op);
    }
}

int Instruction::get_length_in_bytes() { return static_cast<int>(std::size(getBytes())); }

uint Instruction::get_OP() const { return OPCode; }
ssd::Buffer Instruction::getBytes() {
    ssd::Buffer bytes;
    if (OPCode <= 0xFF) bytes.push_back((char)OPCode);
    for (auto& it : operandes) {

        ssd::Buffer op_bytes = it.getValue();
        for (auto&& op_byte : op_bytes) {
            bytes.push_back(op_byte);
        }
    }
    return bytes;
}
