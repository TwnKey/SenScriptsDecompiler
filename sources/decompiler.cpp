
#include "headers/decompiler.h"
#include "headers/CS1InstructionsSet.h"
#include "headers/CS2InstructionsSet.h"
#include "headers/CS3InstructionsSet.h"
#include "headers/CS4InstructionsSet.h"
#include "headers/ReverieInstructionsSet.h"
#include "headers/TXInstructionsSet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxchartsheet.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <QColor>

using namespace QXlsx; // NOLINT(google-build-using-namespace)
namespace fs = std::filesystem;

Decompiler::Decompiler() = default;

bool Decompiler::setup_game(const std::string& game) {
    this->_game = game;
    std::transform(begin(game), end(game), begin(_game), ::tolower);

    if (_game == "cs3") {
        ib = std::make_unique<CS3Builder>();
    } else if (_game == "cs1") {
        ib = std::make_unique<CS1Builder>();
    } else if (_game == "cs2") {
        ib = std::make_unique<CS2Builder>();
    } else if (_game == "cs4") {
        ib = std::make_unique<CS4Builder>();
    } else if (_game == "reverie") {
        ib = std::make_unique<ReverieBuilder>();
    } else if (_game == "tx") {
        ib = std::make_unique<TXBuilder>();
    } else {
        display_text("FAILURE: Unrecognized game specified.");
        return false;
    }

    return true;
}
bool Decompiler::read_xlsx(const std::filesystem::path& filename) {
    if (!fs::exists(filename)) return false;

    Document doc(QString::fromStdString(filename.string()));
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
bool Decompiler::read_dat(const std::filesystem::path& filepath) {
    ssd::Buffer content = ssd::utils::read_file(filepath);

    ib->CreateHeaderFromDAT(content);
    ib->ReadFunctionsDAT(content);

    update_current_tf();
    return true;
}
bool Decompiler::write_dat(const std::filesystem::path& output_dir) {

    ssd::Buffer functions;
    ssd::Buffer current_fun;
    ssd::Buffer file_content;

    int addr = 0;
    ssd::Buffer header = ib->CreateHeaderBytes();

    addr = addr + static_cast<int>(std::ssize(header));
    for (int idx_fun = 0; idx_fun < current_tf.getNbFunctions() - 1; idx_fun++) {

        Function fun = current_tf.FunctionsInFile[idx_fun];

        current_fun.clear();

        for (auto& instr : fun.instructions) {
            current_fun.push_back(instr->getBytes());
        }
        addr = addr + static_cast<int>(std::ssize(current_fun));

        int next_addr = current_tf.FunctionsInFile[idx_fun + 1].actual_addr;
        int padding = next_addr - addr;
        for (int i_z = 0; i_z < padding; i_z++) {
            current_fun.push_back('\0');
        }
        addr = next_addr;
        functions.push_back(current_fun);
    }

    if (current_tf.getNbFunctions() - 1 >= 0) {
        Function fun = current_tf.FunctionsInFile[current_tf.FunctionsInFile.size() - 1];
        current_fun.clear();
        for (auto& instr : fun.instructions) {
            current_fun.push_back(instr->getBytes());
        }
        functions.push_back(current_fun);
    }

    file_content.push_back(header);
    file_content.push_back(functions);

    if (!fs::exists(output_dir)) fs::create_directories(output_dir);

    fs::path output_path = output_dir / (current_tf.getName() += ".dat");

    ssd::utils::write_file(output_path, file_content);

    display_text("File " + output_path.string() + " created.");
    return true;
}
bool Decompiler::write_xlsx(const std::filesystem::path& output_dir) {

    QFont font = QFont("Arial");
    fs::path filename = current_tf.getName() + ".xlsx";
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
        for (const auto& instr : fun.instructions) {
            excel_scenario_sheet.write(excel_row, 1, "Location");
            excel_scenario_sheet.write(excel_row + 1, 1, instr->get_addr_instr());
            int col = 0;
            instr->WriteXLSX(excel_scenario_sheet, current_tf.FunctionsInFile, excel_row, col);
            excel_row += 2;
        }
    }

    fs::path xlsx_output_file = output_dir / filename;

    display_text("File " + xlsx_output_file.string() + " created.");

    if (!fs::exists(output_dir)) fs::create_directories(output_dir);

    excel_scenario_sheet.saveAs(QString::fromStdString(xlsx_output_file.string()));

    return true;
}

bool Decompiler::check_all_files(const std::vector<std::filesystem::path>& files,
                                 const std::filesystem::path& reference_dir,
                                 const std::filesystem::path& output_dir) {
    for (const auto& file_ : files) {

        bool success = true;

        const fs::path full_path = reference_dir / file_;
        const fs::path filename = full_path.filename();
        const std::string file_stem = filename.stem().string();
        const fs::path reference_dat_path = reference_dir / filename;
        const fs::path local_dat_path = output_dir / filename;
        const fs::path xlsx_path = output_dir / (file_stem + ".xlsx");

        ssd::spdlog::info("checking {}", reference_dat_path.string());

        ssd::spdlog::info("start reading reference dat file: {}", reference_dat_path.string());
        this->read_file(reference_dat_path);
        ssd::spdlog::info("finish reading reference dat file: {}", reference_dat_path.string());
        ssd::spdlog::info("writing xlsx file: {}", xlsx_path.string());
        this->write_xlsx(output_dir);
        ssd::spdlog::info("reading xlsx file: {}", xlsx_path.string());
        this->read_file(xlsx_path);
        ssd::spdlog::info("reading xlsx file: {}", xlsx_path.string());
        ssd::spdlog::info("writing dat file: {}", local_dat_path.string());
        this->write_dat(output_dir);
        ssd::spdlog::info("reading {} and {} for comparison", local_dat_path.string(), reference_dat_path.string());

        const ssd::Buffer content1 = ssd::utils::read_file(local_dat_path);
        const ssd::Buffer content2 = ssd::utils::read_file(reference_dat_path);

        int length_header2 = ReadIntegerFromByteArray(0x18, content2);
        int idx_addresses_loc1 = ReadIntegerFromByteArray(8, content1);
        int idx_addresses_loc2 = ReadIntegerFromByteArray(8, content2);
        int size_pointer_section = ReadIntegerFromByteArray(0xC, content2);

        for (int i = 0; i < idx_addresses_loc2; i++) {
            if (content1[i] != content2[i]) {
                ssd::spdlog::err("First part of header, mismatch at {} {:#04x} should be {:#04x}", i, content1[i], content2[i]);
                success = false;
            }
        }

        for (int i = idx_addresses_loc2 + size_pointer_section; i < length_header2; i++) {
            if (content1[i] != content2[i]) {
                ssd::spdlog::err("Second part of header, mismatch at {} {:#04x} should be {:#04x}", i, content1[i], content2[i]);
                success = false;
            }
        }
        this->read_file(local_dat_path);
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

            for (size_t j = 0; j < (uint)current_tf.FunctionsInFile[i].instructions.size(); ++j) {
                uint OPCode = current_tf.FunctionsInFile[i].instructions[j]->get_OP();
                uint byte_in_file = (content2[index_byte]) & 0xFF;

                if (OPCode <= 0xFF) {
                    if (OPCode != byte_in_file) {
                        ssd::spdlog::err("Opcode mismatch at {:#04x} {:#04x} should be {:#04x}", index_byte, OPCode, byte_in_file);
                        success = false;
                    }
                    index_byte++;
                }
                for (size_t k = 0; k < (uint)current_tf.FunctionsInFile[i].instructions[j]->operandes.size(); ++k) {
                    operande op_k = current_tf.FunctionsInFile[i].instructions[j]->operandes[k];
                    ssd::Buffer bytes = op_k.getValue();

                    if (op_k.getType() == "pointer") {
                        int diff1 = op_k.getIntegerValue() - ReadIntegerFromByteArray(index_byte, content2);
                        int diff2 = current_tf.FunctionsInFile[i].actual_addr - idx_fun_2[i];
                        if (diff1 != diff2) {
                            ssd::spdlog::err("Mismatching pointers at {:#04x} {} should be {}", index_byte, diff1, diff2);
                            success = false;
                        }
                        index_byte+=4;

                    } else if (op_k.getType() == "float") {
                        ssd::Buffer float_bytes = ReadSubByteArray(content2, index_byte, 4);
                        float float2 = QByteArrayToFloat(float_bytes);
                        float float1 = QByteArrayToFloat(bytes);
                        if (float2 != float1) {
                            ssd::spdlog::err("Mismatching floats at {:#04x} {} should be {}", index_byte, float1, float2);
                            success = false;
                        }
                    } else {
                        for (auto&& byte : bytes) {
                            uint8_t byte1 = content2[index_byte];
                            uint8_t byte2 = byte;

                            if (byte1 != byte2) {
                                ssd::spdlog::err("Mismatch at {:#04x} {:#04x} should be {:#04x}", index_byte, byte1, byte2);
                                success = false;
                            }
                            index_byte++;
                        }
                    }
                }
            }
        }
        if (!success) ssd::spdlog::err("Mismatch");

        std::string match_msg = success ? "matches" : "does not match";
        ssd::spdlog::info("Size 1: {:#04x} vs Size 2: {:#04x} {}", content1.size(), content2.size(), match_msg);
    }
    return true;
}
bool Decompiler::read_file(const fs::path& filepath) {
    ib->Reset();
    current_tf = TranslationFile();

    if (filepath.extension() == ".xlsx") {
        read_xlsx(filepath);
    } else if (filepath.extension() == ".dat") {
        read_dat(filepath);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
bool Decompiler::write_file(const fs::path& filepath, const fs::path& output_dir) {

    if (filepath.extension() == ".dat") {
        write_xlsx(output_dir);
    } else if (filepath.extension() == ".xlsx") {
        write_dat(output_dir);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
TranslationFile Decompiler::get_tf() { return current_tf; }
