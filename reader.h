#ifndef READER_H
#define READER_H
#include "translationfile.h"

class Reader
{
public:
    Reader();
    void ReadXLSX();
    void ReadDAT();
    TranslationFile GetTF();
private:
    TranslationFile CurrentTF;
};

#endif // READER_H
