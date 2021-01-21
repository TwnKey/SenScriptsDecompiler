#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H
#include "CS3InstructionsSet.h"


class TranslationFile
{
public:
    TranslationFile();
    std::unique_ptr<Instruction> CreateInstructionFromXLSX(int row);
    std::unique_ptr<Instruction> CreateInstructionFromDAT(int addr);
private:
    std::vector<std::unique_ptr<Instruction>> InstructionsInFile;
};

#endif // TRANSLATIONFILE_H
