#ifndef SCRIPTDATA_H
#define SCRIPTDATA_H

#include "headers/functions.h"

class ScriptData {
  public:
    ScriptData();

    void setName(std::string name);
    [[nodiscard]] int getNbFunctions() const;
    std::string getName();
    void addFunction(const function& fun);
    std::vector<function> FunctionsInFile;

  protected:
    std::string SceneName;
};

#endif // SCRIPTDATA_H
