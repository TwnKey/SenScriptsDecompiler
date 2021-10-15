#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "headers/Builder.h"
#include "headers/operande.h"
#include <cmath>

class Instruction {
  public:
    Instruction(int addr, uint OP, Builder* Maker);
    Instruction(int& addr, std::string name, uint op, Builder* Maker);

    virtual ~Instruction();

    [[nodiscard]] uint get_opcode() const;

    [[nodiscard]] Operande get_operande(int i) const;
    void add_operande(Operande operande);
    [[nodiscard]] int get_nb_operandes() const;

    [[nodiscard]] int get_addr_instr() const;
    void set_addr_instr(int i);

    int get_length_in_bytes();
    ssd::Buffer get_bytes();

    Builder* maker;
    std::vector<Operande> operandes;
    bool error = false;

  protected:
    int addr_instr;
    uint opcode;
    std::string name;
    int length_in_bytes = 0;
};

#endif // INSTRUCTION_H
