#include "headers/decompiler.h"
#include "headers/CS1InstructionsSet.h"
#include "headers/CS2InstructionsSet.h"
#include "headers/CS3InstructionsSet.h"
#include "headers/CS4InstructionsSet.h"
#include "headers/ReverieInstructionsSet.h"
#include "headers/TXInstructionsSet.h"
#include "qxlsx/headers/xlsxcellrange.h"
#include "qxlsx/headers/xlsxchart.h"
#include "qxlsx/headers/xlsxchartsheet.h"
#include "qxlsx/headers/xlsxdocument.h"
#include "qxlsx/headers/xlsxformat.h"
#include "qxlsx/headers/xlsxrichstring.h"
#include "qxlsx/headers/xlsxworkbook.h"
#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

using namespace QXlsx; // NOLINT(google-build-using-namespace)

Decompiler::Decompiler() = default;

bool Decompiler::setup_game(const std::string& game) {
    this->_game = game;
    if (game == "CS3") {
        ib = std::make_unique<CS3Builder>();
    } else if (game == "CS1") {
        ib = std::make_unique<CS1Builder>();
    } else if (game == "CS2") {
        ib = std::make_unique<CS2Builder>();
    } else if (game == "CS4") {
        ib = std::make_unique<CS4Builder>();
    } else if (game == "Reverie") {
        ib = std::make_unique<ReverieBuilder>();
    } else if (game == "TX") {
        ib = std::make_unique<TXBuilder>();
    } else {
        display_text("FAILURE: Unrecognized game specified.");
        return false;
    }

    return true;
}
bool Decompiler::read_xlsx(QFile& file) {
    QFileInfo info(file);

    if (!file.open(QIODevice::ReadOnly)) return false;

    Document doc(info.absoluteFilePath());
    auto game_from_file = doc.read(1, 1).toString().toStdString();
    setup_game(game_from_file);
    display_text("Reading functions...");

    ib->ReadFunctionsXLSX(doc);

    display_text("Reading of XLSX done.");

    update_current_tf();
    return true;
}

bool Decompiler::update_current_tf() {
    current_tf.setName(ib->SceneName);

    current_tf.FunctionsInFile.clear();
    for (auto& fun : ib->FunctionsParsed) {
        current_tf.addFunction(fun);
    }

    return true;
}
bool Decompiler::read_dat(QFile& file) {

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray content = file.readAll();

    ib->CreateHeaderFromDAT(content);
    ib->ReadFunctionsDAT(content);

    update_current_tf();
    return true;
}
bool Decompiler::write_dat(const QString& output_dir) {

    QByteArray functions;
    QByteArray current_fun;
    QByteArray file_content;

    int addr = 0;
    QByteArray header = ib->CreateHeaderBytes();

    addr = addr + header.size();
    for (int idx_fun = 0; idx_fun < current_tf.getNbFunctions() - 1; idx_fun++) {

        function fun = current_tf.FunctionsInFile[idx_fun];

        current_fun.clear();

        for (auto& instr : fun.InstructionsInFunction) {
            current_fun.push_back(instr->getBytes());
        }
        addr = addr + current_fun.size();

        int next_addr = current_tf.FunctionsInFile[idx_fun + 1].actual_addr;
        int padding = next_addr - addr;
        for (int i_z = 0; i_z < padding; i_z++) {
            current_fun.push_back('\0');
        }
        addr = next_addr;
        functions.push_back(current_fun);
    }

    if (current_tf.getNbFunctions() - 1 >= 0) {
        function fun = current_tf.FunctionsInFile[current_tf.FunctionsInFile.size() - 1];
        current_fun.clear();
        for (auto& instr : fun.InstructionsInFunction) {
            current_fun.push_back(instr->getBytes());
        }
        functions.push_back(current_fun);
    }

    file_content.append(header);
    file_content.append(functions);

    QDir dir(output_dir);
    if (!dir.exists()) dir.mkpath(".");

    QString output_path = output_dir + "\\" + QString::fromStdString(current_tf.getName()) + ".dat";
    QFile file(output_path);

    file.open(QIODevice::WriteOnly);
    file.write(file_content);
    file.close();
    display_text("File " + output_path + " created.");
    return true;
}
bool Decompiler::write_xlsx(const QString& output_dir) {

    QFont font = QFont("Arial");
    QString filename = QString::fromStdString(current_tf.getName()) + ".xlsx";
    QXlsx::Document excel_scenario_sheet;
    Format format;
    format.setFont(font);
    format.setFontBold(true);
    auto dark_yellow = QColor(qRgb(255, 222, 155));
    format.setPatternBackgroundColor(dark_yellow);
    auto font_color = QColor(qRgb(255, 0, 0));

    format.setFontColor(font_color);
    format.setFontSize(14);

    excel_scenario_sheet.write("A1", QString::fromStdString(_game), format);

    Format format_location;

    format_location.setFont(font);
    format_location.setFontSize(10);
    format_location.setPatternBackgroundColor(dark_yellow);
    excel_scenario_sheet.write("A2", QString::fromStdString(current_tf.getName()), format_location);

    Format row_format;
    row_format.setFillPattern(Format::PatternSolid);
    row_format.setPatternBackgroundColor(qRgb(255, 255, 200));
    row_format.setFont(font);
    row_format.setFontSize(10);
    excel_scenario_sheet.setRowFormat(1, 3, format);
    excel_scenario_sheet.setRowFormat(2, 3, format);
    excel_scenario_sheet.setRowFormat(3, 3, format);
    row_format.setBottomBorderStyle(Format::BorderNone);
    Format format_title_chars;
    format_title_chars.setFont(font);
    format_title_chars.setFontSize(10);
    format_title_chars.setPatternBackgroundColor(qRgb(255, 255, 200));
    format_title_chars.setHorizontalAlignment(Format::AlignHCenter);
    format_title_chars.setVerticalAlignment(Format::AlignVCenter);

    Format Format_title_column_chars;
    Format_title_column_chars.setHorizontalAlignment(Format::AlignHCenter);
    Format_title_column_chars.setVerticalAlignment(Format::AlignVCenter);
    Format_title_column_chars.setFontSize(10);
    Format_title_column_chars.setPatternBackgroundColor(qRgb(255, 255, 200));
    Format_title_column_chars.setTopBorderStyle(Format::BorderThin);
    Format_title_column_chars.setBottomBorderStyle(Format::BorderThin);
    Format_title_column_chars.setLeftBorderStyle(Format::BorderThin);
    Format_title_column_chars.setRightBorderStyle(Format::BorderThin);
    Format_title_column_chars.setFont(font);
    Format_title_column_chars.setFontSize(10);
    Format_title_column_chars.setPatternBackgroundColor(qRgb(255, 255, 255));

    Format_title_column_chars.setBottomBorderStyle(Format::BorderThin);
    Format_title_column_chars.setLeftBorderStyle(Format::BorderThin);
    Format_title_column_chars.setRightBorderStyle(Format::BorderThin);
    Format_title_column_chars.setTopBorderStyle(Format::BorderThin);
    Format row_format_functions;
    row_format_functions.setPatternBackgroundColor(qRgb(255, 200, 200));
    row_format_functions.setFont(font);
    row_format_functions.setFontSize(13);
    row_format_functions.setBottomBorderStyle(QXlsx::Format::BorderThin);

    row_format_functions.setTopBorderStyle(QXlsx::Format::BorderThin);
    int excel_row = 4;
    for (const auto& fun : current_tf.FunctionsInFile) {
        excel_scenario_sheet.setRowFormat(excel_row, excel_row, row_format_functions);
        excel_scenario_sheet.write(excel_row, 1, "FUNCTION");
        excel_scenario_sheet.write(excel_row, 2, QString::fromStdString(fun.name));

        excel_row++;
        for (const auto& instr : fun.InstructionsInFunction) {
            excel_scenario_sheet.write(excel_row, 1, "Location");
            excel_scenario_sheet.write(excel_row + 1, 1, instr->get_addr_instr());
            int col = 0;
            instr->WriteXLSX(excel_scenario_sheet, current_tf.FunctionsInFile, excel_row, col);
            excel_row += 2;
        }
    }

    QString xlsx_output_file = QDir::toNativeSeparators(output_dir + "/" + filename);

    display_text("File " + xlsx_output_file + " created.");
    QDir dir(output_dir);
    if (!dir.exists()) dir.mkpath(".");
    excel_scenario_sheet.saveAs(xlsx_output_file);
    return true;
}

bool Decompiler::check_all_files(const QString& log_filename,
                                 const QStringList& files,
                                 const QString& reference_dir,
                                 const QString& output_dir) {
    QFile file(log_filename);

    QTextStream stream(&file);
    file.remove();
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for (const auto& file_ : files) {

        bool success = true;

        QString full_path = reference_dir + "/" + file_;
        QString filename = full_path.mid(full_path.lastIndexOf("/"));
        QString croped_fileName = filename.section(".", 0, 0);
        qDebug() << "Checking " << full_path;
        QString full_path_ref = reference_dir + filename;
        stream << full_path << "\n";
        this->setup_game("Reverie");
        qDebug() << "reading dat1 file" << full_path;
        this->read_file(full_path);
        qDebug() << "reading dat done." << full_path;
        this->write_xlsx(output_dir);
        qDebug() << "reading xlsx file" << output_dir + croped_fileName + ".xlsx";
        this->read_file(output_dir + croped_fileName + ".xlsx");
        qDebug() << "writing dat file";
        this->write_dat(output_dir);
        qDebug() << "full done";
        qDebug() << "reading dat file" << output_dir + filename;
        qDebug() << "reading dat file" << full_path_ref;
        QFile file1(output_dir + filename);
        QFile file2(full_path_ref);
        if (!file1.open(QIODevice::ReadOnly)) {

            return false;
        }

        const QByteArray content1 = file1.readAll();
        if (!file2.open(QIODevice::ReadOnly)) {

            return false;
        }

        const QByteArray content2 = file2.readAll();
        std::string msg = "Problème de taille avec " + croped_fileName.toStdString();

        int length_header2 = ReadIntegerFromByteArray(0x18, content2);
        int idx_addresses_loc1 = ReadIntegerFromByteArray(8, content1);
        int idx_addresses_loc2 = ReadIntegerFromByteArray(8, content2);
        int size_pointer_section = ReadIntegerFromByteArray(0xC, content2);

        for (int i = 0; i < idx_addresses_loc2; i++) {
            if (content1[i] != content2[i]) {
                stream << "First part of header, mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
                qDebug() << "First part of header, mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
                success = false;
            }
        }

        for (int i = idx_addresses_loc2 + size_pointer_section; i < length_header2; i++) {
            if (content1[i] != content2[i]) {
                stream << "Second part of header, mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
                qDebug() << "Second part of header, mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
                success = false;
            }
        }
        this->read_file(output_dir + filename);
        std::vector<int> idx_fun_1;
        std::vector<int> idx_fun_2;
        for (int i = idx_addresses_loc1; i < idx_addresses_loc1 + size_pointer_section; i += 4) {
            idx_fun_1.push_back(ReadIntegerFromByteArray(i, content1));
        }
        for (int i = idx_addresses_loc2; i < idx_addresses_loc2 + size_pointer_section; i += 4) {
            idx_fun_2.push_back(ReadIntegerFromByteArray(i, content2));
        }

        for (size_t i = 0; i < (uint)current_tf.getNbFunctions(); ++i) {
            int index_byte = idx_fun_2[i];

            for (size_t j = 0; j < (uint)current_tf.FunctionsInFile[i].InstructionsInFunction.size(); ++j) {
                uint OPCode = current_tf.FunctionsInFile[i].InstructionsInFunction[j]->get_OP();
                uint byte_in_file = (content2[index_byte]) & 0xFF;

                if (OPCode <= 0xFF) {
                    if (OPCode != byte_in_file) {
                        stream << "OP Code Mismatch at " << Qt::hex << index_byte << " " << OPCode << " should be " << byte_in_file << "\n";
                        qDebug() << "OP Code Mismatch at " << Qt::hex << index_byte << " " << OPCode << " should be " << byte_in_file << "\n";
                        success = false;
                    }
                    index_byte++;
                }
                for (size_t k = 0; k < (uint)current_tf.FunctionsInFile[i].InstructionsInFunction[j]->operandes.size(); ++k) {
                    operande op_k = current_tf.FunctionsInFile[i].InstructionsInFunction[j]->operandes[k];
                    QByteArray bytes = op_k.getValue();

                    if (op_k.getType() == "pointer") {
                        int diff1 = op_k.getIntegerValue() - ReadIntegerFromByteArray(index_byte, content2);
                        int diff2 = current_tf.FunctionsInFile[i].actual_addr - idx_fun_2[i];
                        if (diff1 != diff2) {
                            stream << "Mismatching pointers at " << Qt::hex << index_byte << " " << diff1 << " should be " << diff2 << "\n";
                            qDebug() << "Mismatching pointers at " << Qt::hex << index_byte << " " << diff1 << " should be " << diff2 << "\n";
                            success = false;
                        }
                        index_byte+=4;

                    } else if (op_k.getType() == "float") {
                        QByteArray float_bytes = ReadSubByteArray(content2, index_byte, 4);
                        float float2 = QByteArrayToFloat(float_bytes);
                        float float1 = QByteArrayToFloat(bytes);
                        if (float2 != float1) {
                            stream << "Mismatching floats at " << Qt::hex << index_byte << " " << float1 << " should be " << float2 << "\n";
                            qDebug() << "Mismatching floats at " << Qt::hex << index_byte << " " << float1 << " should be " << float2 << "\n";
                            success = false;
                        }
                    } else {
                        for (auto&& byte : bytes) {
                            uint8_t byte1 = content2[index_byte];
                            uint8_t byte2 = byte;

                            if (byte1 != byte2) {
                                stream << "Mismatch at " << Qt::hex << index_byte << " " << (int)byte1 << " should be " << (int)byte2 << "\n";
                                qDebug() << "Mismatch at " << Qt::hex << index_byte << " " << (int)byte1 << " should be " << (int)byte2 << "\n";
                                success = false;
                            }
                            index_byte++;
                        }
                    }
                }
            }
        }
        if (!success) {
            qFatal("Mismatch");
            stream << "Mismatch"
                   << "\n";
        }
        stream << " Size 1: " << Qt::hex << content1.size() << " vs Size 2: " << Qt::hex << content2.size() << " " << (int)success;
    }
    return true;
}
bool Decompiler::read_file(const QString& filepath) {
    ib->Reset();
    current_tf = TranslationFile();
    QFile file(filepath);
    QFileInfo info_file(file);

    if (info_file.suffix() == "xlsx") {
        read_xlsx(file);
    } else if (info_file.suffix() == "dat") {
        read_dat(file);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
bool Decompiler::write_file(const QString& filepath, const QString& output_dir) {

    QFile file(filepath);
    QFileInfo info_file(file);
    if (info_file.suffix() == "dat") {
        write_xlsx(output_dir);
    } else if (info_file.suffix() == "xlsx") {
        write_dat(output_dir);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
TranslationFile Decompiler::get_tf() { return current_tf; }
