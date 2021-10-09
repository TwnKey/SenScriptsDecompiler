#ifndef TRANSLATIONFILE_H
#define TRANSLATIONFILE_H

#include "headers/functions.h"

class TranslationFile {
  public:
    TranslationFile();

    void set_name(std::string name);
    std::string get_name();

    [[nodiscard]] int get_nb_functions() const;
    void add_function(const Function& fun);

    std::vector<Function> functions;

  protected:
    std::string scene_name;
};

#endif // TRANSLATIONFILE_H
