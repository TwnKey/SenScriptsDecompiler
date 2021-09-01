
#include "headers/functions.h"
#include <string>

function::function(int ID, std::string n, int declr_pos, int addr, int end) {
    this->ID = ID;
    name = n;
    declr_position = declr_pos;
    actual_addr = addr;
    this->end_addr = end;
    int multiple = 4;
    if (isMultiple0x10(this->name)) multiple = 0x10;
    int nb_byte_to_add = (((int)ceil((float)addr / (float)multiple))) * multiple - addr;
    nb_pad = nb_byte_to_add;
}
void function::AddInstruction(const std::shared_ptr<Instruction>& instr) {
    if (instr->get_OP() > 0) InstructionsInFunction.push_back(instr);
}

bool operator<(const function& f1, const function& f2) { return f1.actual_addr < f2.actual_addr; }
void function::SetAddr(int addr) { this->actual_addr = addr; }
int function::get_length_in_bytes() {

    int length_in_bytes = 0;
    for (auto& it : InstructionsInFunction) {
        length_in_bytes = length_in_bytes + it->get_length_in_bytes();
    }
    return length_in_bytes;
}
bool operator==(const function& f1, const function& f2) { return f1.ID == f2.ID; }
std::vector<function>::iterator find_function_by_name(std::vector<function>& v, std::string name) {
    return find_if(v.begin(), v.end(), [&name](const function& obj) { return obj.name == name; });
}
std::vector<function>::iterator find_function_by_ID(std::vector<function>& v, int ID) {
    return find_if(v.begin(), v.end(), [&ID](const function& obj) { return obj.ID == ID; });
}
std::vector<function>::iterator find_function_by_XLSX_row_index(std::vector<function>& v, int row_index) {
    return find_if(v.begin(), v.end(), [&row_index](const function& obj) { return obj.XLSX_row_index == row_index; });
}
