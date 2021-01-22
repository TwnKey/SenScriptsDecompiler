#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H
#include "instruction.h"
#include "functions.h"

class TranslationFile
{
public:
    TranslationFile();
    void AddInstruction(std::shared_ptr<Instruction> instr);
    void setName(QString name);
    QString getName();
protected:
    std::vector<std::shared_ptr<Instruction>> InstructionsInFile;
    QString SceneName;
    std::vector<function> FunctionsInFile;
};

#endif // TRANSLATIONFILE_H
