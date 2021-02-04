#ifndef READER_H
#define READER_H

#include "headers/translationfile.h"

#include <QFile>
#include <QFileInfo>
#include <QString>
class Decompiler
{
public:
    Decompiler();
    bool SetupGame(QString Game);
    bool ReadXLSX(QFile &File);
    bool ReadDAT(QFile &File);
    bool ReadFile(QString filepath);
    bool UpdateCurrentTF();
    bool WriteXLSX();
    bool WriteDAT();
    bool CheckAllFiles(QStringList filesToRead, QString folder_for_reference, QString folder_for_generated_files);
    TranslationFile GetTF();


private:
    TranslationFile CurrentTF;
    QString Game;
    Builder *IB;
};

#endif // READER_H
