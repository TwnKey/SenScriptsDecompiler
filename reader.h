#ifndef READER_H
#define READER_H
#include "CS3InstructionsSet.h"
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

    void AddInstructionToCurrentFile(std::shared_ptr<Instruction> instr);
    TranslationFile GetTF();

private:
    TranslationFile CurrentTF;
    Builder *IB;
};

#endif // READER_H
