#include "headers/translationfile.h"

TranslationFile::TranslationFile() = default;

void TranslationFile::setName(std::string name) { SceneName = std::move(name); }
std::string TranslationFile::getName() { return SceneName; }
int TranslationFile::getNbFunctions() const { return static_cast<int>(FunctionsInFile.size()); }

void TranslationFile::addFunction(const function& fun) { FunctionsInFile.push_back(fun); }
