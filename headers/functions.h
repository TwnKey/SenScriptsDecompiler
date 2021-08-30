#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "headers/instruction.h"
/*
The class function
*/
class function {
  public:
    function() = default;
    function(int ID, std::string n, int declr_pos, int addr, int end);
    void AddInstruction(const std::shared_ptr<Instruction>& instr);
    void UsingOPCodes(bool is);
    std::vector<std::shared_ptr<Instruction>> InstructionsInFunction;
    std::string name;
    int declr_position = 0; // covers the list of offsets at the beginning
    int actual_addr = 0;    // covers the pointers before that
    uint end_addr = 0;
    int XLSX_row_index = 0;
    bool called = false; // by default
    int ID = 0;
    friend bool operator<(const function& f1, const function& f2);
    friend bool operator==(const function& f1, const function& f2);
    int nb_pad = 0;
    void SetAddr(int addr);
    int get_length_in_bytes();
};
std::vector<function>::iterator find_function_by_name(std::vector<function>& v, std::string name);
std::vector<function>::iterator find_function_by_ID(std::vector<function>& v, int ID);
std::vector<function>::iterator find_function_by_XLSX_row_index(std::vector<function>& v, int row_index);
#endif // FUNCTIONS_H
