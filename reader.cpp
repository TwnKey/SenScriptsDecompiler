#include "reader.h"
#include "utilities.h"

Reader::Reader()
{

}
Reader::Reader(QString Game){
    if (Game == "CS3") IB = new CS3InstructionBuilder();
    else out << "FAILURE: Unrecognized game specified." << endl;

}
void Reader::AddInstructionToCurrentFile(std::shared_ptr<Instruction> instr){
    CurrentTF.AddInstruction(instr);
}
bool Reader::ReadXLSX(QFile &File){
//File is a XLSX document
    return true;
}
bool Reader::ReadDAT(QFile &File){
    //File is a binary file
    //This function is going to read each instruction in the binary file and add them to the TranslationFile it is holding.


    /*currentFile = TranslationFile();
    for (std::map<int,Character>::iterator it=mainCharacterTable.begin();it!=mainCharacterTable.end();++it){
        currentFile.addMainChar(it->second);
    }
    if (!Sc_File.open(QIODevice::ReadOnly)) return false;
    currentFile.extension = "dat";
    content = Sc_File.readAll();
    QByteArray file_name;
    int idx_lecture = 0x14;

    int nb_global_functions = readInteger(idx_lecture,content);
    qDebug() << "Number of global functions found: "<<nb_global_functions;

    idx_lecture = 0x20;
    while (content.at(idx_lecture)!= 0){
        unsigned char letter = content.at(idx_lecture);
        if ((letter<0x20)||(letter>0x7F)) {
            Sc_File.close();
            return false;
        }
        file_name.push_back(letter);
        idx_lecture++;

    }
    idx_lecture++;
    QString str_Name = file_name;

    if (talk_files.find(str_Name) != talk_files.end()) currentTalkingChar = talk_files[str_Name];
    currentFile.setSceneName(str_Name);
    QFile file("C:\\Users\\Antoine\\Desktop\\decompiled_"+str_Name+".txt");
    //QFile file("C:\\Users\\Antoine\\Desktop\\log_failed.txt");
    // We're going to streaming text to the file
    QTextStream stream(&file);
    //file.remove();
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);



    stream << str_Name << "\n";
    int idx_start = idx_lecture;
    qDebug() << "reading global functions...";
    bool success = true;
    for (int idx_fun = 0; idx_fun < nb_global_functions; idx_fun++){
        //currentTalkingChar.name = "";
        int location = readInteger(idx_lecture,content);
        int next_location;
        if (idx_fun < nb_global_functions - 1) next_location = readInteger(idx_lecture+4,content);
        else next_location = content.size();
        QByteArray addr = content.mid(idx_lecture,4);
        currentFile.addPointer(Pointer(addr, idx_lecture));
        stream << "Function_"+idx_fun << "found at " << hex << location << "\n" << "\n";
        int locationName = readUInt16(nb_global_functions * 4 + idx_start + 2 * idx_fun,content);
        QByteArray nameFun = readArray(locationName, content);
        stream << "Name: " << QString(nameFun) << "\n" ;
        qDebug() << "Name: " << QString(nameFun) << "\n" ;
        if (tk_function_table.find(nameFun) != tk_function_table.end()) {
            currentTalkingChar = tk_function_table[nameFun];
            stream << "Talking Char: " << currentTalkingChar.name << "\n" ;
        }
        else currentTalkingChar.name = "";
        qDebug() << "Talking Char: " << currentTalkingChar.name << "\n" ;
        currentFunction = QString(nameFun);
        addr_functions[location] = nameFun;
        success = readGlobalfun(location, stream, InstructionsSet, next_location);
        if (!success) break;
        idx_lecture+=4;



    }



    currentFile.setNames(mainCharacterTable);
    qDebug() << " ALL DONE! ";
    file.close();

    currentFile.setRawContent(content);
    currentFile.uniqueDLs();
    currentFile.uniqueChars();*/
    return true;
}
bool Reader::ReadFile(QString filepath){
    QFile file(filepath);
    QFileInfo infoFile(file);
    if (infoFile.suffix()=="xlsx") ReadXLSX(file);
    else if (infoFile.suffix()=="dat") ReadDAT(file);
    else {
        out << "FAILURE: Unrecognized extension." << endl;
        return false;
    }
    return true;
}
TranslationFile Reader::GetTF(){
    return CurrentTF;
}
