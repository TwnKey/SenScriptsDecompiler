#include "headers/translationfile.h"

TranslationFile::TranslationFile() = default;

void TranslationFile::setName(std::string name) { SceneName = name; }
std::string TranslationFile::getName() { return SceneName; }
int TranslationFile::getNbFunctions() const { return FunctionsInFile.size(); }

void TranslationFile::addFunction(const function& fun) { FunctionsInFile.push_back(fun); }
