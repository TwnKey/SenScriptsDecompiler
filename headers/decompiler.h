#ifndef READER_H
#define READER_H

#include "headers/translationfile.h"

#include <QFile>
#include <QFileInfo>
#include <QString>

/*
The Decompiler class is here to write the XLSX and DAT output files, as well as setting some global parameters such as
the game name, or run regression tests The point is that through the two reading functions, you will build a Translation
File object that could then be written in both output format

SetupGame: selects the correct Builder depending on which game name is provided to the program
ReadXLSX: calls the builder's reading function for XLSX files, then update the pointers (executes the whole reading
process in fact) ReadDAT: calls the builder's reading function for DAT files, then update the pointers ReadFile: selects
the correct reading function depending on the provided file path UpdateCurrentTF: transfers all functions from the
builder to the translation file object WriteXLSX: writes the translation file to an xlsx spreadsheet WriteDAT: writes
the translation file to a dat file readable by the game CheckAllFiles: runs a regression test GetTF: returns the
translation file being edited WriteFile: writes the correct file depending on the provided file path
*/
class Decompiler {
  public:
    Decompiler();
    bool SetupGame(QString Game);
    bool ReadXLSX(QFile& File);
    bool ReadDAT(QFile& File);
    bool ReadFile(QString filepath);
    bool UpdateCurrentTF();
    bool WriteXLSX(QString output_folder);
    bool WriteDAT(QString output_folder);
    bool CheckAllFiles(QStringList filesToRead, QString folder_for_reference, QString folder_for_generated_files, QString output_folder);
    TranslationFile GetTF();
    bool WriteFile(QString filepath, QString output_folder);

  private:
    TranslationFile CurrentTF;
    QString Game;
    Builder* IB;
};

#endif // READER_H
