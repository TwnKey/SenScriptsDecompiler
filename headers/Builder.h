#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include "headers/operande.h"
#include "xlsxdocument.h"

class Function;
class Instruction;

/*A Builder is an "interface" (I doubt it's the correct word though when not all of the methods are abstract)
But basically it is not supposed to be instanciated "by itself". There are a CS1Builder, a CS2Builder, a CS3Builder, a
CS4Builder.

Each game will have its own builder, but the global behaviour of each will stay the same:
- A builder reads a dat file and creates a vector of instructions
- A builder reads a xlsx file (generated previously from a dat file) and creates a vector of instructions

create_instruction_from_dat: it creates an instruction from the dat file (addr pointing to the first byte, the OP Code)
the addr argument is passed by reference to update the value of addr to the next instruction when the current one has
been completely parsed It is abstract because it completely depends on the game that is used, since CS1, CS2, CS3 and
CS4 all have different instructions set.

create_instruction_from_xlsx: this creates an instruction from the input XLSX file.
It is abstract but could have been virtual currently because the behaviour is always the same, currently.
I put it abstract because I don't know when but I might make different ways to write or read the instruction inside the
XLSX, depending on the OP Code

create_header_from_dat: this parses the header of the dat file and creates the functions inside the translation file
Having the vector of functions alone is enough to reconstruct the header bytes from the dat

create_header_bytes: this reconstructs the header bytes from the functions vector, it will then be written inside the
output dat file by the decompiler object

find_function: this function is used to find the Sen function that contains the addr passed as input;  this way we know
with the pointer value which function contains the location pointed

find_instruction: returns the index of the instruction contained in the previously found function, located at the
integer addr passed as parameter

find_operande: returns the index of the operand inside the previously found instruction contained in the previously
found function, located at the integer addr passed as parameter; it is not used because a pointer (from what I've seen)
always points to an instruction (their op code), not operands

functions_to_parse: This vector contains all the functions, empty (without instructions)

functions_parsed: This vector contains all the functions that were finally parsed and filled with their instructions

scene_name: contains the name of the scene (essentially the filename)

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

    virtual ssd::Buffer create_header_bytes() = 0;
    virtual std::shared_ptr<Instruction> create_instruction_from_dat(int& addr, ssd::Buffer& dat_content, int function_type) = 0;
    virtual bool create_header_from_dat(ssd::Buffer& dat_content) = 0; // Header will probably be game specific
    // Clearly this one is supposed to be generic (assuming the game format doesn't change)
    virtual void read_functions_dat(ssd::Buffer& dat_content);
    bool update_pointers_dat();

    virtual std::shared_ptr<Instruction> create_instruction_from_xlsx(int& addr, int row, QXlsx::Document& xls_content) = 0;
    virtual void read_functions_xlsx(QXlsx::Document& xls_content);
    bool update_pointers_xlsx();

    int find_function(int addr);

    int attempts_at_reading_function(Function& fun, ssd::Buffer& dat_content, const std::vector<int>& fallback_types);
    int read_individual_function(Function& fun, ssd::Buffer& dat_content);

    std::vector<int> guess_type_by_name(Function& fun);

    int find_instruction(int addr, Function fun);

    bool reset();

    std::vector<Function> functions_to_parse;
    std::vector<Function> functions_parsed;
    std::string scene_name;
    int goal = 0;
    bool error = false;
    int idx_current_fun = 0;
};
#endif // INSTRUCTIONBUILDER_H
