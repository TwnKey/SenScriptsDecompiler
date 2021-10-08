#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "headers/instruction.h"

class Function {
  public:
    Function() = default;
    Function(int id, std::string n, int declr_pos, int addr, int end);

    void add_instruction(const std::shared_ptr<Instruction>& instr);
    void set_addr(int addr);
    int get_length_in_bytes();

    friend bool operator<(const Function& f1, const Function& f2);
    friend bool operator==(const Function& f1, const Function& f2);

    int id = 0;
    std::string name;
    int declr_position = 0; // covers the list of offsets at the beginning
    int actual_addr = 0;    // covers the pointers before that
    int end_addr = 0;
    int xlsx_row_index = 0;
    int nb_pad = 0;
    std::vector<std::shared_ptr<Instruction>> instructions;
};

std::vector<Function>::iterator find_function_by_name(std::vector<Function>& v, std::string name);
std::vector<Function>::iterator find_function_by_id(std::vector<Function>& v, int id);
#endif // FUNCTIONS_H
