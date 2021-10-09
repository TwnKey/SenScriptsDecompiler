#include "headers/translationfile.h"

TranslationFile::TranslationFile() = default;

void TranslationFile::set_name(std::string name) { scene_name = std::move(name); }
std::string TranslationFile::get_name() { return scene_name; }

int TranslationFile::get_nb_functions() const { return static_cast<int>(functions.size()); }
void TranslationFile::add_function(const Function& fun) { functions.push_back(fun); }
