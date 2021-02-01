
#include "headers/functions.h"

function::function(int ID, QString n,int declr_pos,int addr, int row_index){
    this->ID = ID;
    name = n;
    declr_position = declr_pos;
    actual_addr = addr;
    this->XLSX_row_index = row_index;
    this->length_in_bytes = 0;
}
void function::AddInstruction(std::shared_ptr<Instruction> instr){
    InstructionsInFunction.push_back(instr);
}

void function::UsingOPCodes(bool is){
    this->isUsingOPCodes = is;
}
bool operator< (const function &f1, const function &f2)
{
    return f1.actual_addr < f2.actual_addr;
}
bool operator== (const function &f1, const function &f2)
{
    return f1.ID == f2.ID;
}
std::vector<function>::iterator find_function_by_name(std::vector<function> &v, QString name){
    return find_if(v.begin(), v.end(), [&name](const function& obj) {return obj.name == name;});
}
std::vector<function>::iterator find_function_by_ID(std::vector<function> &v, int ID){
    return find_if(v.begin(), v.end(), [&ID](const function& obj) {return obj.ID == ID;});
}
std::vector<function>::iterator find_function_by_XLSX_row_index(std::vector<function> &v, int row_index){
    return find_if(v.begin(), v.end(), [&row_index](const function& obj) {return obj.XLSX_row_index == row_index;});
}
