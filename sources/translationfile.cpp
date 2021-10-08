#include "headers/translationfile.h"

TranslationFile::TranslationFile() = default;

void TranslationFile::setName(std::string name) { scene_name = std::move(name); }
std::string TranslationFile::getName() { return scene_name; }
int TranslationFile::getNbFunctions() const { return static_cast<int>(FunctionsInFile.size()); }

void TranslationFile::addFunction(const Function& fun) { FunctionsInFile.push_back(fun); }
