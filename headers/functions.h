#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QString>
#include "headers/instruction.h"

class function{
public:
    function(){}
    function(int ID, QString n,int declr_pos,int addr);
    void AddInstruction(std::shared_ptr<Instruction> instr);
    void UsingOPCodes(bool is);
    std::vector<std::shared_ptr<Instruction>> InstructionsInFunction;
    QString name;
    int declr_position; //covers the list of offsets at the beginning
    int actual_addr; //covers the pointers before that
    bool isUsingOPCodes;
    int ID;
    friend bool operator< (const function &f1, const function &f2);
    friend bool operator== (const function &f1, const function &f2);
};
std::vector<function>::iterator find_function_by_name(std::vector<function> &v, QString name);
std::vector<function>::iterator find_function_by_ID(std::vector<function> &v, int ID);
#endif // FUNCTIONS_H
