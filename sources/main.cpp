#include <QCoreApplication>

#include "headers/decompiler.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filepath = "C:/Users/Antoine/Desktop/m3040.dat";
    QString filepath_xlsx = "C:/Users/Antoine/Documents/build-CS3TextDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Debug/m3040.xlsx";
    Decompiler Dc;
    QDir directory("C:/Users/Antoine/Desktop/dat_en/");
    QStringList filesToConvert = directory.entryList(QStringList() << "*.dat",QDir::Files);
    Dc.CheckAllFiles(filesToConvert,"C:/Users/Antoine/Desktop/dat_en/","C:/Users/Antoine/Documents/build-CS3TextDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Debug/");




    /*QString filename = "a0302";

    Dc.SetupGame("CS3");
    Dc.ReadFile("C:/Users/Antoine/Desktop/dat_en/"+filename+".dat");
    Dc.WriteXLSX();

    Dc.ReadFile("C:/Users/Antoine/Documents/build-CS3TextDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Debug/"+filename+".xlsx");
    Dc.WriteDAT();*/

    return a.exec();
}
