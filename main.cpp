#include <QCoreApplication>

#include "reader.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filepath = "C:/Users/Antoine/Desktop/m3040.dat";
    Reader Rd("CS3");
    Rd.ReadFile(filepath);

    return a.exec();
}
