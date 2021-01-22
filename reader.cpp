#include "reader.h"

Reader::Reader()
{

}
Reader::Reader(QString Game){
    if (Game == "CS3") IB = new CS3Builder();
    else display_text("FAILURE: Unrecognized game specified.");

}
void Reader::AddInstructionToCurrentFile(std::shared_ptr<Instruction> instr){
    CurrentTF.AddInstruction(instr);
}
bool Reader::ReadXLSX(QFile &File){
//File is a XLSX document
    return true;
}
bool Reader::ReadDAT(QFile &File){

    return true;
}
bool Reader::ReadFile(QString filepath){
    QFile file(filepath);
    QFileInfo infoFile(file);
    if (infoFile.suffix()=="xlsx") ReadXLSX(file);
    else if (infoFile.suffix()=="dat") ReadDAT(file);
    else {
        display_text("FAILURE: Unrecognized extension.");
        return false;
    }
    return true;
}
TranslationFile Reader::GetTF(){
    return CurrentTF;
}
