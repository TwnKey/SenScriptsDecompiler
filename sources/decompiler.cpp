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

bool Decompiler::SetupGame(QString Game_){
    Game = Game_;

    if (Game == "CS3") IB = new CS3Builder();
    else {
        display_text("FAILURE: Unrecognized game specified.");
        return false;
    }
    return true;
}
bool Decompiler::ReadXLSX(QFile &File){
    QFileInfo info(File);
    if (!File.open(QIODevice::ReadOnly)) return false;
    qDebug() << "Attempt to read " << info.absoluteFilePath();



    Document doc(info.absoluteFilePath());
    Game = doc.read(1, 1).toString();
    SetupGame(Game);

    qDebug() << "Scene name: " << doc.read(2, 1).toString();
    qDebug() << "Reading functions..." << IB;
    IB->ReadFunctionsXLSX(doc);

    qDebug() << "Done";

    UpdateCurrentTF();
    return true;
}

bool Decompiler::UpdateCurrentTF(){
    CurrentTF.setName(IB->SceneName);



    for (int idx_fun = 0; idx_fun < IB->FunctionsParsed.size(); idx_fun++) CurrentTF.addFunction(IB->FunctionsParsed[idx_fun]);

    return true;
}
bool Decompiler::ReadDAT(QFile &File){

    if (!File.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray content = File.readAll();
    QFileInfo info(File);

    qDebug() << info.absoluteFilePath();
    IB->CreateHeaderFromDAT(content);
    IB->ReadFunctionsDAT(content);
    UpdateCurrentTF();
    return true;
}
bool Decompiler::WriteDAT(){
    QString folder = QCoreApplication::applicationDirPath() + "/recompiled_files/";

    QByteArray functions, file_content;
    /*header creation*/
    QByteArray header = IB->CreateHeaderBytes();
    //Header done; let's do the functions now.
    for (int idx_fun = 0; idx_fun < CurrentTF.FunctionsInFile.size(); idx_fun++) {
        function fun = CurrentTF.FunctionsInFile[idx_fun];

        for (int idx_instr = 0; idx_instr < fun.InstructionsInFunction.size(); idx_instr++) {
            functions.push_back(fun.InstructionsInFunction[idx_instr]->getBytes());
        }
    }

    file_content.append(header);
    file_content.append(functions);
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
    IB->Reset();
    CurrentTF = TranslationFile();
    QFile file(filepath);
    QFileInfo infoFile(file);
    QByteArray kek = file.readAll();
    qDebug() << hex << kek[0];
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
