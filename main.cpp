#include <QCoreApplication>
#include "CS3InstructionsSet.h"
#include "reader.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filepath = "C:/Users/Antoine/Desktop/m3040.dat";
    Reader Rd("CS3");
    Rd.ReadFile(filepath);
    std::shared_ptr<Instruction> instr = std::make_unique<OPCode0>();
    instr->WriteDat();
    instr->WriteXLSX();
    return a.exec();
}
