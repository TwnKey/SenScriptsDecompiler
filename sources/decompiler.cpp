#include "headers/decompiler.h"
#include "headers/CS3InstructionsSet.h"
#include "qxlsx/headers/xlsxdocument.h"
#include "qxlsx/headers/xlsxchartsheet.h"
#include "qxlsx/headers/xlsxcellrange.h"
#include "qxlsx/headers/xlsxchart.h"
#include "qxlsx/headers/xlsxrichstring.h"
#include "qxlsx/headers/xlsxworkbook.h"
#include "qxlsx/headers/xlsxformat.h"
#include <QColor>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

using namespace QXlsx;


Decompiler::Decompiler()
{

}

bool Decompiler::SetupGame(QString Game){
    if (Game == "CS3") IB = new CS3Builder();
    else {
        display_text("FAILURE: Unrecognized game specified.");
        return false;
    }
}
bool Decompiler::ReadXLSX(QFile &File){
    QFileInfo info(File);
    if (!File.open(QIODevice::ReadOnly)) return false;

    IB->FunctionsParsed.clear();
    IB->FunctionsToParse.clear();

    TranslationFile ParsedFile = TranslationFile();
    Document doc(info.absoluteFilePath());
    Game = doc.read(1, 1).toString();
    ParsedFile.setName(doc.read(2, 1).toString());

    IB->ReadFunctionsXLSX(doc);
    IB->CreateHeaderFromXLSX(doc);

    UpdateCurrentTF();
    return true;
}

bool Decompiler::UpdateCurrentTF(){
    CurrentTF.setName(IB->SceneName);
    for (int idx_fun = 0; idx_fun < IB->FunctionsParsed.size(); idx_fun++) CurrentTF.addFunction(IB->FunctionsParsed[idx_fun]);

    return true;
}
bool Decompiler::ReadDAT(QFile &File){

    if (!File.open(QIODevice::ReadOnly)) return false;
    QByteArray content = File.readAll();

    IB->CreateHeaderFromDAT(content);
    IB->ReadFunctionsDAT(content);
    UpdateCurrentTF();
    return true;
}
bool Decompiler::WriteDAT(){
    QString folder = QCoreApplication::applicationDirPath() + "/recompiled_files/";
    QByteArray header, functions, file_content;
    /*header creation*/
    QString scene_name = CurrentTF.getName();
    QByteArray scene_name_bytes = scene_name.toUtf8();
    scene_name_bytes.append('\x0');
    int size_of_scene_name = scene_name_bytes.length();
    header.append(GetBytesFromInt(0x20));
    header.append(GetBytesFromInt(0x20));
    header.append(GetBytesFromInt(0x20+size_of_scene_name));
    header.append(GetBytesFromInt(CurrentTF.FunctionsInFile.size()*4));
    header.append(GetBytesFromInt(0x20+size_of_scene_name + CurrentTF.FunctionsInFile.size()*4));
    header.append(GetBytesFromInt(CurrentTF.FunctionsInFile.size()));
    int length_of_names_section = 0;
    for (int idx_fun = 0; idx_fun<CurrentTF.FunctionsInFile.size(); idx_fun++) length_of_names_section = length_of_names_section + CurrentTF.FunctionsInFile[idx_fun].name.toUtf8().length() + 1;
    header.append(GetBytesFromInt(0x20+size_of_scene_name + CurrentTF.FunctionsInFile.size()*4 + CurrentTF.FunctionsInFile.size()*2 + length_of_names_section));
    header.append(GetBytesFromInt(0xABCDEF00));
    header.append(scene_name_bytes);
    QByteArray position_names;
    QByteArray actual_names;
    int offset_names = 0;
    for (int idx_fun = 0; idx_fun<CurrentTF.FunctionsInFile.size(); idx_fun++) {
        header.append(GetBytesFromInt(CurrentTF.FunctionsInFile[idx_fun].actual_addr));
        QByteArray name = CurrentTF.FunctionsInFile[idx_fun].name.toUtf8();
        name.append('\x0');
        position_names.append(GetBytesFromShort(0x20+size_of_scene_name + CurrentTF.FunctionsInFile.size()*4 + CurrentTF.FunctionsInFile.size()*2 + offset_names));
        actual_names.append(name);
        offset_names = offset_names + name.size();
    }
    header.append(position_names);
    header.append(actual_names);
    //Header structure:
    //The first 0x20 is something used to check if the file is a valid file: it will always be that value
    //The second 0x20 is the offset for the file name (shouldn't change too)
    //The next integer is the position of the first pointer
    //The 4th: probably the length in bytes of the pointer section
    //The fifth: probably the position of the "names positions" section (right after the pointer section)
    //The sixth: the number of functions inside the file
    //the seventh: the position one byte after the 0x00 separator at the end of the functions section
    //the eighth: 0xABCDEF00 => seems to always be there (no idea why)
    //Then the name of the file
    //Then the pointer section
    //Then the "names positions" section
    //Then the functions section

    file_content.append(header);
    /*output file creation*/
    QDir dir(folder);
    if (!dir.exists()) dir.mkpath(".");

    QString output_path = CurrentTF.getName() + ".dat";
    QFile file(output_path);
    file.open(QIODevice::WriteOnly);
    file.write(file_content);
    file.close();
    return true;
}
bool Decompiler::WriteXLSX(){
    QFont font = QFont("Arial");
    QString filename = CurrentTF.getName() + ".xlsx";
    QXlsx::Document excelScenarioSheet;
    Format format;
    format.setFont(font);
    format.setFontBold(true);
    QColor DarkYellow = QColor(qRgb(255,222,155));
    format.setPatternBackgroundColor(DarkYellow);
    QColor FontColor = QColor(qRgb(255,0,0));

    format.setFontColor(FontColor);
    format.setFontSize(14);
    excelScenarioSheet.write("A1", Game, format);

    Format formatLocation;

    formatLocation.setFont(font);
    formatLocation.setFontSize(10);
    formatLocation.setPatternBackgroundColor(DarkYellow);
    excelScenarioSheet.write("A2", CurrentTF.getName(), formatLocation);

    Format rowFormat;
    rowFormat.setFillPattern(Format::PatternSolid);
    rowFormat.setPatternBackgroundColor(qRgb(255,255,200));
    rowFormat.setFont(font);
    rowFormat.setFontSize(10);
    excelScenarioSheet.setRowFormat(1, 3, format);
    excelScenarioSheet.setRowFormat(2, 3, format);
    excelScenarioSheet.setRowFormat(3, 3, format);
    rowFormat.setBottomBorderStyle(Format::BorderNone);
    Format formatTitleChars;
    formatTitleChars.setFont(font);
    formatTitleChars.setFontSize(10);
    formatTitleChars.setPatternBackgroundColor(qRgb(255,255,200));
    formatTitleChars.setHorizontalAlignment(Format::AlignHCenter);
    formatTitleChars.setVerticalAlignment(Format::AlignVCenter);

    Format FormatTitleColumnChars;
    FormatTitleColumnChars.setHorizontalAlignment(Format::AlignHCenter);
    FormatTitleColumnChars.setVerticalAlignment(Format::AlignVCenter);
    FormatTitleColumnChars.setFontSize(10);
    FormatTitleColumnChars.setPatternBackgroundColor(qRgb(255,255,200));
    FormatTitleColumnChars.setTopBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setBottomBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setLeftBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setRightBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setFont(font);
    FormatTitleColumnChars.setFontSize(10);
    FormatTitleColumnChars.setPatternBackgroundColor(qRgb(255,255,255));

    FormatTitleColumnChars.setBottomBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setLeftBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setRightBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setTopBorderStyle(Format::BorderThin);
    int excel_row = 4;
    for (int idx_fun=0; idx_fun<CurrentTF.FunctionsInFile.size(); idx_fun++){
        function fun = CurrentTF.FunctionsInFile[idx_fun];
        excelScenarioSheet.write(excel_row,1,"FUNCTION");
        excelScenarioSheet.write(excel_row,2,fun.name);
        excel_row++;
        for (int idx_instr=0; idx_instr<fun.InstructionsInFunction.size(); idx_instr++){
            fun.InstructionsInFunction[idx_instr]->WriteXLSX(excelScenarioSheet,CurrentTF.FunctionsInFile, excel_row);
            excel_row+=2;
        }
    }
    excelScenarioSheet.saveAs(filename);
    return true;
}
bool Decompiler::ReadFile(QString filepath){
    QFile file(filepath);
    QFileInfo infoFile(file);
    if (infoFile.suffix()=="xlsx") ReadXLSX(file);
    else if (infoFile.suffix()=="dat") ReadDAT(file);
    else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
TranslationFile Decompiler::GetTF(){
    return CurrentTF;
}
