#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H

#include "headers/functions.h"

class TranslationFile {
  public:
    TranslationFile();

    void setName(std::string name);
    [[nodiscard]] int getNbFunctions() const;
    std::string getName();
    void addFunction(const Function& fun);
    std::vector<Function> FunctionsInFile;

  protected:
    std::string scene_name;
};

#endif // TRANSLATIONFILE_H
