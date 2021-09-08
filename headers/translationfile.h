#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H

#include "headers/functions.h"

class TranslationFile {
  public:
    TranslationFile();

    void setName(std::string name);
    [[nodiscard]] int getNbFunctions() const;
    std::string getName();
    void addFunction(const function& fun);
    std::vector<function> FunctionsInFile;

  protected:
    std::string SceneName;
};

#endif // TRANSLATIONFILE_H
