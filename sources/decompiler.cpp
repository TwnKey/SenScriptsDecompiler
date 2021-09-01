#include "headers/decompiler.h"
#include "headers/CS1InstructionsSet.h"
#include "headers/CS2InstructionsSet.h"
#include "headers/CS3InstructionsSet.h"
#include "headers/CS4InstructionsSet.h"
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

using namespace QXlsx;

Decompiler::Decompiler() = default;

bool Decompiler::SetupGame(std::string Game_) {
    Game = Game_;
    if (Game == "CS3") {
        IB = new CS3Builder();
    } else if (Game == "CS1") {
        IB = new CS1Builder();
    } else if (Game == "CS2") {

        IB = new CS2Builder();
    } else if (Game == "CS4") {
        IB = new CS4Builder();
    } else if (Game == "TX") {
        IB = new TXBuilder();
    } else {
        display_text("FAILURE: Unrecognized game specified.");
        return false;
    }

    return true;
}
bool Decompiler::ReadXLSX(QFile& File) {
    QFileInfo info(File);

    if (!File.open(QIODevice::ReadOnly)) return false;

    Document doc(info.absoluteFilePath());
    Game = doc.read(1, 1).toString().toStdString();
    SetupGame(Game);
    display_text("Reading functions...");

    IB->ReadFunctionsXLSX(doc);

    display_text("Reading of XLSX done.");

    UpdateCurrentTF();
    return true;
}

bool Decompiler::UpdateCurrentTF() {
    CurrentTF.setName(IB->SceneName);

    CurrentTF.FunctionsInFile.clear();
    for (auto& fun : IB->FunctionsParsed) {
        CurrentTF.addFunction(fun);
    }

    return true;
}
bool Decompiler::ReadDAT(QFile& File) {

    if (!File.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray content = File.readAll();

    IB->CreateHeaderFromDAT(content);
    IB->ReadFunctionsDAT(content);

    UpdateCurrentTF();
    return true;
}
bool Decompiler::WriteDAT(const QString& folder) {

    QByteArray functions;
    QByteArray current_fun;
    QByteArray file_content;

    int addr = 0;
    QByteArray header = IB->CreateHeaderBytes();

    addr = addr + header.size();
    for (int idx_fun = 0; idx_fun < CurrentTF.getNbFunctions() - 1; idx_fun++) {

        function fun = CurrentTF.FunctionsInFile[idx_fun];

        current_fun.clear();

        for (auto& instr : fun.InstructionsInFunction) {
            current_fun.push_back(instr->getBytes());
        }
        addr = addr + current_fun.size();

        int next_addr = CurrentTF.FunctionsInFile[idx_fun + 1].actual_addr;
        int padding = next_addr - addr;
        for (int i_z = 0; i_z < padding; i_z++) {
            current_fun.push_back('\0');
        }
        addr = next_addr;
        functions.push_back(current_fun);
    }

    if (CurrentTF.getNbFunctions() - 1 >= 0) {
        function fun = CurrentTF.FunctionsInFile[CurrentTF.FunctionsInFile.size() - 1];
        current_fun.clear();
        for (auto& instr : fun.InstructionsInFunction) {
            current_fun.push_back(instr->getBytes());
        }
        functions.push_back(current_fun);
    }

    file_content.append(header);
    file_content.append(functions);

    QDir dir(folder);
    if (!dir.exists()) dir.mkpath(".");

    QString output_path = folder + "\\" + QString::fromStdString(CurrentTF.getName()) + ".dat";
    QFile file(output_path);

    file.open(QIODevice::WriteOnly);
    file.write(file_content);
    file.close();
    display_text("File " + output_path + " created.");
    return true;
}
bool Decompiler::WriteXLSX(const QString& output_folder) {

    QFont font = QFont("Arial");
    QString filename = QString::fromStdString(CurrentTF.getName()) + ".xlsx";
    QXlsx::Document excelScenarioSheet;
    Format format;
    format.setFont(font);
    format.setFontBold(true);
    auto DarkYellow = QColor(qRgb(255, 222, 155));
    format.setPatternBackgroundColor(DarkYellow);
    auto FontColor = QColor(qRgb(255, 0, 0));

    format.setFontColor(FontColor);
    format.setFontSize(14);

    excelScenarioSheet.write("A1", QString::fromStdString(Game), format);

    Format formatLocation;

    formatLocation.setFont(font);
    formatLocation.setFontSize(10);
    formatLocation.setPatternBackgroundColor(DarkYellow);
    excelScenarioSheet.write("A2", QString::fromStdString(CurrentTF.getName()), formatLocation);

    Format rowFormat;
    rowFormat.setFillPattern(Format::PatternSolid);
    rowFormat.setPatternBackgroundColor(qRgb(255, 255, 200));
    rowFormat.setFont(font);
    rowFormat.setFontSize(10);
    excelScenarioSheet.setRowFormat(1, 3, format);
    excelScenarioSheet.setRowFormat(2, 3, format);
    excelScenarioSheet.setRowFormat(3, 3, format);
    rowFormat.setBottomBorderStyle(Format::BorderNone);
    Format formatTitleChars;
    formatTitleChars.setFont(font);
    formatTitleChars.setFontSize(10);
    formatTitleChars.setPatternBackgroundColor(qRgb(255, 255, 200));
    formatTitleChars.setHorizontalAlignment(Format::AlignHCenter);
    formatTitleChars.setVerticalAlignment(Format::AlignVCenter);

    Format FormatTitleColumnChars;
    FormatTitleColumnChars.setHorizontalAlignment(Format::AlignHCenter);
    FormatTitleColumnChars.setVerticalAlignment(Format::AlignVCenter);
    FormatTitleColumnChars.setFontSize(10);
    FormatTitleColumnChars.setPatternBackgroundColor(qRgb(255, 255, 200));
    FormatTitleColumnChars.setTopBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setBottomBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setLeftBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setRightBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setFont(font);
    FormatTitleColumnChars.setFontSize(10);
    FormatTitleColumnChars.setPatternBackgroundColor(qRgb(255, 255, 255));

    FormatTitleColumnChars.setBottomBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setLeftBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setRightBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setTopBorderStyle(Format::BorderThin);
    Format rowFormatFunctions;
    rowFormatFunctions.setPatternBackgroundColor(qRgb(255, 200, 200));
    rowFormatFunctions.setFont(font);
    rowFormatFunctions.setFontSize(13);
    rowFormatFunctions.setBottomBorderStyle(QXlsx::Format::BorderThin);

    rowFormatFunctions.setTopBorderStyle(QXlsx::Format::BorderThin);
    int excel_row = 4;
    for (const auto& fun : CurrentTF.FunctionsInFile) {
        excelScenarioSheet.setRowFormat(excel_row, excel_row, rowFormatFunctions);
        excelScenarioSheet.write(excel_row, 1, "FUNCTION");
        excelScenarioSheet.write(excel_row, 2, QString::fromStdString(fun.name));

        excel_row++;
        for (const auto& instr : fun.InstructionsInFunction) {
            excelScenarioSheet.write(excel_row, 1, "Location");
            excelScenarioSheet.write(excel_row + 1, 1, instr->get_addr_instr());
            int col = 0;
            instr->WriteXLSX(excelScenarioSheet, CurrentTF.FunctionsInFile, excel_row, col);
            excel_row += 2;
        }
    }

    QString xlsx_output_file = QDir::toNativeSeparators(output_folder + "/" + filename);

    display_text("File " + xlsx_output_file + " created.");
    QDir dir(output_folder);
    if (!dir.exists()) dir.mkpath(".");
    excelScenarioSheet.saveAs(xlsx_output_file);
    return true;
}

bool Decompiler::CheckAllFiles(QStringList filesToRead,
                               const QString& folder_for_reference,
                               const QString& folder_for_generated_files,
                               const QString& output_folder) {
    QFile file("C:\\Users\\Antoine\\Desktop\\log.txt");

    QTextStream stream(&file);
    file.remove();
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    foreach (QString file_, filesToRead) {

        QString full_path = folder_for_reference + "/" + file_;
        QString filename = full_path.mid(full_path.lastIndexOf("/"));
        QString croped_fileName = filename.section(".", 0, 0);
        qDebug() << "Checking " << full_path;
        QString full_path_ref = folder_for_reference + filename;
        stream << full_path << "\n";
        this->SetupGame("CS4");
        qDebug() << "reading dat1 file" << full_path;
        this->ReadFile(full_path);
        qDebug() << "reading dat done." << full_path;
        this->WriteXLSX(output_folder);
        qDebug() << "reading xlsx file" << folder_for_generated_files + croped_fileName + ".xlsx";
        this->ReadFile(folder_for_generated_files + croped_fileName + ".xlsx");
        qDebug() << "writing dat file";
        this->WriteDAT(output_folder);
        qDebug() << "full done";
        qDebug() << "reading dat file" << folder_for_generated_files + "/recompiled_files" + filename;
        qDebug() << "reading dat file" << full_path_ref;
        QFile file1(folder_for_generated_files + "/release/recompiled_files" + filename);
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
        int ref_size = content2.size();
        for (int i = 0; i < ref_size; i++) {
            if (content1[i] != content2[i]) {
                stream << "Mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
                qDebug() << "Mismatch at " << Qt::hex << i << " " << (int)content1[i] << " should be " << (int)content2[i] << "\n";
            }
        }
        if (content1.size() < ref_size) {
            qFatal("size too short");
            stream << "size too short"
                   << "\n";
        } else {

            if (content1.size() > ref_size) {
                qFatal("size too big");
                stream << "too big"
                       << "\n";
            }
        }
        stream << " Size 1: " << Qt::hex << content1.size() << " vs Size 2: " << Qt::hex << content2.size();
    }
    return true;
}
bool Decompiler::ReadFile(const QString& filepath) {
    IB->Reset();
    CurrentTF = TranslationFile();
    QFile file(filepath);
    QFileInfo infoFile(file);

    if (infoFile.suffix() == "xlsx") {
        ReadXLSX(file);
    } else if (infoFile.suffix() == "dat") {
        ReadDAT(file);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
bool Decompiler::WriteFile(const QString& filepath, const QString& output_folder) {

    QFile file(filepath);
    QFileInfo infoFile(file);
    if (infoFile.suffix() == "dat") {
        WriteXLSX(output_folder);
    } else if (infoFile.suffix() == "xlsx") {
        WriteDAT(output_folder);
    } else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
TranslationFile Decompiler::GetTF() { return CurrentTF; }
