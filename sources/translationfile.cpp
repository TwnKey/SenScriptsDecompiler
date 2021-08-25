#include "headers/translationfile.h"

TranslationFile::TranslationFile() {}

void TranslationFile::setName(QString name) { SceneName = name; }
QString TranslationFile::getName() { return SceneName; }
int TranslationFile::getNbFunctions() { return FunctionsInFile.size(); }

void TranslationFile::addFunction(function fun) { FunctionsInFile.push_back(fun); }
