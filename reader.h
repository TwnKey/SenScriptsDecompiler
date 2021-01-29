#ifndef READER_H
#define READER_H

#include "translationfile.h"

#include <QFile>
#include <QFileInfo>
#include <QString>
class Reader
{
public:
    Reader();
    Reader(QString Game);
    bool ReadXLSX(QFile &File);
    bool ReadDAT(QFile &File);
    bool ReadFile(QString filepath);
    bool UpdateCurrentTF();
    TranslationFile GetTF();

private:
    TranslationFile CurrentTF;
    Builder *IB;
};

#endif // READER_H
