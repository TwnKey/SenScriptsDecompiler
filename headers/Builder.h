#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include "headers/operande.h"
#include "xlsxdocument.h"
#include <QByteArray>
#include <QVector>
class function;
class Instruction;

/*A Builder is an "interface" (I doubt it's the correct word though when not all of the methods are abstract)
But basically it is not supposed to be instanciated "by itself". There are a CS1Builder, a CS2Builder, a CS3Builder, a
CS4Builder.

Each game will have its own builder, but the global behaviour of each will stay the same:
- A builder reads a dat file and creates a vector of instructions
- A builder reads a xlsx file (generated previously from a dat file) and creates a vector of instructions

CreateInstructionFromDAT: it creates an instruction from the dat file (addr pointing to the first byte, the OP Code)
the addr argument is passed by reference to update the value of addr to the next instruction when the current one has
been completely parsed It is abstract because it completely depends on the game that is used, since CS1, CS2, CS3 and
CS4 all have different instructions set.

CreateInstructionFromXLSX: this creates an instruction from the input XLSX file.
It is abstract but could have been virtual currently because the behaviour is always the same, currently.
I put it abstract because I don't know when but I might make different ways to write or read the instruction inside the
XLSX, depending on the OP Code

CreateHeaderFromDAT: this parses the header of the dat file and creates the functions inside the translation file
Having the vector of functions alone is enough to reconstruct the header bytes from the dat

CreateHeaderBytes: this reconstructs the header bytes from the functions vector, it will then be written inside the
output dat file by the decompiler object

find_function: this function is used to find the Sen function that contains the addr passed as input;  this way we know
with the pointer value which function contains the location pointed

find_instruction: returns the index of the instruction contained in the previously found function, located at the
integer addr passed as parameter

find_operande: returns the index of the operand inside the previously found instruction contained in the previously
found function, located at the integer addr passed as parameter; it is not used because a pointer (from what I've seen)
always points to an instruction (their op code), not operands

FunctionsToParse: This vector contains all the functions, empty (without instructions)

FunctionsParsed: This vector contains all the functions that were finally parsed and filled with their instructions

SceneName: contains the name of the scene (essentially the filename)

ReadIndividualFunction: it adds to the function being read and located at addr all its instructions until it reaches the
end of the function

UpdatePointersXLSX: after parsing the XLSX file, we get a translation file with its functions and instructions, but all
of the address are not correct so this is updating both the addresses of each function/instructions and also pointers

UpdatePointersDAT: it adds a more generic object called "Destination" to the operands with type "pointers" so that we
could technically put all the functions of the file in a different order and it would still work (but we dont do that)

Reset: clears the function vectors

goal: contains the current function ending address (we should never go beyond)

flag_monsters: if set to true, the current function is a "monster" function

error: the latest instruction/function was incorrect

*/
class Builder {
  public:
    Builder();
    virtual ~Builder() = default;
    virtual std::shared_ptr<Instruction> CreateInstructionFromDAT(int& addr, QByteArray& dat_content, int function_type) = 0;
    virtual std::shared_ptr<Instruction> CreateInstructionFromXLSX(int& addr, int row, QXlsx::Document& xls_content) = 0;

    virtual bool CreateHeaderFromDAT(QByteArray& dat_content) = 0; // Header will probably be game specific
    virtual QByteArray CreateHeaderBytes() = 0;

    virtual void ReadFunctionsDAT(
      QByteArray& dat_content); // Clearly this one is supposed to be generic (assuming the game format doesn't change)
    virtual void ReadFunctionsXLSX(QXlsx::Document& xls_content);

    int find_function(uint addr);
    int find_instruction(uint addr, function fun);
    int find_operande(uint addr, const Instruction& instr);
    std::vector<function> FunctionsToParse;
    std::vector<function> FunctionsParsed;
    std::string SceneName;
    int ReadIndividualFunction(function& fun, QByteArray& dat_content);
    bool UpdatePointersXLSX();
    bool UpdatePointersDAT();
    bool Reset();
    int goal = 0;
    bool flag_monsters = false;
    bool error = false;
    int idx_current_fun = 0;
    std::string previous_fun_name;
};
#endif // INSTRUCTIONBUILDER_H
