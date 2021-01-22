#include "translationfile.h"

TranslationFile::TranslationFile()
{

}
void TranslationFile::AddInstruction(std::shared_ptr<Instruction> instr){
    InstructionsInFile.push_back(instr);
}
