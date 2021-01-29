#include "reader.h"
#include "CS3InstructionsSet.h"
Reader::Reader()
{

}
Reader::Reader(QString Game){
    if (Game == "CS3") IB = new CS3Builder();
    else display_text("FAILURE: Unrecognized game specified.");

}

bool Reader::ReadXLSX(QFile &File){
//File is a XLSX document
    return true;
}
bool Reader::UpdateCurrentTF(){
    CurrentTF.setName(IB->SceneName);
    for (int idx_fun = 0; idx_fun < IB->FunctionsParsed.size(); idx_fun++) CurrentTF.addFunction(IB->FunctionsParsed[idx_fun]);

    return true;
}
bool Reader::ReadDAT(QFile &File){

    if (!File.open(QIODevice::ReadOnly)) return false;
    QByteArray content = File.readAll();

    IB->CreateHeaderFromDAT(content);
    IB->ReadFunctions(content);
    UpdateCurrentTF();
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
