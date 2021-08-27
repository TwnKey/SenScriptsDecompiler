#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H

#include "headers/functions.h"

class TranslationFile {
  public:
    TranslationFile();

    void setName(QString name);
    [[nodiscard]] int getNbFunctions() const;
    QString getName();
    void addFunction(function fun);
    std::vector<function> FunctionsInFile;

  protected:
    QString SceneName;
};

#endif // TRANSLATIONFILE_H
