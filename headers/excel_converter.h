#pragma once
#include <filesystem>
#include <stack>
#include <string>

#include "xlsxdocument.h"
#include <QTextCodec>

#include "headers/data_converter.h"
#include "headers/functions.h"

namespace ssd {
class ExcelDatDataReader : public ForeignDatDataReader {
  public:
    ExcelDatDataReader() = delete;
    explicit ExcelDatDataReader(std::string encoding, Builder* builder)
      : _encoding{ std::move(encoding) }
      , _builder{ builder } {}

    DatDataReadResult from_file(const std::filesystem::path& filepath) override {
        doc.emplace(QString::fromStdString(filepath.string()));

        auto game = doc->read(1, 1).toString().toStdString();
        std::transform(begin(game), end(game), begin(game), ::tolower);
        game_code = game;
        scene_name = doc->read(2, 1).toString().toStdString();

        auto result = DatDataReadResult{ game_code, scene_name, read_functions() };

        return result;
    }

  private:
    std::vector<Function> read_functions() {
        std::vector<Function> functions;
        int first_row = 4;
        int last_row = doc->dimension().lastRow();
        int fun_id = 0;

        std::string content_first_cell = doc->read(first_row, 1).toString().toStdString();

        if (content_first_cell != "FUNCTION") {
            return functions;
        }

        auto fun_name = doc->read(first_row, 2).toString().toStdString();
        Function current_fun(fun_id, fun_name, 0, 0, 0);
        current_fun.xlsx_row_index = first_row;
        int addr_instr = 0;

        for (int idx_row = first_row + 1; idx_row < last_row; idx_row++) {
            content_first_cell = doc->read(idx_row, 1).toString().toStdString();
            if (content_first_cell == "FUNCTION") { // We start a new function
                addr_instr = 0;
                functions.push_back(current_fun);

                std::string next_fun_name = doc->read(idx_row, 2).toString().toStdString();
                current_fun = Function(fun_id, next_fun_name, 0, 0, 0);
                current_fun.xlsx_row_index = idx_row;
                fun_id++;
            } else {
                int opcode = doc->read(idx_row + 1, 2).toInt();
                std::shared_ptr<Instruction> instr = _builder->create_instruction_from_xlsx(addr_instr, opcode, scene_name);
                if (instr != nullptr) {
                    auto modified_instr = read_sub_instruction(addr_instr, idx_row, instr);
                    instr.swap(modified_instr);
                } else {
                    instr->error = true;
                    ssd::spdlog::err("Opcode {#04x} is not defined", opcode);
                }
                current_fun.add_instruction(instr);
                idx_row++;
            }
        }
        functions.push_back(current_fun);

        return functions;
    }
    std::shared_ptr<Instruction> read_sub_instruction(int& addr, int idx_row, const std::shared_ptr<Instruction>& instruction) {
        std::stack<std::pair<size_t, uint8_t>> stack_of_headers;

        if (instruction->get_opcode() <= 0xFF) addr++;

        int idx_column = 3;
        std::string type = doc->read(idx_row, idx_column).toString().toStdString();

        while (!type.empty()) {
            ssd::Buffer value;
            Operande operande;
            if (type == "int") {
                int op = doc->read(idx_row + 1, idx_column).toInt();

                value = GetBytesFromInt(op);
                operande = Operande(addr, "int", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if (type == "Group ID") {
                stack_of_headers.push(std::make_pair(0, addr - 1)); // the count includes OPCode thus -1

                int operande_grp = doc->read(idx_row + 1, idx_column).toInt();
                idx_column++;
                uint8_t operande_length = doc->read(idx_row + 1, idx_column).toInt();
                // If the byte for length is 0xFF we don't calculate it
                if (operande_length != 0xFF) {
                    stack_of_headers.top().first = instruction->get_nb_operandes();
                }
                value = GetBytesFromInt(operande_grp + (operande_length << 0x18));
                operande = Operande(addr, "header", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();

            } else if (type == "float") {
                float op = doc->read(idx_row + 1, idx_column).toFloat();

                value = GetBytesFromFloat(op);
                operande = Operande(addr, "float", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if (type == "short") {
                int16_t op = static_cast<int16_t>(doc->read(idx_row + 1, idx_column).toInt());
                value = GetBytesFromShort(op);
                operande = Operande(addr, "short", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if ((type == "byte") || (type == "OP Code")) {
                int opcode = doc->read(idx_row + 1, idx_column).toInt();

                if (opcode <= 0xFF) { // actually the opposite never happens.
                    auto op = static_cast<char>(((opcode)&0x000000FF));
                    value.push_back(op);
                    operande = Operande(addr, "byte", value);
                    instruction->add_operande(operande);
                    addr = addr + operande.get_length();
                }
            } else if (type == "End") {
                if (!stack_of_headers.empty()) {
                    if (stack_of_headers.top().first != 0) {
                        uint32_t new_header_value =
                          ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                          (instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
                        ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
                        instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
                        stack_of_headers.pop();
                    }
                }
            } else if (type == "fill") {
                QString operande_prev = doc->read(idx_row + 1, idx_column - 1).toString();
                QString op = doc->read(idx_row + 1, idx_column).toString();
                QString str_max_length = op.mid(1, op.indexOf('-') - 1);

                for (int id = 0; id < str_max_length.toInt() - operande_prev.toUtf8().size() - 1; id++) {
                    value.push_back('\0');
                }
                operande = Operande(addr, "fill", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if ((type == "string") || (type == "dialog")) {
                QString op = (doc->read(idx_row + 1, idx_column).toString());
                QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(_encoding).toUtf8());
                auto q_byte_array = codec->fromUnicode(op);
                value = ssd::Buffer(std::begin(q_byte_array), std::end(q_byte_array));
                if (type == "string") value.push_back('\0');
                value.replace('\n', 1);

                operande = Operande(addr, type, value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if (type == "bytearray") {
                while (type == "bytearray") {
                    auto op = static_cast<char>(((doc->read(idx_row + 1, idx_column).toInt()) & 0x000000FF));
                    value.push_back(op);
                    idx_column++;
                    type = doc->read(idx_row, idx_column).toString().toStdString();
                }
                operande = Operande(addr, "bytearray", value);
                idx_column--;
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            } else if (type == "pointer") {
                QString op = (doc->read(idx_row + 1, idx_column).toString());

                QString RowPointedStr = op.right(op.size() - 2);

                int actual_row = RowPointedStr.toInt();

                value = GetBytesFromInt(actual_row);

                auto operande = Operande(addr, "pointer", value);
                instruction->add_operande(operande);
                addr = addr + operande.get_length();
            }
            idx_column++;
            type = doc->read(idx_row, idx_column).toString().toStdString();
        }

        if (!stack_of_headers.empty()) {
            if (stack_of_headers.top().first != 0) {
                uint32_t new_header_value =
                  ((addr - static_cast<int>(stack_of_headers.top().second)) << 0x18) +
                  (instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).get_integer_value() & 0xFFFFFF);
                ssd::Buffer header_bytes = GetBytesFromInt(new_header_value);
                instruction->get_operande(static_cast<int>(stack_of_headers.top().first)).set_value(header_bytes);
                stack_of_headers.pop();
            }
        }
        return instruction;
    }

    std::string _encoding;
    std::string game_code;
    std::string scene_name;
    std::optional<QXlsx::Document> doc;
    Builder* _builder = nullptr;
};

class ExcelDatDataWriter : public ForeignDatDataWriter {
  public:
    ExcelDatDataWriter() = delete;
    explicit ExcelDatDataWriter(std::string game_code, std::string encoding, Builder* Maker)
      : _game_code{ std::move(game_code) }
      , _encoding{ std::move(encoding) }
      , builder(Maker) {}

    void to_file(const std::vector<Function>& functions, const std::filesystem::path& filepath) override {
        doc.emplace();
        build(functions, filepath);

        doc->saveAs(QString::fromStdString(filepath.string()));
    }

    void build(const std::vector<Function>& functions, const std::filesystem::path& filepath) {
        auto short_name = filepath.filename().stem().string();

        QFont font = QFont("Arial");

        QXlsx::Format format;
        format.setFont(font);
        format.setFontBold(true);
        auto dark_yellow = QColor(qRgb(255, 222, 155));
        format.setPatternBackgroundColor(dark_yellow);
        auto font_color = QColor(qRgb(255, 0, 0));

        format.setFontColor(font_color);
        format.setFontSize(14);

        doc->write("A1", QString::fromStdString(_game_code), format);

        QXlsx::Format format_location;

        format_location.setFont(font);
        format_location.setFontSize(10);
        format_location.setPatternBackgroundColor(dark_yellow);
        doc->write("A2", QString::fromStdString(short_name), format_location);

        QXlsx::Format row_format;
        row_format.setFillPattern(QXlsx::Format::PatternSolid);
        row_format.setPatternBackgroundColor(qRgb(255, 255, 200));
        row_format.setFont(font);
        row_format.setFontSize(10);
        doc->setRowFormat(1, 3, format);
        doc->setRowFormat(2, 3, format);
        doc->setRowFormat(3, 3, format);
        row_format.setBottomBorderStyle(QXlsx::Format::BorderNone);

        QXlsx::Format format_title_chars;
        format_title_chars.setFont(font);
        format_title_chars.setFontSize(10);
        format_title_chars.setPatternBackgroundColor(qRgb(255, 255, 200));
        format_title_chars.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format_title_chars.setVerticalAlignment(QXlsx::Format::AlignVCenter);

        QXlsx::Format format_title_column_chars;
        format_title_column_chars.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format_title_column_chars.setVerticalAlignment(QXlsx::Format::AlignVCenter);
        format_title_column_chars.setFontSize(10);
        format_title_column_chars.setPatternBackgroundColor(qRgb(255, 255, 200));
        format_title_column_chars.setTopBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setBottomBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setLeftBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setRightBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setFont(font);
        format_title_column_chars.setFontSize(10);
        format_title_column_chars.setPatternBackgroundColor(qRgb(255, 255, 255));
        format_title_column_chars.setBottomBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setLeftBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setRightBorderStyle(QXlsx::Format::BorderThin);
        format_title_column_chars.setTopBorderStyle(QXlsx::Format::BorderThin);

        QXlsx::Format row_format_functions;
        row_format_functions.setPatternBackgroundColor(qRgb(255, 200, 200));
        row_format_functions.setFont(font);
        row_format_functions.setFontSize(13);
        row_format_functions.setBottomBorderStyle(QXlsx::Format::BorderThin);

        row_format_functions.setTopBorderStyle(QXlsx::Format::BorderThin);
        int excel_row = 4;
        for (const auto& fun : functions) {
            doc->setRowFormat(excel_row, excel_row, row_format_functions);
            doc->write(excel_row, 1, "FUNCTION");
            doc->write(excel_row, 2, QString::fromStdString(fun.name));

            excel_row++;
            for (const auto& instr : fun.instructions) {
                doc->write(excel_row, 1, "Location");
                doc->write(excel_row + 1, 1, instr->get_addr_instr());
                int col = 0;
                write_instructions(functions, instr, excel_row, col);
                excel_row += 2;
            }
        }
    }

    int write_instructions(const std::vector<Function>& funs, const std::shared_ptr<Instruction>& instruction, int row, int& col) {
        int opcode = static_cast<int>(instruction->get_opcode());
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

        color = QColor::fromHsl((int)opcode, 255, 185, 255);
        FormatOP.setPatternBackgroundColor(color);

        if (instruction->error) {
            doc->write(row, col + 1, "ERROR!", FormatType);
        }
        doc->write(row, col + 2, "OP Code", FormatType);
        doc->write(row + 1, col + 2, opcode, FormatOP);
        int col_cnt = 0;
        for (auto& operande : instruction->operandes) {

            QString type = QString::fromStdString(operande.get_type());
            ssd::Buffer value = operande.get_value();

            if (type == "int") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                doc->write(row + 1, col + 3 + col_cnt, ReadIntegerFromByteArray(0, value), FormatInstr);
                col_cnt++;
            } else if (type == "header") {
                uint32_t header_value = ReadIntegerFromByteArray(0, value);
                uint8_t length_byte = header_value >> 0x18;
                uint32_t group_id = header_value & 0xFFFFFF;
                doc->write(row, col + 3 + col_cnt, "Group ID", FormatType);
                doc->write(row + 1, col + 3 + col_cnt, group_id, FormatInstr);
                col_cnt++;
                doc->write(row, col + 3 + col_cnt, "Length", FormatType);
                doc->write(row + 1, col + 3 + col_cnt, length_byte, FormatInstr);
                col_cnt++;
            } else if (type == "float") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                doc->write(row + 1, col + 3 + col_cnt, QByteArrayToFloat(value), FormatInstr);
                col_cnt++;
            } else if (type == "short") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                doc->write(row + 1, col + 3 + col_cnt, (uint16_t)ReadShortFromByteArray(0, value), FormatInstr);
                col_cnt++;
            } else if (type == "byte") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                doc->write(row + 1, col + 3 + col_cnt, (unsigned char)value[0], FormatInstr);
                col_cnt++;
            } else if (type == "fill") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                doc->write(row + 1,
                           col + 3 + col_cnt,
                           "=" + QString::number(operande.get_bytes_to_fill()) + "-LENB(INDIRECT(ADDRESS(" + QString::number(row + 1) +
                             "," + QString::number(3 + col_cnt - 1) + ")))",
                           FormatInstr);
                col_cnt++;
            } else if (type == "bytearray") {
                for (auto&& idx_byte : value) {
                    doc->write(row, col + 3 + col_cnt, type, FormatType);
                    doc->write(row + 1, col + 3 + col_cnt, (unsigned char)idx_byte, FormatInstr);
                    col_cnt++;
                }
            } else if (type == "instruction") {
                int addr = 0;
                doc->write(row, col + 3 + col_cnt, "Start", FormatStartEnd);
                doc->write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
                col_cnt++;

                // function type is 0 here because sub05 is only called by OP Code instructions.
                std::shared_ptr<Instruction> instr = builder->create_instruction_from_dat(addr, value, 0);
                int column_instr = col + 3 + col_cnt - 2;
                int cnt = write_instructions(funs, instr, row, column_instr);
                col = col + cnt + 1;

                doc->write(row, col + 3 + col_cnt, "End", FormatStartEnd);
                doc->write(row + 1, col + 3 + col_cnt, "", FormatStartEnd);
                col_cnt++;
            } else if ((type == "string") || (type == "dialog")) {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                ssd::Buffer value_ = value;

                value_.replace(1, '\n');

                QTextCodec* codec = QTextCodec::codecForName(QString::fromStdString(_encoding).toUtf8());

                auto q_byte_array = QByteArray(reinterpret_cast<const char*>(value_.data()), static_cast<int>(std::ssize(value_)));
                QString string = codec->toUnicode(q_byte_array);

                doc->write(row + 1, col + 3 + col_cnt, string, FormatInstr);
                col_cnt++;
            } else if (type == "pointer") {
                doc->write(row, col + 3 + col_cnt, type, FormatType);
                int id = funs[0].id;
                size_t nb_row = 3;
                int idx_fun = 0;
                while (id != operande.get_destination().function_id) {
                    nb_row = nb_row + 1; // row with function name
                    nb_row = nb_row + 2 * funs[idx_fun].instructions.size();
                    idx_fun++;
                    id = funs[idx_fun].id;
                }
                nb_row = nb_row + 1; // row with function name
                nb_row = nb_row + static_cast<uint64_t>(2 * (operande.get_destination().instruction_id + 1));
                QString ptrExcel = "=A" + QString::number((nb_row));

                QXlsx::Format format;
                format.setBottomBorderStyle(QXlsx::Format::BorderThin);
                format.setLeftBorderStyle(QXlsx::Format::BorderThin);
                format.setRightBorderStyle(QXlsx::Format::BorderThin);
                format.setTopBorderStyle(QXlsx::Format::BorderThin);
                format.setFontBold(true);
                auto FontColor = QColor(qRgb(255, 0, 0));
                format.setFontColor(FontColor);
                doc->write(row + 1, col + 3 + col_cnt, ptrExcel, format);
                col_cnt++;
            }
        }

        return col_cnt;
    }

  private:
    std::optional<QXlsx::Document> doc;
    std::string _game_code;
    std::string _encoding;
    Builder* builder;
};
}; // namespace ssd
