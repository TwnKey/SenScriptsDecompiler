#include "headers/scriptdata.h"

ScriptData::ScriptData() = default;

void ScriptData::setName(std::string name) { SceneName = std::move(name); }
std::string ScriptData::getName() { return SceneName; }
int ScriptData::getNbFunctions() const { return static_cast<int>(FunctionsInFile.size()); }

void ScriptData::addFunction(const function& fun) { FunctionsInFile.push_back(fun); }
