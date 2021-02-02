#include <QCoreApplication>

#include "headers/decompiler.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filepath = "C:/Users/Antoine/Desktop/m3040.dat";
    QString filepath_xlsx = "C:/Users/Antoine/Documents/build-CS3TextDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Debug/m3040.xlsx";
    Decompiler Dc;
    Dc.SetupGame("CS3");
    Dc.ReadFile(filepath);
    Dc.WriteXLSX();
    Dc.ReadFile(filepath_xlsx);
    Dc.WriteDAT();

    return a.exec();
}
