#include <QCoreApplication>
#include "CS3InstructionsSet.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::unique_ptr<Instruction> instr = std::make_unique<OPCode0>();
    instr->WriteDat();
    instr->WriteXLSX();
    return a.exec();
}
