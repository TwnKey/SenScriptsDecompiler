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
    Decompiler(QString Game);
    bool ReadXLSX(QFile &File);
    bool ReadDAT(QFile &File);
    bool ReadFile(QString filepath);
    bool UpdateCurrentTF();
    bool WriteXLSX();
    TranslationFile GetTF();


private:
    TranslationFile CurrentTF;
    QString Game;
    Builder *IB;
};

#endif // READER_H
