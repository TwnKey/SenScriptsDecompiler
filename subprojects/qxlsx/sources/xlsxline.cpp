#include "xlsxline.h"

XLSXLine::XLSXLine(QString CharName, QString English, QString French, int rownb, int number_of_rows){
    Name = CharName;
    Replique = English;
    TranslatedText = French;
    row_number = rownb;
    if ((CharName == NULL)&&(English == NULL)&&(French == NULL)) EmptyLine = true;
    else EmptyLine = false;
    this->number_of_rows = number_of_rows;
}
QString XLSXLine::toStr(){
    return Name + ": " + Replique + " // " + TranslatedText + "\n";
}
