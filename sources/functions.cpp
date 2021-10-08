
#include "headers/functions.h"
#include <string>

Function::Function(int id, std::string n, int declr_pos, int addr, int end)
  : name(std::move(n))
  , declr_position(declr_pos)
  , actual_addr(addr)
  , end_addr(end)
  , id(id) {
    int multiple = 4;
    if (isMultiple0x10(this->name)) multiple = 0x10;
    int nb_byte_to_add = (((int)std::ceil((float)addr / (float)multiple))) * multiple - addr;
    nb_pad = nb_byte_to_add;
}
void Function::add_instruction(const std::shared_ptr<Instruction>& instr) {
    if (instr->get_OP() > 0) instructions.push_back(instr);
}

bool operator<(const Function& f1, const Function& f2) { return f1.actual_addr < f2.actual_addr; }
void Function::set_addr(int addr) { this->actual_addr = addr; }
int Function::get_length_in_bytes() {

    int length_in_bytes = 0;
    for (auto& it : instructions) {
        length_in_bytes = length_in_bytes + it->get_length_in_bytes();
    }
    return length_in_bytes;
}
bool operator==(const Function& f1, const Function& f2) { return f1.id == f2.id; }
std::vector<Function>::iterator find_function_by_name(std::vector<Function>& v, std::string name) {
    return find_if(v.begin(), v.end(), [&name](const Function& obj) { return obj.name == name; });
}
std::vector<Function>::iterator find_function_by_id(std::vector<Function>& v, int id) {
    return find_if(v.begin(), v.end(), [&id](const Function& obj) { return obj.id == id; });
}
