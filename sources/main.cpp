#include <QCoreApplication>

#include "headers/decompiler.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filepath = "C:/Users/Antoine/Desktop/m3040.dat";
    Decompiler Dc();
    Dc.SetupGame("CS3");
    Dc.ReadFile(filepath);
    Dc.WriteXLSX();
    return a.exec();
}
