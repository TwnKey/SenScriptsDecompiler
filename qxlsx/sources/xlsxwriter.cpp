#include "header/xlsxwriter.h"
#include "header/XLSXParser.h"
#include "header/xlsxdocument.h"
#include "header/xlsxchartsheet.h"
#include "header/xlsxcellrange.h"
#include "header/xlsxchart.h"
#include "header/xlsxrichstring.h"
#include "header/xlsxworkbook.h"
#include "header/xlsxformat.h"
#include <QColor>
#include <QDebug>
#include <QCoreApplication>
using namespace QXlsx;
QString Offset_Column = "H";
QString Original_EN_Column = "E";
QString Original_JP_Column = "D";
QString Translation_Column = "F";
QString Correction_Column = "G";
QString Name_EN_Column = "C";
QString Name_JP_Column = "B";
QString CH_Column = "A";

XLSXWriter::XLSXWriter()
{

}
XLSXWriter::XLSXWriter(Console *c)
{
    console = c;
}
void XLSXWriter::addOffsetsToExistingSheet(QFileInfo Xlsx_FileInfo, TranslationFile TF){
    Document doc(Xlsx_FileInfo.absoluteFilePath());
    auto it = TF.charInScenes.begin();
    int cntRow = 6;
    int offset;

    QVector<DialogLine> dls = TF.getDLs();
    if (it != TF.charInScenes.end()){
        it++;
        while (it != TF.charInScenes.end()){
            offset = it->second.offsetInFile;
            doc.write(Offset_Column+QString::number(cntRow), offset);
            it++;
            cntRow++;

        }
    }
    QVariant content = doc.read(Translation_Column+QString::number(cntRow));
    while (content.toString() != "TRADUCTION") {
        cntRow++;
        content = doc.read(Translation_Column+QString::number(cntRow));
    }
    cntRow++;
    for (int idx_text = cntRow; idx_text -cntRow  < dls.size(); idx_text++){
        doc.write(Offset_Column+QString::number(idx_text),dls[idx_text-cntRow].getOffset());
        //doc.write("G"+QString::number(idx_text),dls[idx_text-cntRow].getText());
    }
    doc.setColumnHidden(6, true);
    QDir parent_folder = Xlsx_FileInfo.absoluteDir();
    QString output_folder = parent_folder.absolutePath() + "/output/";

    if (!QDir(output_folder).exists()) QDir().mkdir(output_folder);
    doc.saveAs(output_folder+Xlsx_FileInfo.baseName()+".xlsx");
}
void XLSXWriter::WritingDT(DTFile DT, QString Folder)
{
    QDir dir(Folder);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    QFont font = QFont("Arial");
    QString filename = Folder+"/"+DT.getOriginalName() + ".xlsx";
    QXlsx::Document excelScenarioSheet;
    Format format;

    format.setFont(font);
    format.setFontBold(true);
    format.setPatternBackgroundColor(qRgb(255,255,200));
    QColor FontColor = QColor(qRgb(255,0,0));
    format.setFontColor(FontColor);
    format.setFontSize(14);
    excelScenarioSheet.write("A1", DT.getOriginalName() + " - " + DT.getCharName(), format);
    Format formatLocation;

    formatLocation.setFont(font);
    formatLocation.setFontSize(10);
    formatLocation.setPatternBackgroundColor(qRgb(255,255,200));


    Format rowFormat;
    rowFormat.setFillPattern(Format::PatternSolid);
    rowFormat.setPatternBackgroundColor(qRgb(255,255,200));
    rowFormat.setFont(font);
    rowFormat.setFontSize(10);
    //rowFormat.setPatternForegroundColor(qRgb(255,255,0));
    //rowFormat.setBottomBorderStyle(Format::BorderMedium);
    excelScenarioSheet.setRowFormat(3, 3, rowFormat);
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
    std::map<int, QString> charLocations;


    int rowID;


    rowID = 4;

    formatTitleChars.setFontSize(12);
    formatTitleChars.setFontColor(FontColor);
    formatTitleChars.setFontBold(true);


    excelScenarioSheet.write("A"+QString::number(rowID), "ORIGINAL",formatTitleChars);
    excelScenarioSheet.write("B"+QString::number(rowID), "TRADUCTION",formatTitleChars);

    FormatTitleColumnChars.setHorizontalAlignment(Format::AlignHGeneral);
    FormatTitleColumnChars.setTextWrap(true);
    Format FormatNames;
    FormatNames.setHorizontalAlignment(Format::AlignHCenter);
    FormatNames.setVerticalAlignment(Format::AlignVCenter);

    FormatNames.setBottomBorderStyle(Format::BorderThin);
    FormatNames.setLeftBorderStyle(Format::BorderThin);
    FormatNames.setRightBorderStyle(Format::BorderThin);
    FormatNames.setTopBorderStyle(Format::BorderThin);
    FormatNames.setPatternBackgroundColor(qRgb(255,255,200));
    FormatNames.setFont(font);
    FormatNames.setFontSize(10);
    FormatNames.setFontBold(true);
    DTTextLine DL;
    int row_IDX_DialogLines;
    int row_IDX_Image;
    Format ImageFormat = FormatTitleColumnChars;

    ImageFormat.setPatternBackgroundColor(qRgb(255,255,200));
    qDebug()<<" filename: " << filename;
    qDebug()<<" DT.GetNbLines(): " << DT.GetNbLines();
    for (int i=0; i<DT.GetNbLines();i++){
        row_IDX_DialogLines = rowID + i + 1;
        DL = *DT.getTextLine(i);
        QString whatToActuallyPrint;
        excelScenarioSheet.write("A"+QString::number(row_IDX_DialogLines), DL.text,FormatTitleColumnChars);
        excelScenarioSheet.write("B"+QString::number(row_IDX_DialogLines), "",FormatTitleColumnChars);
    }


    excelScenarioSheet.autosizeColumnWidth("A"+QString::number(1)+":A"+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth("B"+QString::number(1)+":B"+QString::number(row_IDX_DialogLines));

    excelScenarioSheet.setRowFormat(1, 2, rowFormat);
    excelScenarioSheet.setColumnWidth(2, 20);
    excelScenarioSheet.setColumnWidth(3, 60);
    excelScenarioSheet.setColumnWidth(4, 60);
    excelScenarioSheet.setColumnWidth(5, 60);
    excelScenarioSheet.saveAs(filename);


}

void XLSXWriter::Writing(TranslationFile TFOrig, TranslationFile TFEdited, QString Folder)
{
    QDir dir(Folder);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    QFont font = QFont("Arial");
    QString filename = Folder+"/"+TFOrig.getSceneName() + ".xlsx";
    QXlsx::Document excelScenarioSheet;
    Format format;

    format.setFont(font);
    format.setFontBold(true);
    QColor DarkYellow = QColor(qRgb(255,222,155));
    format.setPatternBackgroundColor(DarkYellow);
    QColor FontColor = QColor(qRgb(255,0,0));

    format.setFontColor(FontColor);
    format.setFontSize(14);
    excelScenarioSheet.write("A1", TFOrig.getSceneName(), format);
    Format formatLocation;

    formatLocation.setFont(font);
    formatLocation.setFontSize(10);
    formatLocation.setPatternBackgroundColor(DarkYellow);
    excelScenarioSheet.write("A2", TFOrig.GetFullLocation(),formatLocation);
    excelScenarioSheet.write("A3", QString::number(TFOrig.GetNbLines()) + " rows");

    Format rowFormat;
    rowFormat.setFillPattern(Format::PatternSolid);
    rowFormat.setPatternBackgroundColor(qRgb(255,255,200));
    rowFormat.setFont(font);
    rowFormat.setFontSize(10);
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
    excelScenarioSheet.write(Original_JP_Column+"5", "Nom original JP",FormatTitleColumnChars);
    excelScenarioSheet.write(Original_EN_Column+"5", "Nom original EN",FormatTitleColumnChars);
    excelScenarioSheet.write(Translation_Column+"5", "Nom traduit",FormatTitleColumnChars);

    FormatTitleColumnChars.setPatternBackgroundColor(qRgb(255,255,255));

    FormatTitleColumnChars.setBottomBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setLeftBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setRightBorderStyle(Format::BorderThin);
    FormatTitleColumnChars.setTopBorderStyle(Format::BorderThin);
    std::map<int, QString> charLocations;

    //formatTitleChars.setTopBorderStyle(Format::BorderMedium);
    int cnt = 0;
    int rowID;
    bool separator = false;
    int lines_per_dialog = 1;
    std::map<int,Character>::iterator it = TFOrig.charInScenes.begin();
    std::map<int,Character>::iterator itEdited = TFEdited.charInScenes.begin();
    qDebug() << " starting writing characters";
    //ici on part du principe qu'on a forcément + de trucs à mettre que la version traduite qui en a potentiellement moins;
    //donc on descend bien toute la liste de perso, et pour chaque perso, on regarde si on a une version traduite
    //sachant que les deux maps sont triées
    if (it!=TFOrig.charInScenes.end()){
        it++;
        itEdited++;
        for (it;it!=TFOrig.charInScenes.end();it++){
           rowID = 6+cnt;
           QString leftColumnName = Original_JP_Column+QString::number(rowID);
           QString midColumnName = Original_EN_Column+QString::number(rowID);
           QString rightColumnName = Translation_Column+QString::number(rowID);
           QString offsetColumnName = Offset_Column+QString::number(rowID);
           charLocations[it->second.instanceID] = QString::number(rowID);
           excelScenarioSheet.write(offsetColumnName, it->second.offsetInFile,FormatTitleColumnChars);
           excelScenarioSheet.write(leftColumnName, it->second.name_JP,FormatTitleColumnChars);
           excelScenarioSheet.write(midColumnName, it->second.name,FormatTitleColumnChars);

           while (itEdited != TFEdited.charInScenes.end()){

               if (itEdited->second.offsetInFile < it->second.offsetInFile) { itEdited++;}

               else break;
           }


           if (itEdited != TFEdited.charInScenes.end()){
               if (itEdited->second.offsetInFile == it->second.offsetInFile){
                    excelScenarioSheet.write(rightColumnName, itEdited->second.name,FormatTitleColumnChars);
               }
               else excelScenarioSheet.write(rightColumnName, it->second.name,FormatTitleColumnChars);

           }
           else excelScenarioSheet.write(rightColumnName, it->second.name,FormatTitleColumnChars);


        cnt++;
     }
    }
    rowID = 6+cnt;

    Format bottomFormat = Format(rowFormat);
    Format Other = Format(rowFormat);
    bottomFormat.setFillPattern(Format::PatternSolid);
    bottomFormat.setPatternBackgroundColor(DarkYellow);
    bottomFormat.setFont(font);
    bottomFormat.setFontSize(10);
    Other.setPatternBackgroundColor(DarkYellow);
    excelScenarioSheet.mergeCells(Offset_Column+"4:O4", Other);
    bottomFormat.setBottomBorderStyle(Format::BorderThin);
    excelScenarioSheet.setColumnFormat(CH_Column+"5:"+Name_EN_Column+QString::number(rowID),rowFormat);
    excelScenarioSheet.setColumnFormat(Correction_Column+"5:E"+QString::number(rowID),rowFormat);
    excelScenarioSheet.mergeCells(CH_Column+"4:"+Name_EN_Column+"4", Other);


    excelScenarioSheet.write(Correction_Column+"4", "", bottomFormat);
    excelScenarioSheet.write(Original_JP_Column+QString::number(rowID),"Personnages principaux",formatTitleChars);
    excelScenarioSheet.setRowHidden(rowID, true);
    cnt++;

    separator = true;
    for (std::map<int,Character>::iterator it=TFOrig.mainCharactersTable.begin();it!=TFOrig.mainCharactersTable.end();++it){
        rowID = 6+cnt;
        QString leftColumnName = Original_JP_Column+QString::number(rowID);
        QString midColumnName = Original_EN_Column+QString::number(rowID);
        QString rightColumnName = Translation_Column+QString::number(rowID);
        charLocations[it->second.instanceID] = QString::number(rowID);

        excelScenarioSheet.write(leftColumnName, "",FormatTitleColumnChars);
        excelScenarioSheet.write(midColumnName, it->second.name,FormatTitleColumnChars);
        excelScenarioSheet.write(rightColumnName, it->second.name,FormatTitleColumnChars);
        excelScenarioSheet.setRowHidden(rowID, true);

        cnt++;
    }




    rowID = 6+cnt;
    formatTitleChars.setFontSize(12);
    formatTitleChars.setFontColor(FontColor);
    formatTitleChars.setFontBold(true);

    formatTitleChars.setBottomBorderStyle(Format::BorderThin);
    formatTitleChars.setLeftBorderStyle(Format::BorderThin);
    formatTitleChars.setRightBorderStyle(Format::BorderThin);
    formatTitleChars.setTopBorderStyle(Format::BorderThin);
    excelScenarioSheet.mergeCells(CH_Column+QString::number(rowID)+":"+Name_EN_Column+QString::number(rowID), formatTitleChars);
    excelScenarioSheet.mergeCells(Original_JP_Column+"4:"+Translation_Column+"4",formatTitleChars);
    excelScenarioSheet.write(Original_JP_Column+"4", "PNJs",formatTitleChars);
    excelScenarioSheet.write(CH_Column+QString::number(rowID), "PERSONNAGE",formatTitleChars);
    excelScenarioSheet.mergeCells(Original_JP_Column+QString::number(rowID)+":"+Original_EN_Column+QString::number(rowID), formatTitleChars);
    excelScenarioSheet.write(Original_JP_Column+QString::number(rowID), "ORIGINAL",formatTitleChars);
    excelScenarioSheet.write(Translation_Column+QString::number(rowID), "TRADUCTION",formatTitleChars);
    excelScenarioSheet.write(Correction_Column+QString::number(rowID), "CORRECTION",formatTitleChars);
    excelScenarioSheet.setRowHeight(rowID, 30.0);
    FormatTitleColumnChars.setHorizontalAlignment(Format::AlignHGeneral);
    FormatTitleColumnChars.setTextWrap(true);
    Format FormatNames;
    FormatNames.setHorizontalAlignment(Format::AlignHCenter);
    FormatNames.setVerticalAlignment(Format::AlignVCenter);

    FormatNames.setBottomBorderStyle(Format::BorderThin);
    FormatNames.setLeftBorderStyle(Format::BorderThin);
    FormatNames.setRightBorderStyle(Format::BorderThin);
    FormatNames.setTopBorderStyle(Format::BorderThin);
    FormatNames.setPatternBackgroundColor(qRgb(255,255,200));
    FormatNames.setFont(font);
    FormatNames.setFontSize(12);
    FormatNames.setFontBold(true);
    DialogLine DL, DLEdited;
    int row_IDX_DialogLines;
    int row_IDX_Image;
    Format ImageFormat = FormatTitleColumnChars;

    ImageFormat.setPatternBackgroundColor(qRgb(255,255,200));
    int idx_Edited = 0;

    for (int i=0; i<TFOrig.GetNbLines();i++){

        QColor normalColor = FormatNames.fontColor();
        row_IDX_DialogLines = rowID + i + 1;
        row_IDX_Image = rowID + 1 + i*(lines_per_dialog);

        DL = *TFOrig.GetLine(i);


        QString pathImage = TFOrig.getChipPNGPath(DL.getCharacterID());
        QString completepath = QCoreApplication::applicationDirPath() + "/"+pathImage;
        QImage *CHImage = new QImage(completepath);

        bool load = excelScenarioSheet.insertImage(row_IDX_Image-1, 0, *CHImage);

        QString locationOfCharacter;
        QString whatToActuallyPrint,whatToActuallyPrintJP;


        if ((DL.getType()==LineType::Anon)||(DL.getType() == LineType::NPC)){

            whatToActuallyPrint = DL.getCharacterName();
            whatToActuallyPrintJP = DL.getCharacterJPName();

            if (DL.getType()==LineType::Anon) FormatNames.setFontColor(QColor(qRgb(255,0,255)));
            else  FormatNames.setFontColor(QColor(qRgb(255,0,0)));
            excelScenarioSheet.write(Name_JP_Column+QString::number(row_IDX_Image), whatToActuallyPrintJP,FormatNames);
            excelScenarioSheet.write(Name_EN_Column+QString::number(row_IDX_Image), whatToActuallyPrint,FormatNames);


        }

        else{



            locationOfCharacter = charLocations[DL.getCharacterInstanceID()];
            whatToActuallyPrint = "="+Original_EN_Column+locationOfCharacter;
            whatToActuallyPrintJP = "="+Original_JP_Column+locationOfCharacter;
            if (locationOfCharacter == 0) {
                if(DL.getCharacterName()!= "") {
                    whatToActuallyPrint = DL.getCharacterName();
                    whatToActuallyPrintJP = DL.getCharacterJPName();
                }
                else {
                    whatToActuallyPrint = "INCONNU";
                    whatToActuallyPrintJP = "INCONNU";
                }
            }
            excelScenarioSheet.write(Name_EN_Column+QString::number(row_IDX_Image), whatToActuallyPrint,FormatNames);
            excelScenarioSheet.write(Name_JP_Column+QString::number(row_IDX_Image), whatToActuallyPrintJP,FormatNames);
        }


        excelScenarioSheet.setRowHeight(row_IDX_Image, 100.0);


        excelScenarioSheet.write(Original_JP_Column+QString::number(row_IDX_Image), DL.getJPText(),FormatTitleColumnChars);
        excelScenarioSheet.write(Original_EN_Column+QString::number(row_IDX_Image), DL.getText(),FormatTitleColumnChars);
        excelScenarioSheet.write(Offset_Column+QString::number(row_IDX_Image), DL.getOffset(),FormatTitleColumnChars);
        excelScenarioSheet.write(Correction_Column+QString::number(row_IDX_Image), "",FormatTitleColumnChars);
        //DLEdited = *TFEdited.GetLine(idx_Edited);

        while (idx_Edited < TFEdited.GetNbLines()){

            DLEdited = *TFEdited.GetLine(idx_Edited);

            if (DLEdited.getOffset() < DL.getOffset()) idx_Edited++;
            else break;
        }


        if (idx_Edited < TFEdited.GetNbLines()){

            DLEdited = *TFEdited.GetLine(idx_Edited);

            if (DLEdited.getOffset() == DL.getOffset()){
                QString translatedLine = DLEdited.getText();

                translatedLine.replace(" \n", "\n");
                translatedLine.replace("\n ", "\n");
                if (translatedLine.size() != 0){
                    excelScenarioSheet.write(Translation_Column+QString::number(row_IDX_Image), translatedLine,FormatTitleColumnChars);
                    if (DL.getType() == LineType::NPC){
                        excelScenarioSheet.write(Name_EN_Column+QString::number(row_IDX_Image), DLEdited.getCharacter().name,FormatNames);
                    }
                }
                else{
                    excelScenarioSheet.write(Translation_Column+QString::number(row_IDX_Image), "",FormatTitleColumnChars);
                }
            }
            else excelScenarioSheet.write(Translation_Column+QString::number(row_IDX_Image), "",FormatTitleColumnChars);

        }
        else excelScenarioSheet.write(Translation_Column+QString::number(row_IDX_Image), "",FormatTitleColumnChars);
        FormatNames.setFontColor(normalColor);
    }


    excelScenarioSheet.autosizeColumnWidth(CH_Column+QString::number(1)+":"+CH_Column+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth(Name_EN_Column+QString::number(1)+":"+Name_EN_Column+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth(Name_JP_Column+QString::number(1)+":"+Name_JP_Column+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth(Original_JP_Column+QString::number(1)+":"+Original_JP_Column+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth(Original_EN_Column+QString::number(1)+":"+Original_EN_Column+QString::number(row_IDX_DialogLines));
    excelScenarioSheet.autosizeColumnWidth(Translation_Column+QString::number(1)+":"+Translation_Column+QString::number(row_IDX_DialogLines));

    excelScenarioSheet.setRowFormat(1, 2, Other);
    excelScenarioSheet.setColumnWidth(2, 25);
    excelScenarioSheet.setColumnWidth(3, 25);
    excelScenarioSheet.setColumnWidth(4, 50);
    excelScenarioSheet.setColumnWidth(5, 50);
    excelScenarioSheet.setColumnWidth(6, 50);
    excelScenarioSheet.setColumnWidth(7, 50);

    excelScenarioSheet.setColumnHidden(8, true);
    if (!TFOrig.getJapaneseGood()) {
        excelScenarioSheet.setColumnHidden(2, true);
        excelScenarioSheet.setColumnHidden(4, true);
    }
    qDebug() << "Saving..." << filename;
    excelScenarioSheet.saveAs(filename);


}
