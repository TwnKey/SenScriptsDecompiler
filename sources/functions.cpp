
#include "headers/functions.h"

function::function(int ID, QString n,int declr_pos,int addr, int end){
    this->ID = ID;
    name = n;
    declr_position = declr_pos;
    actual_addr = addr;
    this->end_addr = end;
}
void function::AddInstruction(std::shared_ptr<Instruction> instr){
    if (instr->get_OP()>0) InstructionsInFunction.push_back(instr); //the only 0 instructions should be
    //the padded 0 at the end; that we will add ourselves depending on how long the function is
}

void function::UsingOPCodes(bool is){
    this->isUsingOPCodes = is;
}
bool operator< (const function &f1, const function &f2)
{
    return f1.actual_addr < f2.actual_addr;
}
void function::SetAddr(int addr){
    this->actual_addr = addr;
}
int function::get_length_in_bytes(){
    int multiple = 4;
    int length_in_bytes = 0;
    for (std::vector<std::shared_ptr<Instruction>>::iterator it = InstructionsInFunction.begin(); it!=InstructionsInFunction.end();it++) length_in_bytes = length_in_bytes+(*it)->get_length_in_bytes();
    int nb_byte_to_add = (((int) ceil((float)length_in_bytes/multiple)))*multiple - length_in_bytes;
    return length_in_bytes + nb_byte_to_add;
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
