#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H
#include "CS3InstructionsSet.h"


class TranslationFile
{
public:
    TranslationFile();
    void AddInstruction(std::shared_ptr<Instruction> instr);
protected:
    std::vector<std::shared_ptr<Instruction>> InstructionsInFile;
};

#endif // TRANSLATIONFILE_H
