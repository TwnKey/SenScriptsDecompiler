#include "translationfile.h"

TranslationFile::TranslationFile()
{

}
void TranslationFile::AddInstruction(std::shared_ptr<Instruction> instr){
    InstructionsInFile.push_back(instr);
}
void TranslationFile::setName(QString name){
    SceneName = name;
}
QString TranslationFile::getName(){
    return SceneName;
}
