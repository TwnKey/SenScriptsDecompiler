#ifndef TXINSTRUCTIONSSET_H
#define TXINSTRUCTIONSSET_H
#include "headers/functions.h"
#include "headers/utilities.h"

class TXBuilder : public Builder {
  public:
    TXBuilder() = default;

    std::set<std::string> TXUIFiles = { "battle_menu", "camp_menu",   "camp_menu_v", "note_menu",   "note_menu_v",
                                        "shop_menu",   "shop_menu_v", "title_menu",  "title_menu_v" };
    static void reading_dialog(int& addr, ssd::Buffer& content, Instruction* instr) {

        std::vector<uint8_t> current_op_value;
        int addr_ = addr;

        bool start_text = false;
        int cnt = 0;
        do {
            unsigned char current_byte = content[addr];

            switch (current_byte) {
                case 0x00:
                    current_op_value.clear();
                    current_op_value.push_back(0);
                    instr->add_operande(Operande(addr, "byte", current_op_value));
                    addr++;
                    return;
                case 0x01:

                    if (start_text) {
                        current_op_value.push_back(current_byte);
                    } else {

                        current_op_value.clear();
                        current_op_value.push_back(current_byte);
                        instr->add_operande(Operande(addr, "byte", current_op_value));
                        current_op_value.clear();
                    }
                    addr++;
                    break;
                case 0x02:
                    start_text = false;
                    if (!current_op_value.empty()) {
                        instr->add_operande(Operande(addr_, "dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->add_operande(Operande(addr, "byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    break;
                case 0x10:
                    if (!current_op_value.empty()) {
                        instr->add_operande(Operande(addr_, "dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->add_operande(Operande(addr, "byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x17:
                case 0x19:
                    start_text = false;
                    if (!current_op_value.empty()) {
                        instr->add_operande(Operande(addr_, "dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->add_operande(Operande(addr, "byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x11:
                case 0x12:
                    if (!current_op_value.empty()) {
                        instr->add_operande(Operande(addr_, "dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->add_operande(Operande(addr, "byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x23:
                    current_op_value.push_back(0x23);
                    addr_ = addr;
                    addr++;
                    current_byte = content[addr];
                    if ((((current_byte == 0x45) || (current_byte == 0x65)) || (current_byte == 0x4d)) || (current_byte == 0x42) ||
                        (current_byte == 0x48) || (current_byte == 0x56) || (current_byte == 0x4b) || (current_byte == 0x6b) ||
                        (current_byte == 0x46)) {
                        current_op_value.push_back(current_byte);
                        addr++;
                        current_byte = content[addr];

                        if (current_byte != 0x5f) {
                            if (current_byte == 0x5b) {
                                do {
                                    current_op_value.push_back(current_byte);
                                    addr++;
                                    current_byte = content[addr];
                                } while (current_byte != 0x5D);
                                current_op_value.push_back(current_byte);
                                addr++;
                                current_byte = content[addr];
                            }
                        } else {
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                        }
                    } else if ((((current_byte + 0xb7) & 0xdf) == 0) || (current_byte == 0x50) || (current_byte == 0x54) ||
                               (current_byte == 0x57) || (current_byte == 0x53) || (current_byte == 0x73) || (current_byte == 0x43) ||
                               (current_byte == 99) || (current_byte == 0x78) || (current_byte == 0x79) || (current_byte == 0x47) ||
                               (current_byte == 0x44) || (current_byte == 0x55) || (current_byte == 0x52)) {
                        current_op_value.push_back(current_byte);
                        addr++;
                        current_byte = content[addr];
                    }
                    break;
                default:
                    if (current_byte < 0x20) {
                        if (!current_op_value.empty()) {
                            instr->add_operande(Operande(addr_, "dialog", current_op_value));
                        }
                        start_text = false;
                        current_op_value.clear();
                        current_op_value.push_back(current_byte);
                        instr->add_operande(Operande(addr, "byte", current_op_value));
                        addr++;
                        current_op_value.clear();
                    } else {
                        if (!(start_text)) start_text = true;

                        if ((current_byte < 0xE0) && (current_byte >= 0xC0)) {
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                        } else if ((current_byte >= 0xE0) && (current_byte <= 0xF7)) {
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                        } else if ((current_byte > 0xF7)) {
                            current_op_value.push_back(current_byte);
                            addr++;
                        } else {
                            current_op_value.push_back(current_byte);
                            addr++;
                        }
                    }
                    break;
            }

            cnt++;
        } while (cnt < 9999);
    }
    static void fun_1403c90e0(int& addr, ssd::Buffer& content, Instruction* instr, int param) {
        ssd::Buffer control_byte3_arr = ReadSubByteArray(content, addr, 1);
        instr->add_operande(Operande(addr, "byte", control_byte3_arr));
        auto control_byte3 = (unsigned char)control_byte3_arr[0];

        if (control_byte3 == '\x11') {
            instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

        } else if (control_byte3 != '3') {

            if (control_byte3 == '\"') {
                instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

            } else if (control_byte3 != 0x44) {

                if (control_byte3 == 0xDD) {

                    if (param != 0) instr->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    instr->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                } else if (control_byte3 == 0xFF) {
                    instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                } else {
                    if (control_byte3 != 0xEE) {

                    } else {
                        instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    }
                }

            } else {
                instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                if (param != 0) instr->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            }

        } else {
            instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    }
    static void sub05(int& addr, ssd::Buffer& content, Instruction* instr) {
        ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
        instr->add_operande(Operande(addr, "byte", control_byte));

        while ((int)control_byte[0] != 1) {
            if (addr > std::ssize(content)) throw ssd::exceptions::unspecified_recoverable();
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                case 0x24:
                    instr->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x1c: {
                    // the next byte is the OP code for a new instruction

                    std::shared_ptr<Instruction> instr2 = instr->maker->create_instruction_from_dat(addr, content, 0);

                    auto op = Operande(addr, "instruction", instr2->get_bytes());
                    instr->add_operande(op);

                    break;
                }
                case 0x1e:
                    instr->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x1f:
                case 0x20:
                case 0x23:

                    instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x21:
                    instr->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x25:
                    instr->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                default:;
            }

            control_byte = ReadSubByteArray(content, addr, 1);

            instr->add_operande(Operande(addr, "byte", control_byte));
        }
    }
    class CreateMonsters : public Instruction {
      public:
        CreateMonsters(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "CreateMonsters", 256, Maker) {}
        CreateMonsters(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "CreateMonsters", 256, Maker) {
            int initial_addr = addr;
            int max_nb_monsters = 8;
            if (Maker->goal < addr + 0x20) {

                addr = initial_addr;
                throw ssd::exceptions::unexpected_operand();
            }
            int first = ReadIntegerFromByteArray(addr, content);

            if ((first == -1) && (addr + 0x20 == Maker->goal)) {

                this->add_operande(
                  Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1C))); //?? we don't forget to take the int as well
                throw ssd::exceptions::unexpected_operand();
            }
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            int check1 = ReadIntegerFromByteArray(addr, content);
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            int check2 = ReadIntegerFromByteArray(addr, content);
            if (check1 != 0 && check2 != 0) { // bad
                addr = initial_addr;
                throw ssd::exceptions::unexpected_operand();
            }
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x10
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x1A
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 0x1C
            // 0x20

            int cnt = 0;
            do {

                ssd::Buffer array = ReadSubByteArray(content, addr, 4);
                first = ReadIntegerFromByteArray(0, array);
                this->add_operande(Operande(addr, "int", array));

                int counter = 0;

                if (first == -2) {

                    array = ReadSubByteArray(content, addr, 4);
                    this->add_operande(Operande(addr, "int", array));
                    max_nb_monsters = 4;
                } else if (first == -1) {
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x18))); //?? it's empty
                    return;
                } else {
                    max_nb_monsters = 8;
                }

                if (Maker->goal < addr + (0x10) * max_nb_monsters + max_nb_monsters * 2) {

                    addr = initial_addr;
                    throw ssd::exceptions::unexpected_operand();
                }

                do {
                    counter++;
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", monsters_name));

                    ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - monsters_name.size());
                    auto fill = Operande(addr, "fill", remaining);
                    fill.set_bytes_to_fill(0x10);
                    this->add_operande(fill);

                } while (counter < max_nb_monsters);

                for (int idx_byte = 0; idx_byte < max_nb_monsters; idx_byte++) {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }

                if ((unsigned char)content[addr] == 0) {
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, max_nb_monsters))); //??
                } else {
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", monsters_name));

                    ssd::Buffer remaining = ReadSubByteArray(content, addr, 12 - monsters_name.size());
                    auto fill = Operande(addr, "bytearray", remaining);
                    fill.set_bytes_to_fill(12);
                    this->add_operande(fill);
                }

                first = ReadIntegerFromByteArray(addr, content);

                cnt++;

            } while ((first != -1) && (addr != Maker->goal - 4));
            if (addr == Maker->goal - 4) {
                first = ReadIntegerFromByteArray(addr, content);
                if (first != 1) {
                    addr = initial_addr;
                    throw ssd::exceptions::unexpected_operand();
                }
                return;
            }
            this->add_operande(
              Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1C))); //?? we don't forget to take the int as well
        }
    };
    class EffectsInstr : public Instruction {
      public:
        EffectsInstr(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "EffectsInstr", 257, Maker) {}
        EffectsInstr(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "EffectsInstr", 257, Maker) {
            bool bytes_blocks_remain = Maker->goal >= addr + 0x28;

            while (bytes_blocks_remain) {

                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));

                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());

                auto fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);

                bytes_blocks_remain = Maker->goal >= addr + 0x28;
            }
        }
    };
    class ActionTable : public Instruction {
      public:
        ActionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "ActionTable", 258, Maker) {}
        ActionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ActionTable", 258, Maker) {

            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {

                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                this->add_operande(Operande(addr, "short", short_bytes));                       // 2
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-46
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-45
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-44
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-43
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-42
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-41
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-40
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-3F
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-3E
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // ebp-3D
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-3C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-38
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-34
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-30
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-2C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-28
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // ebp-24
                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - str.size());
                auto fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x10);
                this->add_operande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x30 - str.size());
                fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x30);
                this->add_operande(fill);

                cnt++;
            }
        }
    };
    class AddCollision : public Instruction {
      public:
        AddCollision(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AddCollision", 271, Maker) {}
        AddCollision(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AddCollision", 271, Maker) {

            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                cnt++;
            }
        }
    };
    class ConditionTable : public Instruction {
      public:
        ConditionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "ConditionTable", 272, Maker) {}
        ConditionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ConditionTable", 272, Maker) {

            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // EBP-52
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-50
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-4C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-48
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-44
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-40
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-3C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-38
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-34
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-30
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-2C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-28
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-24
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-20
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-1C
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-18
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // EBP-14
                cnt++;
            }
        }
    };
    class AlgoTable
      : public Instruction // 42d177
    {
      public:
        AlgoTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AlgoTable", 259, Maker) {}
        AlgoTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AlgoTable", 259, Maker) {
            int cnt = 0;
            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // 3
            while (cnt < current_byte) {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);

                this->add_operande(Operande(addr, "short", short_bytes));

                this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1E)));
                if (addr + 0x20 > Maker->goal) return;
                cnt++;
            }

            // this->add_operande(Operande(addr,"bytearray", ReadSubByteArray(content, addr,0x1E)));
        }
    };
    class WeaponAttTable : public Instruction {
      public:
        WeaponAttTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "WeaponAttTable", 260, Maker) {}
        WeaponAttTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "WeaponAttTable", 260, Maker) {

            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 4)));
        }
    };
    class BreakTable : public Instruction {
      public:
        BreakTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BreakTable", 261, Maker) {}
        BreakTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "BreakTable", 261, Maker) {
            int cnt = 0;
            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // 3
            while (cnt < current_byte) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                cnt++;
            }
        }
    };
    class SummonTable
      : public Instruction // 140142002
    {
      public:
        SummonTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "SummonTable", 262, Maker) {}
        SummonTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "SummonTable", 262, Maker) {

            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                uint16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->add_operande(Operande(addr, "short", short_bytes));
                if (shrt == 0xFFFF) break;
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                auto fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);
                cnt++;
            }
        }
    };
    class ReactionTable
      : public Instruction // 140142002
    {
      public:
        ReactionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "ReactionTable", 263, Maker) {}
        ReactionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ReactionTable", 263, Maker) {
            int cnt = 0;
            uint16_t current_shrt = ReadShortFromByteArray(addr, content);
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            while (cnt < current_shrt) {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);

                this->add_operande(Operande(addr, "short", short_bytes));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                cnt++;
            }
        }
    };
    class PartTable
      : public Instruction // 14019797c
    {
      public:
        PartTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "PartTable", 264, Maker) {}
        PartTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "PartTable", 264, Maker) {
            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {

                ssd::Buffer int_bytes = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "int", int_bytes));

                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                auto fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);

                cnt++;
            }
        }
    };

    class AnimeClipTable
      : public Instruction // from CS3
    {
      public:
        AnimeClipTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AnimeClipTable", 265, Maker) {}
        AnimeClipTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AnimeClipTable", 265, Maker) {

            int first_integer = ReadIntegerFromByteArray(addr, content);
            auto itt_current_fun = find_function_by_id(Maker->functions_to_parse, Maker->idx_current_fun);
            while (first_integer != 0) {
                itt_current_fun->add_instruction(std::make_shared<AnimeClipData>(addr, content, Maker));
                first_integer = ReadIntegerFromByteArray(addr, content);
            }
            ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
            this->add_operande(Operande(addr, "int", first_integer_bytes));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class AnimeClipData : public Instruction {
      public:
        AnimeClipData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AnimeClipData", 273, Maker) {}
        AnimeClipData(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AnimeClipData", 273, Maker) {

            ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
            this->add_operande(Operande(addr, "int", first_integer_bytes));
            ssd::Buffer str = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", str));
            ssd::Buffer remaining = ReadSubByteArray(content, addr, (0x20) - str.size());
            auto fill = Operande(addr, "fill", remaining);
            fill.set_bytes_to_fill((0x20));
            this->add_operande(fill);
            str = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", str));
            remaining = ReadSubByteArray(content, addr, (0x20) - str.size());
            fill = Operande(addr, "fill", remaining);
            fill.set_bytes_to_fill((0x20));
            this->add_operande(fill);
        }
    };
    class FieldMonsterData
      : public Instruction // 00000001402613C2 probably trigger only for monsters on the field
    {
      public:
        FieldMonsterData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FieldMonsterData", 266, Maker) {}
        FieldMonsterData(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "FieldMonsterData", 266, Maker) {

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class FieldFollowData
      : public Instruction //
    {
      public:
        FieldFollowData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FieldMonsterData", 267, Maker) {}
        FieldFollowData(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "FieldMonsterData", 267, Maker) {

            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class FC_autoX
      : public Instruction //
    {
      public:
        FC_autoX(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FC_autoX", 268, Maker) {}
        FC_autoX(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "FC_autoX", 268, Maker) {

            ssd::Buffer things = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", things));
        }
    };
    class BookData99
      : public Instruction //
    {
      public:
        BookData99(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BookData99", 269, Maker) {}
        BookData99(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "BookData99", 269, Maker) {
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class BookDataX : public Instruction {
      public:
        BookDataX(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BookDataX", 270, Maker) {}
        BookDataX(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "BookDataX", 270, Maker) {
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "short", control_short)); // 3
            if (control > 0) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                ssd::Buffer title = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", title));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - title.size());
                auto fill = Operande(addr, "bytearray", remaining);
                fill.set_bytes_to_fill(0x10);
                this->add_operande(fill);
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x12
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x14
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x16
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x18
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1A
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1C
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1E
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x20
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x22
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x24
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            } else {
                if ((unsigned char)content[addr] != 1) {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                }
            }
        }
    };

    class OPCode0 : public Instruction {
      public:
        OPCode0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "Instruction 0", 0, Maker) {}
        OPCode0(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Instruction 0", 0, Maker) {
            addr++;
        }
    };
    class OPCode1 : public Instruction {
      public:
        OPCode1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "Return", 1, Maker) {}
        OPCode1(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Return", 1, Maker) {
            addr++;
        }
    };

    class UI_OP13 : public Instruction {
      public:
        UI_OP13(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x13, Maker) {}
        UI_OP13(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x13, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);

            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x00: {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x01: {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 10: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0xb: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0xc: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x26:
                case 0xd: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xe: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0xf: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x10: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x11: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x12: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x13: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x17: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x18: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x19: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x1B:
                case 0x1A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x1C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x1D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x20:
                case 0x1F:
                case 0x1E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x21: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x22: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x23: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x24: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x25: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x27: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x28: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x29: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x30:
                case 0x2F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x33: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x34: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x35: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x36: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x37: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3B:
                case 0x38: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x3A:
                case 0x39: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x3C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x41:
                case 0x3D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x40: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x44:
                case 0x43: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x65: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x68: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x76:
                case 0x69:
                case 0x75: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x6B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x6A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x6C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x70: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x71: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x72: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x73: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x32: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x31: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                default:
                    throw ssd::exceptions::not_analyzed_yet(control_byte.at(0));
            }
        }
    };
    // SCENARIO FILES INSTRUCTIONS
    // 0x05
    class OPCode05 : public Instruction {
      public:
        OPCode05(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x05, Maker) {}
        OPCode05(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x05, Maker) {
            addr++;
            sub05(addr, content, this);

            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };

    // 0x02
    class OPCode02 : public Instruction {
      public:
        OPCode02(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x02, Maker) {}
        OPCode02(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x02, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x03
    class OPCode03 : public Instruction {
      public:
        OPCode03(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x03, Maker) {}
        OPCode03(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x03, Maker) {
            addr++;
            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x04
    class OPCode04 : public Instruction {
      public:
        OPCode04(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x04, Maker) {}
        OPCode04(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x04, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x06
    class OPCode06 : public Instruction {
      public:
        OPCode06(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x06, Maker) {}
        OPCode06(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x06, Maker) {
            addr++;
            sub05(addr, content, this);
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            if ((unsigned char)control_byte[0] != 0) {

                for (unsigned char i = 0; i < (unsigned char)control_byte[0]; i++) {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
                }
            }
            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x07
    class OPCode07 : public Instruction {
      public:
        OPCode07(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x07, Maker) {}
        OPCode07(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x07, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x08
    class OPCode08 : public Instruction {
      public:
        OPCode08(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x08, Maker) {}
        OPCode08(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x08, Maker) {
            addr++;
            sub05(addr, content, this);
        }
    };
    // 0x0A
    class OPCode0A : public Instruction {
      public:
        OPCode0A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0A, Maker) {}
        OPCode0A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
    // 0x0C
    class OPCode0C : public Instruction {
      public:
        OPCode0C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0C, Maker) {}
        OPCode0C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x0D
    class OPCode0D : public Instruction {
      public:
        OPCode0D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0D, Maker) {}
        OPCode0D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x0E
    class OPCode0E : public Instruction {
      public:
        OPCode0E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0E, Maker) {}
        OPCode0E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x0F
    class OPCode0F : public Instruction {
      public:
        OPCode0F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0F, Maker) {}
        OPCode0F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x10
    class OPCode10 : public Instruction {
      public:
        OPCode10(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x10, Maker) {}
        OPCode10(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x10, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x11
    class OPCode11 : public Instruction {
      public:
        OPCode11(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x11, Maker) {}
        OPCode11(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x11, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x12
    class OPCode12 : public Instruction {
      public:
        OPCode12(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x12, Maker) {}
        OPCode12(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x12, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
    // 0x13
    class OPCode13 : public Instruction {
      public:
        OPCode13(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x13, Maker) {}
        OPCode13(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x13, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(
              Operande(addr, "int", ReadSubByteArray(content, addr, 4))); // i think this one is the id of the battle function it triggers
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };

    // 0x14
    class OPCode14 : public Instruction {
      public:
        OPCode14(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x14, Maker) {}
        OPCode14(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x14, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x15
    class OPCode15 : public Instruction {
      public:
        OPCode15(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x15, Maker) {}
        OPCode15(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x15, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x16
    class OPCode16 : public Instruction {
      public:
        OPCode16(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x16, Maker) {}
        OPCode16(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x16, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x17
    class OPCode17 : public Instruction {
      public:
        OPCode17(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x17, Maker) {}
        OPCode17(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x17, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x18
    class OPCode18 : public Instruction {
      public:
        OPCode18(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x18, Maker) {}
        OPCode18(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x18, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            reading_dialog(addr, content, this);
        }
    };
    // 0x19
    class OPCode19 : public Instruction {
      public:
        OPCode19(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x19, Maker) {}
        OPCode19(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x19, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x05:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x1A
    class OPCode1A : public Instruction {
      public:
        OPCode1A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1A, Maker) {}
        OPCode1A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            reading_dialog(addr, content, this);
        }
    };
    // 0x1B
    class OPCode1B : public Instruction {
      public:
        OPCode1B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1B, Maker) {}
        OPCode1B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x1C
    class OPCode1C : public Instruction {
      public:
        OPCode1C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1C, Maker) {}
        OPCode1C(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1C, Maker) {
            addr++;
        }
    };
    // 0x1D
    class OPCode1D : public Instruction {
      public:
        OPCode1D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1D, Maker) {}
        OPCode1D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x1E
    class OPCode1E : public Instruction {
      public:
        OPCode1E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1E, Maker) {}
        OPCode1E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x1F
    class OPCode1F : public Instruction {
      public:
        OPCode1F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1F, Maker) {}
        OPCode1F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte2));
            if ((unsigned char)control_byte2[0] < 0x4) {
                switch ((unsigned char)control_byte[0]) {

                    case 0x00: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }

                    case 0x01: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }
                    case 0x02: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x04: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x05: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }
                    case 0x07:
                    case 0x06: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                        break;
                    }
                    case 0x08: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }
                    case 0x09: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }
                    case 0x0B: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }

                    case 0x0E:
                    case 0x0D: {
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }
                }
            }
        }
    };
    // 0x20
    class OPCode20 : public Instruction {
      public:
        OPCode20(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x20, Maker) {}
        OPCode20(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x20, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x02:
                case 0x01:
                case 0x04:
                case 0x03:
                case 0x00: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x21
    class OPCode21 : public Instruction {
      public:
        OPCode21(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x21, Maker) {}
        OPCode21(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x21, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x22
    class OPCode22 : public Instruction {
      public:
        OPCode22(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x22, Maker) {}
        OPCode22(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x22, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x23
    class OPCode23 : public Instruction {
      public:
        OPCode23(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x23, Maker) {}
        OPCode23(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x23, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x24
    class OPCode24 : public Instruction {
      public:
        OPCode24(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x24, Maker) {}
        OPCode24(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x24, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case 2:
                case 4:
                case 10:
                case 0xB:
                case 5: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 7:
                case 6: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 9: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0xE: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 8: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
            }
        }
    };
    // 0x25
    class OPCode25 : public Instruction {
      public:
        OPCode25(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x25, Maker) {}
        OPCode25(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x25, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            switch ((unsigned char)control_byte[0]) {
                /*case 1:{
                    this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }*/
                case 7:
                case 8: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x26
    class OPCode26 : public Instruction {
      public:
        OPCode26(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x26, Maker) {}
        OPCode26(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x26, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x27
    class OPCode27 : public Instruction {
      public:
        OPCode27(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x27, Maker) {}
        OPCode27(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x27, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x0A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x0B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x0C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }

                case 0x17: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x12: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x14: {
                    throw ssd::exceptions::parse_error("not entirely sure");
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x13: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0xD: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0xE: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x0F:
                case 0x11: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x10: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x18: {
                    throw ssd::exceptions::parse_error("not sureeeeee");
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x19: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x1A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x28
    /*class OPCode28: public Instruction{
        OPCode28(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){ case 0x00:{ this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x01:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '\b':
            case 0x06:
            case 0x04:
            {
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '\a':
            case 0x05:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 'm':
            case 'l':
            case 'k':
            case 'f':
            case 'e':
            case 'c':
            case '\x1C':



            case '\x14':
            case '\r':
            case '\x10':
            case 'H':
            case ']':
            case '\t':
            case '^':
            case '_':
            case '`':
            case 's':
            case 't':
            case 'u':
            case 'y':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '\x0f':
            case '\n':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '\f':
            case '4':
            case 'Z':
            case '\v':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\x0e':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }

            case '\x11':
            case 'z':{

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '\x12':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                for (int i = 0; i<8; i++) this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '\x13':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\x15':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '2':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case '5':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '6':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '7':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case '<':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'F':{

                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'P':{

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case '[':{

                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\\':{

                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 'a':{

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 'd':{

                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 'n':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '~':
            case 'q':{

                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 'r':{

                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '{':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 'i':{



                break;
            }
            case 'j':{


                break;
            }
            case 0x51:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x91:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x1D:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x27:{


                break;
            }
            case 0x26:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x28:{

                break;
            }
            case 0x29:{

                break;
            }
            case 0x2A:{

                break;
            }
            case 0x1A:{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x19:{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'b':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x97:{

                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x76:{

                break;
            }
            case 0x8D:
            case 0x8C:
            case 0x8B:{
                break;
            }
            case 0x47:{
                break;
            }
            case 0x53:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x68:{

                break;
            }
            case 0x92:{
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x93:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x96:
            case 0x95:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x9D:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0xA0:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0xA1:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0xA2:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA3:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA4:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA5:{
                 this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                 this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                 this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x87:{

                break;
            }
            case 0x2B:{

                break;
            }
            case 0x9A:{

                break;
            }
            case 0x33:{

                break;
            }
            case 0x02:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x67:{

                break;
            }
            case 0x03:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x6F:{

                break;
            }
            case 0x70:{

                break;
            }
            case 0x83:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x85:{

                break;
            }
            case 0x86:{

                break;
            }
            case 0x1B:{

                break;
            }
            case 0x98:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x9E:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x16:{

                break;
            }
            case 0x2E:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x7F:{

                break;
            }
            case 0x89:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

            case 0x8A:{
                break;
            }
            case 0x52:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x1F:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x18:{

                break;
            }
            case 0x1E:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x2C:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x2D:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x20:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x22:{
                break;
            }
            case 0x24:{
                break;
            }
            case 0x25:{
                break;
            }
            case 0x23:{
                break;
            }
            case 0x21:{
                break;
            }
            case '\x17':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x8F:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x8E:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x99:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x9B:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x9C:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x7D:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x49:{
                this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x88:{

                break;
            }
            case 0x80:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x9F:{

                break;
            }
            case 0x81:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x82:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            default:
                throw ssd::exceptions::not_analyzed_yet(control_byte.at(0));
        }


        }

    };*/
    // 0x28
    class OPCode28 : public Instruction {
      public:
        OPCode28(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x28, Maker) {}
        OPCode28(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x28, Maker) {
            addr++;

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x29
    class OPCode29 : public Instruction {
      public:
        OPCode29(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x29, Maker) {}
        OPCode29(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x29, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x2A
    class OPCode2A : public Instruction {
      public:
        OPCode2A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2A, Maker) {}
        OPCode2A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x2B
    class OPCode2B : public Instruction {
      public:
        OPCode2B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2B, Maker) {}
        OPCode2B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

    // 0x2C
    /*class OPCode2C: public Instruction{
        OPCode2C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,2))); this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

        }

    };*/
    // 0x2C
    class OPCode2C : public Instruction {
      public:
        OPCode2C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2C, Maker) {}
        OPCode2C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x02: {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x14:
                case 0x03: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x05: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x07: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x08: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x09: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0B: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0D:
                case 0x0C: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0E: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x11: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x12: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x13: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x17: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x18:
                case 0x19: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1A: {

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x1B: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x1E: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x22:
                case 0x1F: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x21:
                case 0x20: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x24: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x25: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x26: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x2D
    class OPCode2D : public Instruction {
      public:
        OPCode2D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2D, Maker) {}
        OPCode2D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x2E
    class OPCode2E : public Instruction {
      public:
        OPCode2E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2E, Maker) {}
        OPCode2E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
                case 0x03: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x2F
    class OPCode2F : public Instruction {
      public:
        OPCode2F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2F, Maker) {}
        OPCode2F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x30
    class OPCode30 : public Instruction {
      public:
        OPCode30(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x30, Maker) {}
        OPCode30(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x30, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 1: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 2: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                default: {
                    break;
                }
                case 3: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 4: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 5: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 6: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x31
    class OPCode31 : public Instruction {
      public:
        OPCode31(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x31, Maker) {}
        OPCode31(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x31, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            if ((unsigned char)control_byte[0] > 0xFD) {
                if ((unsigned char)control_byte[0] == 0xFE) {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                } else if ((unsigned char)control_byte[0] == 0xFF) {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                }

                return;
            }

            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                case 0x14:
                case 0x32: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x33:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x34:
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x35:
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x39:
                case 0x37:
                case 0x3B:
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x3A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 100: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x65: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x96: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x97: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 4:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                case 0x10:
                case 0x11:
                case 0x12:
                case 0x13:
                case 0x15:
                case 0x16:
                case 0x17:
                case 0x18:
                case 0x19:
                case 0x1a:
                case 0x1b:
                case 0x1c:
                case 0x1d:
                case 0x1e:
                case 0x1f:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                case 0x25:
                case 0x26:
                case 0x27:
                case 0x28:
                case 0x29:
                case 0x2a:
                case 0x2b:
                case 0x2c:
                case 0x2d:
                case 0x2e:
                case 0x2f:
                case 0x30:
                case 0x31:
                case 0x36:
                case 0x38:
                case 0x3c:
                case 0x3d:
                case 0x3e:
                case 0x3f:
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x43:
                case 0x44:
                case 0x45:
                case 0x46:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4a:
                case 0x4b:
                case 0x4c:
                case 0x4d:
                case 0x4e:
                case 0x4f:
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57:
                case 0x58:
                case 0x59:
                case 0x5a:
                case 0x5b:
                case 0x5c:
                case 0x5d:
                case 0x5e:
                case 0x5f:
                case 0x60:
                case 0x61:
                case 0x62:
                case 99:
                case 0x66:
                case 0x67:
                case 0x68:
                case 0x69:
                case 0x6a:
                case 0x6b:
                case 0x6c:
                case 0x6d:
                case 0x6e:
                case 0x6f:
                case 0x70:
                case 0x71:
                case 0x72:
                case 0x73:
                case 0x74:
                case 0x75:
                case 0x76:
                case 0x77:
                case 0x78:
                case 0x79:
                case 0x7a:
                case 0x7b:
                case 0x7c:
                case 0x7d:
                case 0x7e:
                case 0x7f:
                case 0x80:
                case 0x81:
                case 0x82:
                case 0x83:
                case 0x84:
                case 0x85:
                case 0x86:
                case 0x87:
                case 0x88:
                case 0x89:
                case 0x8a:
                case 0x8b:
                case 0x8c:
                case 0x8d:
                case 0x8e:
                case 0x8f:
                case 0x90:
                case 0x91:
                case 0x92:
                case 0x93:
                case 0x94:
                case 0x95:
                    break;
                default: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
            }
        }
    };
    // 0x33
    class OPCode32 : public Instruction {
      public:
        OPCode32(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x32, Maker) {}
        OPCode32(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x32, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x01: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
            }
        }
    };
    // 0x33
    class OPCode33 : public Instruction {
      public:
        OPCode33(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x33, Maker) {}
        OPCode33(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x33, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x34
    class OPCode34 : public Instruction {
      public:
        OPCode34(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x34, Maker) {}
        OPCode34(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x34, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x35
    class OPCode35 : public Instruction {
      public:
        OPCode35(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x35, Maker) {}
        OPCode35(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x35, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x36
    class OPCode36 : public Instruction {
      public:
        OPCode36(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x36, Maker) {}
        OPCode36(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x36, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            int16_t second_arg = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            if ((second_arg == -0x1fe) || (second_arg == -0x1fd)) {
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            }
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            if (second_arg == -0x1fc) {
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            }
        }
    };
    // 0x37
    class OPCode37 : public Instruction {
      public:
        OPCode37(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x37, Maker) {}
        OPCode37(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x37, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x38
    class OPCode38 : public Instruction {
      public:
        OPCode38(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x38, Maker) {}
        OPCode38(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x38, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x39
    class OPCode39 : public Instruction {
      public:
        OPCode39(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x39, Maker) {}
        OPCode39(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x39, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch (code) {
                case 0xFF: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0C:
                case 0x0B:
                case 0x0A:
                case 0x69:
                case 0x05:
                case 0xFE: {
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x3A
    class OPCode3A : public Instruction {
      public:
        OPCode3A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3A, Maker) {}
        OPCode3A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x3B
    class OPCode3B : public Instruction {
      public:
        OPCode3B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3B, Maker) {}
        OPCode3B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x3C

    class OPCode3C : public Instruction {
      public:
        OPCode3C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3C, Maker) {}
        OPCode3C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3C, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            ssd::Buffer control_short2 = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short2));
            ssd::Buffer control_short3 = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short3));
            uint16_t short1 = ReadShortFromByteArray(0, control_short);
            ssd::Buffer control_short4 = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short4));

            if (short1 == 1) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            }
        }
    };
    // 0x3D
    class OPCode3D : public Instruction {
      public:
        OPCode3D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3D, Maker) {}
        OPCode3D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x3E
    class OPCode3E : public Instruction {
      public:
        OPCode3E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3E, Maker) {}
        OPCode3E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x3F
    class OPCode3F : public Instruction {
      public:
        OPCode3F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3F, Maker) {}
        OPCode3F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x09:
                case 0x0A:
                case 0x0B:
                case 0x01:
                case 0x06:
                case 0x05:
                case 0x04:
                case 0x10:
                case 0x13:
                case 0x00: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x08: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0D: {
                    for (int i = 0; i < 0x18; i++) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;
                }
                case 0x1A:
                case 0x0E:
                case 0x16: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x19: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x40
    class OPCode40 : public Instruction {
      public:
        OPCode40(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x40, Maker) {}
        OPCode40(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x40, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x41
    class OPCode41 : public Instruction {
      public:
        OPCode41(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x41, Maker) {}
        OPCode41(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x41, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };

    // 0x42
    class OPCode42 : public Instruction {
      public:
        OPCode42(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x42, Maker) {}
        OPCode42(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x42, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };

    // 0x43
    class OPCode43 : public Instruction {
      public:
        OPCode43(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x43, Maker) {}
        OPCode43(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x43, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x44
    class OPCode44 : public Instruction {
      public:
        OPCode44(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x44, Maker) {}
        OPCode44(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x44, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {

                case 0x00: {

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04:
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x06: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x07: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x08: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };

    // 0x45
    class OPCode45 : public Instruction {
      public:
        OPCode45(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x45, Maker) {}
        OPCode45(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x45, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode46 : public Instruction {
      public:
        OPCode46(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x46, Maker) {}
        OPCode46(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x46, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 3:
                case 1: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 2: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };

    // 0x47
    /*class OPCode47: public Instruction{
        OPCode47(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,4))); this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };*/
    // 0x47
    class OPCode47 : public Instruction {
      public:
        OPCode47(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x47, Maker) {}
        OPCode47(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x47, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x48
    class OPCode48 : public Instruction {
      public:
        OPCode48(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x48, Maker) {}
        OPCode48(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x48, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x49
    /*class OPCode49: public Instruction{
        OPCode49(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){

            case 0x0A:
            case 0x05:
            case 0x06:
            case 0x04:
            case 0x03:
            case 0x02:
            case 0x01:
            case 0x00:{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\r':{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x14:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x15:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));


                break;
            }
            case 0xE:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x17:{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x18:{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x19:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x1C:{
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '!':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '(':
            case '$':
            case '#':
            case '\v':
            case '3':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '&':{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case ')':{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '+':{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '4':
            case '0':
            case ',':{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '5':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case '7':{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '8':{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '9':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case ':':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case ';':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '<':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '=':{
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '>':{
                this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'D':
            case 'B':
            case '@':{
                this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

        }
        }

    };*/
    class OPCode49 : public Instruction {
      public:
        OPCode49(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x49, Maker) {}
        OPCode49(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x49, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x00: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x01: {

                    ssd::Buffer str = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", str));

                    ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                    auto fill = Operande(addr, "fill", remaining);
                    fill.set_bytes_to_fill(0x20);
                    this->add_operande(fill);
                    break;
                }
                case 0x0A:
                case 0x04:
                case 0x02:
                case 0x03: { // Here, not entirely sure, doesn't seem to be used tho. Example I found has 0x20 zeros.
                    ssd::Buffer str = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", str));

                    ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                    auto fill = Operande(addr, "fill", remaining);
                    fill.set_bytes_to_fill(0x20);
                    this->add_operande(fill);
                    break;
                }
                case 0x0B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0D: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0E: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x17: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x18: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x19: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x1A:
                case 0x1B:
                case 0x3D: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x1C: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x21: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x28:
                case 0x24:
                case 0x23: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x29: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    throw ssd::exceptions::parse_error("not sure here");
                    break;
                }
                case 0x2B: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    throw ssd::exceptions::parse_error("not sure here");
                }
                case 0x34:
                case 0x30: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x33: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x35: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x38: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x40:
                case 0x39: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x3A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x3B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x3C: {
                    throw ssd::exceptions::parse_error("not sure here");
                    break;
                }
                case 0x3E: {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));


                    break;
                }
                case 0x44: {

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x45:
                case 0x46: {

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x47: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x48: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
        }
    };

    // 0x4A
    class OPCode4A : public Instruction {
      public:
        OPCode4A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4A, Maker) {}
        OPCode4A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x4B
    class OPCode4B : public Instruction {
      public:
        OPCode4B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4B, Maker) {}
        OPCode4B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x4C
    class OPCode4C : public Instruction {
      public:
        OPCode4C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4C, Maker) {}
        OPCode4C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x4D
    class OPCode4D : public Instruction {
      public:
        OPCode4D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4D, Maker) {}
        OPCode4D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x4F
    /*class OPCode4F: public Instruction{
        OPCode4F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1)));
        }

    };*/
    // 0x4F
    class OPCode4F : public Instruction {
      public:
        OPCode4F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4F, Maker) {}
        OPCode4F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x50
    class OPCode50 : public Instruction {
      public:
        OPCode50(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x50, Maker) {}
        OPCode50(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x50, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
    // 0x51
    class OPCode51 : public Instruction {
      public:
        OPCode51(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x51, Maker) {}
        OPCode51(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x51, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x52
    class OPCode52 : public Instruction {
      public:
        OPCode52(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x52, Maker) {}
        OPCode52(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x52, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x53
    class OPCode53 : public Instruction {
      public:
        OPCode53(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x53, Maker) {}
        OPCode53(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x53, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            if ((unsigned char)control_byte[0] == 0) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    // 0x55
    class OPCode55 : public Instruction {
      public:
        OPCode55(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x55, Maker) {}
        OPCode55(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x55, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x56
    class OPCode56 : public Instruction {
      public:
        OPCode56(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x56, Maker) {}
        OPCode56(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x56, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x57
    class OPCode57 : public Instruction {
      public:
        OPCode57(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x57, Maker) {}
        OPCode57(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x57, Maker) {
            addr++;
        }
    };
    // 0x58
    class OPCode58 : public Instruction {
      public:
        OPCode58(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x58, Maker) {}
        OPCode58(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x58, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x59
    /*class OPCode59: public Instruction{
        OPCode59(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,2)));

        }

    };*/
    // 0x5A
    class OPCode5A : public Instruction {
      public:
        OPCode5A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5A, Maker) {}
        OPCode5A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };
    // 0x5B
    class OPCode5B : public Instruction {
      public:
        OPCode5B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5B, Maker) {}
        OPCode5B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x06:
                case 0x04:
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };
    // 0x5D
    /* class OPCode5D: public Instruction{
         OPCode5D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
     ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte));
             this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
         switch((unsigned char)control_byte[0]){
             case 0x01:
             case 0x00:{
                 this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                 break;
             }
         }
         }

     };*/
    // 0x5E
    class OPCode5D : public Instruction {
      public:
        OPCode5D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5D, Maker) {}
        OPCode5D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5D, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x04:
                case 0x03:
                case 0x02: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case '\a': {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case '\b': {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case '\v': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x0D:
                case 0x0C: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x0E: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x10: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x11: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x12: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x17: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x18: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0x5E
    class OPCode5E : public Instruction {
      public:
        OPCode5E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5E, Maker) {}
        OPCode5E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5E, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x03:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }

                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x0C:
                case 0x0B:
                case 0x0A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x05:
                case 0x06: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };

    // 0x5F
    /*class OPCode5F: public Instruction{
        OPCode5F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0B:
                case 0x09:
                case 0x07:
                case 0x08:
                case 0x04:
                case 0x13:
                case 0x01:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x05:
                case 0x03:
                case 0x02:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x06:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0A:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x0E:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr, 4)));;
                    break;
                }
                case 0x0D:
                case 0x0C:
                case 0x0F:
                case 0x12:{

                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x10:
                case 0x11:{
                    this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }


            }
        }

    };*/
    // 0x5F
    class OPCode5F : public Instruction {
      public:
        OPCode5F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5F, Maker) {}
        OPCode5F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                case 0x03: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x05: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0x61
    /*class OPCode61: public Instruction{
        OPCode61(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1)));; this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
        this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
        this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };*/
    // 0x60
    class OPCode60 : public Instruction {
      public:
        OPCode60(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x60, Maker) {}
        OPCode60(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x60, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x61
    class OPCode61 : public Instruction {
      public:
        OPCode61(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x61, Maker) {}
        OPCode61(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x61, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x63
    class OPCode63 : public Instruction {
      public:
        OPCode63(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x63, Maker) {}
        OPCode63(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x63, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x64
    class OPCode64 : public Instruction {
      public:
        OPCode64(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x64, Maker) {}
        OPCode64(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x64, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            switch ((unsigned char)control_byte[0]) {
                case 0x11: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x65
    class OPCode65 : public Instruction {
      public:
        OPCode65(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x65, Maker) {}
        OPCode65(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x65, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x09:
                case 0x04:
                case 0x0C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x08:
                case 0x07:
                case 0x0B:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x05:
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x06: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };

    // 0x66
    class OPCode66 : public Instruction {
      public:
        OPCode66(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x66, Maker) {}
        OPCode66(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x66, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x68
    class OPCode67 : public Instruction {
      public:
        OPCode67(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x67, Maker) {}
        OPCode67(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x67, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            switch (code) {
                case 0x02:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x03:
                case 0x04: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x68
    class OPCode68 : public Instruction {
      public:
        OPCode68(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x68, Maker) {}
        OPCode68(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x68, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
        }
    };
    // 0x69
    class OPCode69 : public Instruction {
      public:
        OPCode69(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x69, Maker) {}
        OPCode69(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x69, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }

                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0x6A
    class OPCode6A : public Instruction {
      public:
        OPCode6A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6A, Maker) {}
        OPCode6A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte2));

            if ((unsigned char)control_byte2[0] < 4) {
                switch ((unsigned char)control_byte[0]) {
                    case 0x00: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                        break;
                    }

                    case 0x01: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }
                    case 0x02: {

                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x04: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    }
                    case 0x05: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                        break;
                    }
                    case 0x09:
                    case 0x07:
                    case 0x06: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }
                }
            }
        }
    };
    // 0x6B
    class OPCode6B : public Instruction {
      public:
        OPCode6B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6B, Maker) {}
        OPCode6B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x6C
    class OPCode6C : public Instruction {
      public:
        OPCode6C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6C, Maker) {}
        OPCode6C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6C, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x6D
    class OPCode6D : public Instruction {
      public:
        OPCode6D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6D, Maker) {}
        OPCode6D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6D, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
                case 0x01: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x6E
    class OPCode6E : public Instruction {
      public:
        OPCode6E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6E, Maker) {}
        OPCode6E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6E, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0C: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
            }
        }
    };
    // 0x6F
    class OPCode6F : public Instruction {
      public:
        OPCode6F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6F, Maker) {}
        OPCode6F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x02:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
            }
        }
    };
    // 0x70
    class OPCode70 : public Instruction {
      public:
        OPCode70(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x70, Maker) {}
        OPCode70(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x70, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x01) {
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
        }
    };
    // 0x71
    /*class OPCode72: public Instruction{
        OPCode72(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); unsigned char code =
    control_byte[0]; if (code == 0x00){ this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
        }

    };*/
    // 0x72
    class OPCode72 : public Instruction {
      public:
        OPCode72(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x72, Maker) {}
        OPCode72(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x72, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x73

    class OPCode73 : public Instruction {
      public:
        OPCode73(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x73, Maker) {}
        OPCode73(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x73, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x03: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0xFF:
                case 0x06:
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x05: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x07: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0xFD:
                case 0xFE:
                case 0xFC: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x00:
                case 0x01:
                case 0x08:
                case 0x09: {
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0x74
    class OPCode74 : public Instruction {
      public:
        OPCode74(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x74, Maker) {}
        OPCode74(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x74, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x75
    class OPCode75 : public Instruction {
      public:
        OPCode75(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x75, Maker) {}
        OPCode75(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x75, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x78
    class OPCode78 : public Instruction {
      public:
        OPCode78(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x78, Maker) {}
        OPCode78(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x78, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x79

    class OPCode79 : public Instruction {
      public:
        OPCode79(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x79, Maker) {}
        OPCode79(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x79, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    return;
                    break;
                }
            }
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x7A
    class OPCode7A : public Instruction {
      public:
        OPCode7A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7A, Maker) {}
        OPCode7A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x7B
    /*class OPCode7B: public Instruction{
        OPCode7B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",

            this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };*/

    // 0x7B
    class OPCode7B : public Instruction {
      public:
        OPCode7B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7B, Maker) {}
        OPCode7B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7B, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x7D
    class OPCode7D : public Instruction {
      public:
        OPCode7D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7D, Maker) {}
        OPCode7D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7D, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x7E
    class OPCode7E : public Instruction {
      public:
        OPCode7E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7E, Maker) {}
        OPCode7E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x7F
    class OPCode7F : public Instruction {
      public:
        OPCode7F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7F, Maker) {}
        OPCode7F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch (control_byte[0]) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case -2: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case -1: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                }
            }
        }
    };
    // 0x80
    class OPCode80 : public Instruction {
      public:
        OPCode80(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x80, Maker) {}
        OPCode80(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x80, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    // 0x81
    class OPCode81 : public Instruction {
      public:
        OPCode81(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x81, Maker) {}
        OPCode81(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x81, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            } else if (code == 0x03) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            }
        }
    };
    // 0x82
    /*class OPCode82: public Instruction{
        OPCode82(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,2))); this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };*/
    // 0x82
    class OPCode82 : public Instruction {
      public:
        OPCode82(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x82, Maker) {}
        OPCode82(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x82, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x83
    class OPCode83 : public Instruction {
      public:
        OPCode83(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x83, Maker) {}
        OPCode83(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x83, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x84
    class OPCode84 : public Instruction {
      public:
        OPCode84(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x84, Maker) {}
        OPCode84(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x84, Maker) {
            addr++;
        }
    };
    // 0x85
    class OPCode85 : public Instruction {
      public:
        OPCode85(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x85, Maker) {}
        OPCode85(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x85, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x86
    class OPCode86 : public Instruction {
      public:
        OPCode86(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x86, Maker) {}
        OPCode86(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x86, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x63: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x64: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3: {

                    break;
                }
            }
        }
    };
    // 0x87
    class OPCode87 : public Instruction {
      public:
        OPCode87(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x87, Maker) {}
        OPCode87(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x87, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x88
    class OPCode88 : public Instruction {
      public:
        OPCode88(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x88, Maker) {}
        OPCode88(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x88, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
        }
    };

    // 0x89
    class OPCode89 : public Instruction {
      public:
        OPCode89(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x89, Maker) {}
        OPCode89(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x89, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if ((code == 0x00) || (code == 0x14)) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            } else if (code == 0x0A) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    // 0x8A
    class OPCode8A : public Instruction {
      public:
        OPCode8A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8A, Maker) {}
        OPCode8A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    // 0x8C
    class OPCode8C : public Instruction {
      public:
        OPCode8C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8C, Maker) {}
        OPCode8C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x8D
    /*class OPCode8D: public Instruction{
        OPCode8D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,2)));

        }
    };*/
    // 0x8D
    class OPCode8D : public Instruction {
      public:
        OPCode8D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8D, Maker) {}
        OPCode8D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8D, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "short", control_short));
            switch (control) {
                case 0x02:
                case 0x01: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    // 0x8E
    class OPCode8E : public Instruction {
      public:
        OPCode8E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8E, Maker) {}
        OPCode8E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x8F
    class OPCode8F : public Instruction {
      public:
        OPCode8F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8F, Maker) {}
        OPCode8F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x90
    class OPCode90 : public Instruction {
      public:
        OPCode90(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x90, Maker) {}
        OPCode90(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x90, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x91
    class OPCode91 : public Instruction {
      public:
        OPCode91(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x91, Maker) {}
        OPCode91(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x91, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };

    // 0x92
    class OPCode92 : public Instruction {
      public:
        OPCode92(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x92, Maker) {}
        OPCode92(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x92, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };

    // 0x93
    class OPCode93 : public Instruction {
      public:
        OPCode93(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x93, Maker) {}
        OPCode93(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x93, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte2));
            if ((unsigned char)control_byte2[0] == 1) {
                switch ((unsigned char)control_byte[0]) {
                    case 0x01: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }
                    case 0x03: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x05: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x08: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x0D: {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }
                    case 0x10: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x14:
                    case 0x12: {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    }
                    case 0x17: {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                        break;
                    }
                }
            }
        }
    };
    // 0x94
    class OPCode94 : public Instruction {
      public:
        OPCode94(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x94, Maker) {}
        OPCode94(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x94, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x95
    class OPCode95 : public Instruction {
      public:
        OPCode95(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x95, Maker) {}
        OPCode95(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x95, Maker) {
            addr++;
        }
    };
    // 0x96
    class OPCode96 : public Instruction {
      public:
        OPCode96(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x96, Maker) {}
        OPCode96(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x96, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x97
    class OPCode97 : public Instruction {
      public:
        OPCode97(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x97, Maker) {}
        OPCode97(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x97, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x98
    /*class OPCode98: public Instruction{
        OPCode98(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,2))); this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };*/
    // 0x99
    class OPCode99 : public Instruction {
      public:
        OPCode99(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x99, Maker) {}
        OPCode99(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x99, Maker) {
            addr++;
        }
    };
    // 0x9A
    class OPCode9A : public Instruction {
      public:
        OPCode9A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9A, Maker) {}
        OPCode9A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x9B
    /*class OPCode9B: public Instruction{
        OPCode9B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){ case 0x01: case 0x02:{ this->add_operande(Operande(addr,"short", ReadSubByteArray(content,
    addr,2))); this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));


                    break;
                }
                case 0x03:
                case 0x04:
                {
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
            }
        }
    };*/
    // 0x9B
    class OPCode9B : public Instruction {
      public:
        OPCode9B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9B, Maker) {}
        OPCode9B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x9D
    class OPCode9D : public Instruction {
      public:
        OPCode9D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9D, Maker) {}
        OPCode9D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x9E
    class OPCode9E : public Instruction {
      public:
        OPCode9E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9E, Maker) {}
        OPCode9E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x9F
    /*class OPCode9F: public Instruction{
        OPCode9F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1)));; this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content, addr)));

        }
    };*/
    // 0x9F
    class OPCode9F : public Instruction {
      public:
        OPCode9F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9F, Maker) {}
        OPCode9F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0xA0
    class OPCodeA0 : public Instruction {
      public:
        OPCodeA0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA0, Maker) {}
        OPCodeA0(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA0, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                case 0x04: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x02:
                case 0x05: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x03: {

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };
    // 0xA1
    class OPCodeA1 : public Instruction {
      public:
        OPCodeA1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA1, Maker) {}
        OPCodeA1(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA1, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x03:
                case 0x04:
                case 0x02:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };

    // 0xA2
    class OPCodeA2 : public Instruction {
      public:
        OPCodeA2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA2, Maker) {}
        OPCodeA2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA2, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0xA3
    /*class OPCodeA3: public Instruction{
        OPCodeA3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr,1))); this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
        }
    };*/
    // 0xA3
    class OPCodeA3 : public Instruction {
      public:
        OPCodeA3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA3, Maker) {}
        OPCodeA3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA3, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0xA4
    class OPCodeA4 : public Instruction {
      public:
        OPCodeA4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA4, Maker) {}
        OPCodeA4(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA4, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0xA6
    /*class OPCodeA6: public Instruction{
        OPCodeA6(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){


                case 0x03:
                case 0x01:
                case 0x04:
                case 0x0E:
                {
                    this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }

            }
        }
    };*/
    // 0xA6
    class OPCodeA6 : public Instruction {
      public:
        OPCodeA6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA6, Maker) {}
        OPCodeA6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA6, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0xA7
    class OPCodeA7 : public Instruction {
      public:
        OPCodeA7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA7, Maker) {}
        OPCodeA7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0xA8
    class OPCodeA8 : public Instruction {
      public:
        OPCodeA8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA8, Maker) {}
        OPCodeA8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA8, Maker) {
            addr++;

            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0xA9
    class OPCodeA9 : public Instruction {
      public:
        OPCodeA9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xA9, Maker) {}
        OPCodeA9(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA9, Maker) {
            addr++;
        }
    };
    // 0xAA
    class OPCodeAA : public Instruction {
      public:
        OPCodeAA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAA, Maker) {}
        OPCodeAA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAA, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0xAB
    class OPCodeAB : public Instruction {
      public:
        OPCodeAB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAB, Maker) {}
        OPCodeAB(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAB, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0xAC
    class OPCodeAC : public Instruction {
      public:
        OPCodeAC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAC, Maker) {}
        OPCodeAC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAC, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0xAD
    class OPCodeAD : public Instruction {
      public:
        OPCodeAD(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAD, Maker) {}
        OPCodeAD(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAD, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0xAE
    class OPCodeAE : public Instruction {
      public:
        OPCodeAE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAE, Maker) {}
        OPCodeAE(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAE, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0xAF
    class OPCodeAF : public Instruction {
      public:
        OPCodeAF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xAF, Maker) {}
        OPCodeAF(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xAF, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0x65: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x66: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x67: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x69: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0xB0
    /*class OPCodeB0: public Instruction{
        OPCodeB0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){ case 0x02:{ this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
            }



        }
    };*/
    // 0xB0
    class OPCodeB0 : public Instruction {
      public:
        OPCodeB0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB0, Maker) {}
        OPCodeB0(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB0, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0xB1
    class OPCodeB1 : public Instruction {
      public:
        OPCodeB1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB1, Maker) {}
        OPCodeB1(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB1, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x04:
                case 0x03: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x06: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0D:
                case 0x0C: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0E: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0F: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };
    // 0xB2
    class OPCodeB2 : public Instruction {
      public:
        OPCodeB2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB2, Maker) {}
        OPCodeB2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB2, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0x01: { // this one is REALLY strange. two times I found that they added two missing floats (or
                             // int)
                    // in the files, the executable doesn't parse them right, not sure what to do
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    // these might need to be removed
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x0A:
                case 0x0B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x0C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x0D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x12: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };

    // 0xB3
    /*class OPCodeB3: public Instruction{
        OPCodeB3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 1); this->add_operande(Operande(addr,"byte", control_byte)); switch((unsigned
    char)control_byte[0]){ case 0x00:{ this->add_operande(Operande(addr,"string", ReadStringSubByteArray(content,
    addr))); break;
                }
                case 0x01:{
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x03:
                {
                    this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0A:
                {
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }



        }
    };*/
    // 0xB3
    class OPCodeB3 : public Instruction {
      public:
        OPCodeB3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB3, Maker) {}
        OPCodeB3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB3, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0xB5
    /*class OPCodeB5: public Instruction{
        OPCodeB5(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???",
    ReadSubByteArray(content, addr, 2))); this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr,"byte", control_byte));
            this->add_operande(Operande(addr,"float", ReadSubByteArray(content, addr,4)));



        }
    };*/
    // 0xB5
    class OPCodeB5 : public Instruction {
      public:
        OPCodeB5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB5, Maker) {}
        OPCodeB5(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB5, Maker) {
            addr++;
            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));
            switch (control) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 100: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0xB6
    class OPCodeB6 : public Instruction {
      public:
        OPCodeB6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB6, Maker) {}
        OPCodeB6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB6, Maker) {
            addr++;
            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));
            ssd::Buffer control_ba2 = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_ba2));
            switch (control) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x06:
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0xB7
    class OPCodeB7 : public Instruction {
      public:
        OPCodeB7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB7, Maker) {}
        OPCodeB7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB7, Maker) {
            addr++;
            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));
            switch (control) {
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
            }
        }
    };
    // 0xB8
    class OPCodeB8 : public Instruction {
      public:
        OPCodeB8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB8, Maker) {}
        OPCodeB8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB8, Maker) {
            addr++;

            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_ba));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0xB9
    class OPCodeB9 : public Instruction {
      public:
        OPCodeB9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xB9, Maker) {}
        OPCodeB9(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xB9, Maker) {
            addr++;

            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));

            switch (control) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

                case 0x02: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x04: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x07: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x09: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x2C:
                case 0x0F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x16: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x18:

                {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0A:

                {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0B:

                {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0C:

                {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0D:

                {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0E: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x2E:
                case 0x1A:
                case 0x1B:
                case 0x17: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1D:
                case 0x1E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x20:
                case 0x24: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x21: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x22: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x23: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x27: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x29: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x2B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x30: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x31: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x32: {

                    break;
                }
                case 0x36: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x34:
                case 0x38: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x39: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x37:
                case 0x3B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3C: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3E: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x3F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x19:
                case 0x40:
                case 0x28: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x42: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x41:
                case 0x43: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x44: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x45: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x46:
                case 0x4A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3A:
                case 0x48: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x47: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x49: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x54: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                default:
                    throw ssd::exceptions::not_analyzed_yet(control);
            }
        }
    };
    // 0xBA
    class OPCodeBA : public Instruction {
      public:
        OPCodeBA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xBA, Maker) {}
        OPCodeBA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xBA, Maker) {
            addr++;

            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));

            switch (control) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x02:

                {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

                case 0x06: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x40:
                case 0x41: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x9: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0A: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
        }
    };
    // 0xBB
    class OPCodeBB : public Instruction {
      public:
        OPCodeBB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xBB, Maker) {}
        OPCodeBB(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xBB, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeBC : public Instruction {
      public:
        OPCodeBC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xBC, Maker) {}
        OPCodeBC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xBC, Maker) {
            addr++;

            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_ba);
            this->add_operande(Operande(addr, "short", control_ba));

            switch (control) {
                case 0x08:
                case 0x07:
                case 0x2E:
                case 0x3A:
                case 0x3C:
                case 0x3F:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02:
                case 0x3B:
                case 0x3D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x18:
                case 0x15:
                case 0x14:
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x40:
                case 0x41: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x17:
                case 0x16:
                case 0x0A:
                case 0x04:
                case 0x28:
                case 0x29:
                case 0x38:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4A:
                case 0x4D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x2D:
                case 0x44:
                case 0x45:
                case 0x5B:
                case 0x5F: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x35: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    class OPCodeBE : public Instruction {
      public:
        OPCodeBE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xBE, Maker) {}
        OPCodeBE(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xBE, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeBF : public Instruction {
      public:
        OPCodeBF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xBF, Maker) {}
        OPCodeBF(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xBF, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeC0 : public Instruction {
      public:
        OPCodeC0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC0, Maker) {}
        OPCodeC0(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC0, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeC1 : public Instruction {
      public:
        OPCodeC1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC1, Maker) {}
        OPCodeC1(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC1, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeC2 : public Instruction {
      public:
        OPCodeC2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC2, Maker) {}
        OPCodeC2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC2, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeC3 : public Instruction {
      public:
        OPCodeC3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC3, Maker) {}
        OPCodeC3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC3, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x02:
                case 0x01: {
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
            }
        }
    };
    class OPCodeC4 : public Instruction {
      public:
        OPCodeC4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC4, Maker) {}
        OPCodeC4(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC4, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeC5 : public Instruction {
      public:
        OPCodeC5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC5, Maker) {}
        OPCodeC5(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC5, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    class OPCodeC7 : public Instruction {
      public:
        OPCodeC7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC7, Maker) {}
        OPCodeC7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCodeC8 : public Instruction {
      public:
        OPCodeC8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC8, Maker) {}
        OPCodeC8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC8, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            ssd::Buffer control_ba = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_ba));
        }
    };
    class OPCodeC9 : public Instruction {
      public:
        OPCodeC9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xC9, Maker) {}
        OPCodeC9(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC9, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x01: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    class OPCodeCA : public Instruction {
      public:
        OPCodeCA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xCA, Maker) {}
        OPCodeCA(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xCA, Maker) {
            addr++;
        }
    };
    std::shared_ptr<Instruction> create_instruction_from_dat(int& addr, ssd::Buffer& dat_content, int function_type) override {
        int OP = (dat_content[addr] & 0xFF);

        if (TXUIFiles.contains(scene_name) && (OP == 0x13)) {
            return std::make_shared<UI_OP13>(addr, dat_content, this); // UI files have a special 0x13 instruction
        }

        if (function_type == 0) { // the function is read with OPCodes

            switch (OP) {
                case 0x00:
                    return std::make_shared<OPCode0>(addr, dat_content, this);
                case 0x01:
                    return std::make_shared<OPCode1>(addr, dat_content, this);
                case 0x02:
                    return std::make_shared<OPCode02>(addr, dat_content, this);
                case 0x03:
                    return std::make_shared<OPCode03>(addr, dat_content, this);
                case 0x04:
                    return std::make_shared<OPCode04>(addr, dat_content, this);
                case 0x05:
                    return std::make_shared<OPCode05>(addr, dat_content, this);
                case 0x06:
                    return std::make_shared<OPCode06>(addr, dat_content, this);
                case 0x07:
                    return std::make_shared<OPCode07>(addr, dat_content, this);
                case 0x08:
                    return std::make_shared<OPCode08>(addr, dat_content, this);
                case 0x0A:
                    return std::make_shared<OPCode0A>(addr, dat_content, this);
                case 0x0C:
                    return std::make_shared<OPCode0C>(addr, dat_content, this);
                case 0x0D:
                    return std::make_shared<OPCode0D>(addr, dat_content, this);
                case 0x0E:
                    return std::make_shared<OPCode0E>(addr, dat_content, this);
                case 0x0F:
                    return std::make_shared<OPCode0F>(addr, dat_content, this);
                case 0x10:
                    return std::make_shared<OPCode10>(addr, dat_content, this);
                case 0x11:
                    return std::make_shared<OPCode11>(addr, dat_content, this);
                case 0x12:
                    return std::make_shared<OPCode12>(addr, dat_content, this);
                case 0x13:
                    return std::make_shared<OPCode13>(addr, dat_content, this);
                case 0x14:
                    return std::make_shared<OPCode14>(addr, dat_content, this);
                case 0x15:
                    return std::make_shared<OPCode15>(addr, dat_content, this);
                case 0x16:
                    return std::make_shared<OPCode16>(addr, dat_content, this);
                case 0x17:
                    return std::make_shared<OPCode17>(addr, dat_content, this);
                case 0x18:
                    return std::make_shared<OPCode18>(addr, dat_content, this);
                case 0x19:
                    return std::make_shared<OPCode19>(addr, dat_content, this);
                case 0x1A:
                    return std::make_shared<OPCode1A>(addr, dat_content, this);
                case 0x1B:
                    return std::make_shared<OPCode1B>(addr, dat_content, this);
                case 0x1C:
                    return std::make_shared<OPCode1C>(addr, dat_content, this);
                case 0x1D:
                    return std::make_shared<OPCode1D>(addr, dat_content, this);
                case 0x1E:
                    return std::make_shared<OPCode1E>(addr, dat_content, this);
                case 0x1F:
                    return std::make_shared<OPCode1F>(addr, dat_content, this);
                case 0x20:
                    return std::make_shared<OPCode20>(addr, dat_content, this);
                    //                case 0x21: return std::make_shared<OPCode21>(addr,dat_content,this);
                case 0x22:
                    return std::make_shared<OPCode22>(addr, dat_content, this);
                case 0x23:
                    return std::make_shared<OPCode23>(addr, dat_content, this);
                case 0x24:
                    return std::make_shared<OPCode24>(addr, dat_content, this);
                case 0x25:
                    return std::make_shared<OPCode25>(addr, dat_content, this);
                case 0x26:
                    return std::make_shared<OPCode26>(addr, dat_content, this);
                case 0x27:
                    return std::make_shared<OPCode27>(addr, dat_content, this);
                case 0x28:
                    return std::make_shared<OPCode28>(addr, dat_content, this);
                case 0x29:
                    return std::make_shared<OPCode29>(addr, dat_content, this);
                case 0x2A:
                    return std::make_shared<OPCode2A>(addr, dat_content, this);
                case 0x2B:
                    return std::make_shared<OPCode2B>(addr, dat_content, this);
                case 0x2C:
                    return std::make_shared<OPCode2C>(addr, dat_content, this);
                case 0x2D:
                    return std::make_shared<OPCode2D>(addr, dat_content, this);
                case 0x2E:
                    return std::make_shared<OPCode2E>(addr, dat_content, this);
                case 0x2F:
                    return std::make_shared<OPCode2F>(addr, dat_content, this);
                case 0x30:
                    return std::make_shared<OPCode30>(addr, dat_content, this);
                case 0x31:
                    return std::make_shared<OPCode31>(addr, dat_content, this);
                case 0x32:
                    return std::make_shared<OPCode32>(addr, dat_content, this);
                case 0x33:
                    return std::make_shared<OPCode33>(addr, dat_content, this);
                case 0x34:
                    return std::make_shared<OPCode34>(addr, dat_content, this);
                case 0x35:
                    return std::make_shared<OPCode35>(addr, dat_content, this);
                case 0x36:
                    return std::make_shared<OPCode36>(addr, dat_content, this);
                case 0x37:
                    return std::make_shared<OPCode37>(addr, dat_content, this);
                case 0x38:
                    return std::make_shared<OPCode38>(addr, dat_content, this);
                case 0x39:
                    return std::make_shared<OPCode39>(addr, dat_content, this);
                case 0x3A:
                    return std::make_shared<OPCode3A>(addr, dat_content, this);
                case 0x3B:
                    return std::make_shared<OPCode3B>(addr, dat_content, this);
                case 0x3C:
                    return std::make_shared<OPCode3C>(addr, dat_content, this);
                case 0x3D:
                    return std::make_shared<OPCode3D>(addr, dat_content, this);
                case 0x3E:
                    return std::make_shared<OPCode3E>(addr, dat_content, this);
                case 0x3F:
                    return std::make_shared<OPCode3F>(addr, dat_content, this);
                case 0x40:
                    return std::make_shared<OPCode40>(addr, dat_content, this);
                case 0x41:
                    return std::make_shared<OPCode41>(addr, dat_content, this);
                case 0x42:
                    return std::make_shared<OPCode42>(addr, dat_content, this);
                case 0x43:
                    return std::make_shared<OPCode43>(addr, dat_content, this);
                case 0x44:
                    return std::make_shared<OPCode44>(addr, dat_content, this);
                case 0x45:
                    return std::make_shared<OPCode45>(addr, dat_content, this);
                case 0x46:
                    return std::make_shared<OPCode46>(addr, dat_content, this);
                case 0x47:
                    return std::make_shared<OPCode47>(addr, dat_content, this);
                case 0x48:
                    return std::make_shared<OPCode48>(addr, dat_content, this);
                case 0x49:
                    return std::make_shared<OPCode49>(addr, dat_content, this);
                case 0x4A:
                    return std::make_shared<OPCode4A>(addr, dat_content, this);
                case 0x4B:
                    return std::make_shared<OPCode4B>(addr, dat_content, this);
                case 0x4C:
                    return std::make_shared<OPCode4C>(addr, dat_content, this);
                case 0x4D:
                    return std::make_shared<OPCode4D>(addr, dat_content, this);
                    //                case 0x4E: return std::make_shared<OPCode4E>(addr,dat_content,this);
                case 0x4F:
                    return std::make_shared<OPCode4F>(addr, dat_content, this);
                case 0x50:
                    return std::make_shared<OPCode50>(addr, dat_content, this);
                case 0x51:
                    return std::make_shared<OPCode51>(addr, dat_content, this);
                case 0x52:
                    return std::make_shared<OPCode52>(addr, dat_content, this);
                case 0x53:
                    return std::make_shared<OPCode53>(addr, dat_content, this);
                    //                case 0x54: return std::make_shared<OPCode54>(addr,dat_content,this);
                case 0x55:
                    return std::make_shared<OPCode55>(addr, dat_content, this);
                case 0x56:
                    return std::make_shared<OPCode56>(addr, dat_content, this);
                case 0x57:
                    return std::make_shared<OPCode57>(addr, dat_content, this);
                case 0x58:
                    return std::make_shared<OPCode58>(addr, dat_content, this);
                    //                case 0x59: return std::make_shared<OPCode59>(addr,dat_content,this);
                case 0x5A:
                    return std::make_shared<OPCode5A>(addr, dat_content, this);
                case 0x5B:
                    return std::make_shared<OPCode5B>(addr, dat_content, this);
                    //                case 0x5C: return std::make_shared<OPCode5C>(addr,dat_content,this);
                case 0x5D:
                    return std::make_shared<OPCode5D>(addr, dat_content, this);
                case 0x5E:
                    return std::make_shared<OPCode5E>(addr, dat_content, this);
                case 0x5F:
                    return std::make_shared<OPCode5F>(addr, dat_content, this);
                case 0x60:
                    return std::make_shared<OPCode60>(addr, dat_content, this);
                case 0x61:
                    return std::make_shared<OPCode61>(addr, dat_content, this);
                // case 0x62: return std::make_shared<OPCode62>(addr,dat_content,this);
                case 0x63:
                    return std::make_shared<OPCode63>(addr, dat_content, this);
                case 0x64:
                    return std::make_shared<OPCode64>(addr, dat_content, this);
                case 0x65:
                    return std::make_shared<OPCode65>(addr, dat_content, this);
                case 0x66:
                    return std::make_shared<OPCode66>(addr, dat_content, this);
                case 0x67:
                    return std::make_shared<OPCode67>(addr, dat_content, this);
                case 0x68:
                    return std::make_shared<OPCode68>(addr, dat_content, this);
                case 0x69:
                    return std::make_shared<OPCode69>(addr, dat_content, this);
                case 0x6A:
                    return std::make_shared<OPCode6A>(addr, dat_content, this);
                case 0x6B:
                    return std::make_shared<OPCode6B>(addr, dat_content, this);
                case 0x6C:
                    return std::make_shared<OPCode6C>(addr, dat_content, this);
                case 0x6D:
                    return std::make_shared<OPCode6D>(addr, dat_content, this);
                case 0x6E:
                    return std::make_shared<OPCode6E>(addr, dat_content, this);
                case 0x6F:
                    return std::make_shared<OPCode6F>(addr, dat_content, this);
                case 0x70:
                    return std::make_shared<OPCode70>(addr, dat_content, this);
                    //                case 0x71: return std::make_shared<OPCode71>(addr,dat_content,this);
                case 0x72:
                    return std::make_shared<OPCode72>(addr, dat_content, this);
                case 0x73:
                    return std::make_shared<OPCode73>(addr, dat_content, this);
                case 0x74:
                    return std::make_shared<OPCode74>(addr, dat_content, this);
                case 0x75:
                    return std::make_shared<OPCode75>(addr, dat_content, this);
                    //                case 0x76: return std::make_shared<OPCode76>(addr,dat_content,this);
                    //                case 0x78: return std::make_shared<OPCode78>(addr,dat_content,this);
                case 0x79:
                    return std::make_shared<OPCode79>(addr, dat_content, this);
                    //                case 0x7A: return std::make_shared<OPCode7A>(addr,dat_content,this);
                case 0x7B:
                    return std::make_shared<OPCode7B>(addr, dat_content, this);
                    //                case 0x7C: return std::make_shared<OPCode7C>(addr,dat_content,this);
                case 0x7D:
                    return std::make_shared<OPCode7D>(addr, dat_content, this);
                case 0x7E:
                    return std::make_shared<OPCode7E>(addr, dat_content, this);
                case 0x7F:
                    return std::make_shared<OPCode7F>(addr, dat_content, this);
                case 0x80:
                    return std::make_shared<OPCode80>(addr, dat_content, this);
                case 0x81:
                    return std::make_shared<OPCode81>(addr, dat_content, this);
                case 0x82:
                    return std::make_shared<OPCode82>(addr, dat_content, this);
                case 0x83:
                    return std::make_shared<OPCode83>(addr, dat_content, this);
                case 0x84:
                    return std::make_shared<OPCode84>(addr, dat_content, this);
                case 0x85:
                    return std::make_shared<OPCode85>(addr, dat_content, this);
                case 0x86:
                    return std::make_shared<OPCode86>(addr, dat_content, this);
                case 0x87:
                    return std::make_shared<OPCode87>(addr, dat_content, this);
                case 0x88:
                    return std::make_shared<OPCode88>(addr, dat_content, this);
                case 0x89:
                    return std::make_shared<OPCode89>(addr, dat_content, this);
                case 0x8A:
                    return std::make_shared<OPCode8A>(addr, dat_content, this);
                    //                case 0x8B: return std::make_shared<OPCode8B>(addr,dat_content,this);
                case 0x8C:
                    return std::make_shared<OPCode8C>(addr, dat_content, this);
                case 0x8D:
                    return std::make_shared<OPCode8D>(addr, dat_content, this);
                case 0x8E:
                    return std::make_shared<OPCode8E>(addr, dat_content, this);
                case 0x8F:
                    return std::make_shared<OPCode8F>(addr, dat_content, this);
                case 0x90:
                    return std::make_shared<OPCode90>(addr, dat_content, this);
                    //                case 0x91: return std::make_shared<OPCode91>(addr,dat_content,this);
                case 0x92:
                    return std::make_shared<OPCode92>(addr, dat_content, this);
                case 0x93:
                    return std::make_shared<OPCode93>(addr, dat_content, this);
                case 0x94:
                    return std::make_shared<OPCode94>(addr, dat_content, this);
                case 0x95:
                    return std::make_shared<OPCode95>(addr, dat_content, this);
                case 0x96:
                    return std::make_shared<OPCode96>(addr, dat_content, this);
                case 0x97:
                    return std::make_shared<OPCode97>(addr, dat_content, this);
                    //                case 0x98: return std::make_shared<OPCode98>(addr,dat_content,this);
                    //                //case 0x99: return std::make_shared<OPCode99>(addr,dat_content,this);
                    //                case 0x9A: return std::make_shared<OPCode9A>(addr,dat_content,this);
                case 0x9B:
                    return std::make_shared<OPCode9B>(addr, dat_content, this);
                    //                case 0x9C: return std::make_shared<OPCode9C>(addr,dat_content,this);
                case 0x9D:
                    return std::make_shared<OPCode9D>(addr, dat_content, this);
                case 0x9E:
                    return std::make_shared<OPCode9E>(addr, dat_content, this);
                case 0x9F:
                    return std::make_shared<OPCode9F>(addr, dat_content, this);
                case 0xA0:
                    return std::make_shared<OPCodeA0>(addr, dat_content, this);
                case 0xA1:
                    return std::make_shared<OPCodeA1>(addr, dat_content, this);
                case 0xA2:
                    return std::make_shared<OPCodeA2>(addr, dat_content, this);
                case 0xA3:
                    return std::make_shared<OPCodeA3>(addr, dat_content, this);
                case 0xA4:
                    return std::make_shared<OPCodeA4>(addr, dat_content, this);
                    //                case 0xA5: return std::make_shared<OPCodeA5>(addr,dat_content,this);
                case 0xA6:
                    return std::make_shared<OPCodeA6>(addr, dat_content, this);
                case 0xA7:
                    return std::make_shared<OPCodeA7>(addr, dat_content, this);
                case 0xA8:
                    return std::make_shared<OPCodeA8>(addr, dat_content, this);
                case 0xA9:
                    return std::make_shared<OPCodeA9>(addr, dat_content, this);
                case 0xAA:
                    return std::make_shared<OPCodeAA>(addr, dat_content, this);
                case 0xAB:
                    return std::make_shared<OPCodeAB>(addr, dat_content, this);
                case 0xAC:
                    return std::make_shared<OPCodeAC>(addr, dat_content, this);
                case 0xAD:
                    return std::make_shared<OPCodeAD>(addr, dat_content, this);
                    //                case 0xAE: return std::make_shared<OPCodeAE>(addr,dat_content,this);
                case 0xAF:
                    return std::make_shared<OPCodeAF>(addr, dat_content, this);
                case 0xB0:
                    return std::make_shared<OPCodeB0>(addr, dat_content, this);
                case 0xB1:
                    return std::make_shared<OPCodeB1>(addr, dat_content, this);
                case 0xB2:
                    return std::make_shared<OPCodeB2>(addr, dat_content, this);
                case 0xB3:
                    return std::make_shared<OPCodeB3>(addr, dat_content, this);
                    //                case 0xB4: return std::make_shared<OPCodeB4>(addr,dat_content,this);
                case 0xB5:
                    return std::make_shared<OPCodeB5>(addr, dat_content, this);
                case 0xB6:
                    return std::make_shared<OPCodeB6>(addr, dat_content, this);
                case 0xB7:
                    return std::make_shared<OPCodeB7>(addr, dat_content, this);
                case 0xB8:
                    return std::make_shared<OPCodeB8>(addr, dat_content, this);
                case 0xB9:
                    return std::make_shared<OPCodeB9>(addr, dat_content, this);
                case 0xBA:
                    return std::make_shared<OPCodeBA>(addr, dat_content, this);
                case 0xBB:
                    return std::make_shared<OPCodeBB>(addr, dat_content, this);
                case 0xBC:
                    return std::make_shared<OPCodeBC>(addr, dat_content, this);
                case 0xBE:
                    return std::make_shared<OPCodeBE>(addr, dat_content, this);
                case 0xBF:
                    return std::make_shared<OPCodeBF>(addr, dat_content, this);
                case 0xC0:
                    return std::make_shared<OPCodeC0>(addr, dat_content, this);
                case 0xC1:
                    return std::make_shared<OPCodeC1>(addr, dat_content, this);
                case 0xC2:
                    return std::make_shared<OPCodeC2>(addr, dat_content, this);
                case 0xC3:
                    return std::make_shared<OPCodeC3>(addr, dat_content, this);
                case 0xC4:
                    return std::make_shared<OPCodeC4>(addr, dat_content, this);
                case 0xC5:
                    return std::make_shared<OPCodeC5>(addr, dat_content, this);
                case 0xC7:
                    return std::make_shared<OPCodeC7>(addr, dat_content, this);
                case 0xC8:
                    return std::make_shared<OPCodeC8>(addr, dat_content, this);
                case 0xC9:
                    return std::make_shared<OPCodeC9>(addr, dat_content, this);
                case 0xCA:
                    return std::make_shared<OPCodeCA>(addr, dat_content, this);
                default:
                    throw ssd::exceptions::bad_opcode(OP, addr);
            }
        } else {
            std::shared_ptr<Instruction> res;
            if (function_type == 2) { // the function is a "effect" function
                res = std::make_shared<EffectsInstr>(addr, dat_content, this);
            } else if (function_type == 10) {
                res =  std::make_shared<AnimeClipTable>(addr, dat_content, this);
            } else if (function_type == 11) {
                res =  std::make_shared<FieldMonsterData>(addr, dat_content, this);
            } else if (function_type == 13) {
                res =  std::make_shared<FC_autoX>(addr, dat_content, this);
            } else if (function_type == 14) {
                res =  std::make_shared<BookData99>(addr, dat_content, this);
            } else if (function_type == 15) {
                res =  std::make_shared<BookDataX>(addr, dat_content, this);
            }
            if ((uint8_t)dat_content[addr] != 1) throw ssd::exceptions::past_the_end_addr();
            if (this->goal < addr) throw ssd::exceptions::past_the_end_addr();
            return res;
        }

        return {};
    }
    bool create_header_from_dat(ssd::Buffer& dat_content) override {
        display_text("Reading header...");
        int nb_functions = ReadIntegerFromByteArray(0x14, dat_content);
        int position_filename = ReadIntegerFromByteArray(0x4, dat_content);
        int position = position_filename;
        int next_position = 0;
        scene_name = ReadStringFromByteArray(position, dat_content);
        int start_offset_area = ReadIntegerFromByteArray(0x8, dat_content);
        for (int idx_fun = 0; idx_fun < nb_functions; idx_fun++) {
            position = start_offset_area + 4 * idx_fun;
            next_position = start_offset_area + 4 * (idx_fun + 1);
            int addr = ReadIntegerFromByteArray(position, dat_content);
            position = start_offset_area + 4 * nb_functions + 2 * idx_fun;
            int16_t name_pos = ReadShortFromByteArray(position, dat_content);
            int name_pos_int = name_pos;
            std::string function_name = ReadStringFromByteArray(name_pos_int, dat_content);
            size_t end_addr = 0;
            if (idx_fun == nb_functions - 1) { // we are at the last function, so it ends at the end of the file
                end_addr = dat_content.size();
            } else {
                end_addr = ReadIntegerFromByteArray(next_position, dat_content);
            }
            functions_to_parse.emplace_back(idx_fun, function_name, name_pos, addr, end_addr);
        }
        display_text("Header parsed.");
        return true;
    }
    std::shared_ptr<Instruction> create_instruction_from_xlsx(int& addr, int opcode, int row, QXlsx::Document& xls_content) override {
        if (TXUIFiles.contains(scene_name) && (opcode == 0x13)) {
            return std::make_shared<UI_OP13>(addr, row, xls_content, this);
        }

        switch (opcode) {
            case 0x00:
                return std::make_shared<OPCode0>(addr, row, xls_content, this);
            case 0x01:
                return std::make_shared<OPCode1>(addr, row, xls_content, this);
            case 0x02:
                return std::make_shared<OPCode02>(addr, row, xls_content, this);
            case 0x03:
                return std::make_shared<OPCode03>(addr, row, xls_content, this);
            case 0x04:
                return std::make_shared<OPCode04>(addr, row, xls_content, this);
            case 0x05:
                return std::make_shared<OPCode05>(addr, row, xls_content, this);
            case 0x06:
                return std::make_shared<OPCode06>(addr, row, xls_content, this);
            case 0x07:
                return std::make_shared<OPCode07>(addr, row, xls_content, this);
            case 0x08:
                return std::make_shared<OPCode08>(addr, row, xls_content, this);
            case 0x0A:
                return std::make_shared<OPCode0A>(addr, row, xls_content, this);
            case 0x0C:
                return std::make_shared<OPCode0C>(addr, row, xls_content, this);
            case 0x0D:
                return std::make_shared<OPCode0D>(addr, row, xls_content, this);
            case 0x0E:
                return std::make_shared<OPCode0E>(addr, row, xls_content, this);
            case 0x0F:
                return std::make_shared<OPCode0F>(addr, row, xls_content, this);
            case 0x10:
                return std::make_shared<OPCode10>(addr, row, xls_content, this);
            case 0x11:
                return std::make_shared<OPCode11>(addr, row, xls_content, this);
            case 0x12:
                return std::make_shared<OPCode12>(addr, row, xls_content, this);
            case 0x13:
                return std::make_shared<OPCode13>(addr, row, xls_content, this);
            case 0x14:
                return std::make_shared<OPCode14>(addr, row, xls_content, this);
            case 0x15:
                return std::make_shared<OPCode15>(addr, row, xls_content, this);
            case 0x16:
                return std::make_shared<OPCode16>(addr, row, xls_content, this);
            case 0x17:
                return std::make_shared<OPCode17>(addr, row, xls_content, this);
            case 0x18:
                return std::make_shared<OPCode18>(addr, row, xls_content, this);
            case 0x19:
                return std::make_shared<OPCode19>(addr, row, xls_content, this);
            case 0x1A:
                return std::make_shared<OPCode1A>(addr, row, xls_content, this);
            case 0x1B:
                return std::make_shared<OPCode1B>(addr, row, xls_content, this);
            case 0x1C:
                return std::make_shared<OPCode1C>(addr, row, xls_content, this);
            case 0x1D:
                return std::make_shared<OPCode1D>(addr, row, xls_content, this);
            case 0x1E:
                return std::make_shared<OPCode1E>(addr, row, xls_content, this);
            case 0x1F:
                return std::make_shared<OPCode1F>(addr, row, xls_content, this);
            case 0x20:
                return std::make_shared<OPCode20>(addr, row, xls_content, this);
                //                case 0x21: return std::make_shared<OPCode21>(addr, row, xls_content,this);
            case 0x22:
                return std::make_shared<OPCode22>(addr, row, xls_content, this);
            case 0x23:
                return std::make_shared<OPCode23>(addr, row, xls_content, this);
            case 0x24:
                return std::make_shared<OPCode24>(addr, row, xls_content, this);
            case 0x25:
                return std::make_shared<OPCode25>(addr, row, xls_content, this);
            case 0x26:
                return std::make_shared<OPCode26>(addr, row, xls_content, this);
            case 0x27:
                return std::make_shared<OPCode27>(addr, row, xls_content, this);
            case 0x28:
                return std::make_shared<OPCode28>(addr, row, xls_content, this);
            case 0x29:
                return std::make_shared<OPCode29>(addr, row, xls_content, this);
            case 0x2A:
                return std::make_shared<OPCode2A>(addr, row, xls_content, this);
            case 0x2B:
                return std::make_shared<OPCode2B>(addr, row, xls_content, this);
            case 0x2C:
                return std::make_shared<OPCode2C>(addr, row, xls_content, this);
            case 0x2D:
                return std::make_shared<OPCode2D>(addr, row, xls_content, this);
            case 0x2E:
                return std::make_shared<OPCode2E>(addr, row, xls_content, this);
            case 0x2F:
                return std::make_shared<OPCode2F>(addr, row, xls_content, this);
            case 0x30:
                return std::make_shared<OPCode30>(addr, row, xls_content, this);
            case 0x31:
                return std::make_shared<OPCode31>(addr, row, xls_content, this);
            case 0x32:
                return std::make_shared<OPCode32>(addr, row, xls_content, this);
            case 0x33:
                return std::make_shared<OPCode33>(addr, row, xls_content, this);
            case 0x34:
                return std::make_shared<OPCode34>(addr, row, xls_content, this);
            case 0x35:
                return std::make_shared<OPCode35>(addr, row, xls_content, this);
            case 0x36:
                return std::make_shared<OPCode36>(addr, row, xls_content, this);
            case 0x37:
                return std::make_shared<OPCode37>(addr, row, xls_content, this);
            case 0x38:
                return std::make_shared<OPCode38>(addr, row, xls_content, this);
            case 0x39:
                return std::make_shared<OPCode39>(addr, row, xls_content, this);
            case 0x3A:
                return std::make_shared<OPCode3A>(addr, row, xls_content, this);
            case 0x3B:
                return std::make_shared<OPCode3B>(addr, row, xls_content, this);
            case 0x3C:
                return std::make_shared<OPCode3C>(addr, row, xls_content, this);
            case 0x3D:
                return std::make_shared<OPCode3D>(addr, row, xls_content, this);
            case 0x3E:
                return std::make_shared<OPCode3E>(addr, row, xls_content, this);
            case 0x3F:
                return std::make_shared<OPCode3F>(addr, row, xls_content, this);
            case 0x40:
                return std::make_shared<OPCode40>(addr, row, xls_content, this);
            case 0x41:
                return std::make_shared<OPCode41>(addr, row, xls_content, this);
            case 0x42:
                return std::make_shared<OPCode42>(addr, row, xls_content, this);
            case 0x43:
                return std::make_shared<OPCode43>(addr, row, xls_content, this);
            case 0x44:
                return std::make_shared<OPCode44>(addr, row, xls_content, this);
            case 0x45:
                return std::make_shared<OPCode45>(addr, row, xls_content, this);
            case 0x46:
                return std::make_shared<OPCode46>(addr, row, xls_content, this);
            case 0x47:
                return std::make_shared<OPCode47>(addr, row, xls_content, this);
            case 0x48:
                return std::make_shared<OPCode48>(addr, row, xls_content, this);
            case 0x49:
                return std::make_shared<OPCode49>(addr, row, xls_content, this);
            case 0x4A:
                return std::make_shared<OPCode4A>(addr, row, xls_content, this);
            case 0x4B:
                return std::make_shared<OPCode4B>(addr, row, xls_content, this);
            case 0x4C:
                return std::make_shared<OPCode4C>(addr, row, xls_content, this);
            case 0x4D:
                return std::make_shared<OPCode4D>(addr, row, xls_content, this);
                //                case 0x4E: return std::make_shared<OPCode4E>(addr, row, xls_content,this);
            case 0x4F:
                return std::make_shared<OPCode4F>(addr, row, xls_content, this);
            case 0x50:
                return std::make_shared<OPCode50>(addr, row, xls_content, this);
            case 0x51:
                return std::make_shared<OPCode51>(addr, row, xls_content, this);
            case 0x52:
                return std::make_shared<OPCode52>(addr, row, xls_content, this);
            case 0x53:
                return std::make_shared<OPCode53>(addr, row, xls_content, this);
                //                case 0x54: return std::make_shared<OPCode54>(addr, row, xls_content,this);
            case 0x55:
                return std::make_shared<OPCode55>(addr, row, xls_content, this);
            case 0x56:
                return std::make_shared<OPCode56>(addr, row, xls_content, this);
            case 0x57:
                return std::make_shared<OPCode57>(addr, row, xls_content, this);
            case 0x58:
                return std::make_shared<OPCode58>(addr, row, xls_content, this);
                //                case 0x59: return std::make_shared<OPCode59>(addr, row, xls_content,this);
            case 0x5A:
                return std::make_shared<OPCode5A>(addr, row, xls_content, this);
            case 0x5B:
                return std::make_shared<OPCode5B>(addr, row, xls_content, this);
                //                case 0x5C: return std::make_shared<OPCode5C>(addr, row, xls_content,this);
            case 0x5D:
                return std::make_shared<OPCode5D>(addr, row, xls_content, this);
            case 0x5E:
                return std::make_shared<OPCode5E>(addr, row, xls_content, this);
            case 0x5F:
                return std::make_shared<OPCode5F>(addr, row, xls_content, this);
            case 0x60:
                return std::make_shared<OPCode60>(addr, row, xls_content, this);
            case 0x61:
                return std::make_shared<OPCode61>(addr, row, xls_content, this);
                //                case 0x62: return std::make_shared<OPCode62>(addr, row, xls_content,this);
            case 0x63:
                return std::make_shared<OPCode63>(addr, row, xls_content, this);
            case 0x64:
                return std::make_shared<OPCode64>(addr, row, xls_content, this);
            case 0x65:
                return std::make_shared<OPCode65>(addr, row, xls_content, this);
            case 0x66:
                return std::make_shared<OPCode66>(addr, row, xls_content, this);
            case 0x67:
                return std::make_shared<OPCode67>(addr, row, xls_content, this);
            case 0x68:
                return std::make_shared<OPCode68>(addr, row, xls_content, this);
            case 0x69:
                return std::make_shared<OPCode69>(addr, row, xls_content, this);
            case 0x6A:
                return std::make_shared<OPCode6A>(addr, row, xls_content, this);
            case 0x6B:
                return std::make_shared<OPCode6B>(addr, row, xls_content, this);
            case 0x6C:
                return std::make_shared<OPCode6C>(addr, row, xls_content, this);
            case 0x6D:
                return std::make_shared<OPCode6D>(addr, row, xls_content, this);
            case 0x6E:
                return std::make_shared<OPCode6E>(addr, row, xls_content, this);
            case 0x6F:
                return std::make_shared<OPCode6F>(addr, row, xls_content, this);
            case 0x70:
                return std::make_shared<OPCode70>(addr, row, xls_content, this);
                //                case 0x71: return std::make_shared<OPCode71>(addr, row, xls_content,this);
            case 0x72:
                return std::make_shared<OPCode72>(addr, row, xls_content, this);
            case 0x73:
                return std::make_shared<OPCode73>(addr, row, xls_content, this);
            case 0x74:
                return std::make_shared<OPCode74>(addr, row, xls_content, this);
            case 0x75:
                return std::make_shared<OPCode75>(addr, row, xls_content, this);
                //                case 0x76: return std::make_shared<OPCode76>(addr, row, xls_content,this);
                //                case 0x78: return std::make_shared<OPCode78>(addr, row, xls_content,this);
            case 0x79:
                return std::make_shared<OPCode79>(addr, row, xls_content, this);
                //                case 0x7A: return std::make_shared<OPCode7A>(addr, row, xls_content,this);
            case 0x7B:
                return std::make_shared<OPCode7B>(addr, row, xls_content, this);
                //                case 0x7C: return std::make_shared<OPCode7C>(addr, row, xls_content,this);
            case 0x7D:
                return std::make_shared<OPCode7D>(addr, row, xls_content, this);
            case 0x7E:
                return std::make_shared<OPCode7E>(addr, row, xls_content, this);
            case 0x7F:
                return std::make_shared<OPCode7F>(addr, row, xls_content, this);
            case 0x80:
                return std::make_shared<OPCode80>(addr, row, xls_content, this);
            case 0x81:
                return std::make_shared<OPCode81>(addr, row, xls_content, this);
            case 0x82:
                return std::make_shared<OPCode82>(addr, row, xls_content, this);
            case 0x83:
                return std::make_shared<OPCode83>(addr, row, xls_content, this);
            case 0x84:
                return std::make_shared<OPCode84>(addr, row, xls_content, this);
            case 0x85:
                return std::make_shared<OPCode85>(addr, row, xls_content, this);
            case 0x86:
                return std::make_shared<OPCode86>(addr, row, xls_content, this);
            case 0x87:
                return std::make_shared<OPCode87>(addr, row, xls_content, this);
            case 0x88:
                return std::make_shared<OPCode88>(addr, row, xls_content, this);
            case 0x89:
                return std::make_shared<OPCode89>(addr, row, xls_content, this);
            case 0x8A:
                return std::make_shared<OPCode8A>(addr, row, xls_content, this);
                //                case 0x8B: return std::make_shared<OPCode8B>(addr, row, xls_content,this);
            case 0x8C:
                return std::make_shared<OPCode8C>(addr, row, xls_content, this);
            case 0x8D:
                return std::make_shared<OPCode8D>(addr, row, xls_content, this);
            case 0x8E:
                return std::make_shared<OPCode8E>(addr, row, xls_content, this);
            case 0x8F:
                return std::make_shared<OPCode8F>(addr, row, xls_content, this);
            case 0x90:
                return std::make_shared<OPCode90>(addr, row, xls_content, this);
                //                case 0x91: return std::make_shared<OPCode91>(addr, row, xls_content,this);
            case 0x92:
                return std::make_shared<OPCode92>(addr, row, xls_content, this);
            case 0x93:
                return std::make_shared<OPCode93>(addr, row, xls_content, this);
            case 0x94:
                return std::make_shared<OPCode94>(addr, row, xls_content, this);
            case 0x95:
                return std::make_shared<OPCode95>(addr, row, xls_content, this);
            case 0x96:
                return std::make_shared<OPCode96>(addr, row, xls_content, this);
            case 0x97:
                return std::make_shared<OPCode97>(addr, row, xls_content, this);
                //                case 0x98: return std::make_shared<OPCode98>(addr, row, xls_content,this);
                //                //case 0x99: return std::make_shared<OPCode99>(addr, row, xls_content,this);
                //                case 0x9A: return std::make_shared<OPCode9A>(addr, row, xls_content,this);
            case 0x9B:
                return std::make_shared<OPCode9B>(addr, row, xls_content, this);
                //                case 0x9C: return std::make_shared<OPCode9C>(addr, row, xls_content,this);
            case 0x9D:
                return std::make_shared<OPCode9D>(addr, row, xls_content, this);
            case 0x9E:
                return std::make_shared<OPCode9E>(addr, row, xls_content, this);
            case 0x9F:
                return std::make_shared<OPCode9F>(addr, row, xls_content, this);
            case 0xA0:
                return std::make_shared<OPCodeA0>(addr, row, xls_content, this);
            case 0xA1:
                return std::make_shared<OPCodeA1>(addr, row, xls_content, this);
            case 0xA2:
                return std::make_shared<OPCodeA2>(addr, row, xls_content, this);
            case 0xA3:
                return std::make_shared<OPCodeA3>(addr, row, xls_content, this);
            case 0xA4:
                return std::make_shared<OPCodeA4>(addr, row, xls_content, this);
                //                case 0xA5: return std::make_shared<OPCodeA5>(addr, row, xls_content,this);
            case 0xA6:
                return std::make_shared<OPCodeA6>(addr, row, xls_content, this);
            case 0xA7:
                return std::make_shared<OPCodeA7>(addr, row, xls_content, this);
            case 0xA8:
                return std::make_shared<OPCodeA8>(addr, row, xls_content, this);
            case 0xA9:
                return std::make_shared<OPCodeA9>(addr, row, xls_content, this);
            case 0xAA:
                return std::make_shared<OPCodeAA>(addr, row, xls_content, this);
            case 0xAB:
                return std::make_shared<OPCodeAB>(addr, row, xls_content, this);
            case 0xAC:
                return std::make_shared<OPCodeAC>(addr, row, xls_content, this);
            case 0xAD:
                return std::make_shared<OPCodeAD>(addr, row, xls_content, this);
                //                case 0xAE: return std::make_shared<OPCodeAE>(addr, row, xls_content,this);
            case 0xAF:
                return std::make_shared<OPCodeAF>(addr, row, xls_content, this);
            case 0xB0:
                return std::make_shared<OPCodeB0>(addr, row, xls_content, this);
            case 0xB1:
                return std::make_shared<OPCodeB1>(addr, row, xls_content, this);
            case 0xB2:
                return std::make_shared<OPCodeB2>(addr, row, xls_content, this);
            case 0xB3:
                return std::make_shared<OPCodeB3>(addr, row, xls_content, this);
                //                case 0xB4: return std::make_shared<OPCodeB4>(addr, row, xls_content,this);
            case 0xB5:
                return std::make_shared<OPCodeB5>(addr, row, xls_content, this);
            case 0xB6:
                return std::make_shared<OPCodeB6>(addr, row, xls_content, this);
            case 0xB7:
                return std::make_shared<OPCodeB7>(addr, row, xls_content, this);
            case 0xB8:
                return std::make_shared<OPCodeB8>(addr, row, xls_content, this);
            case 0xB9:
                return std::make_shared<OPCodeB9>(addr, row, xls_content, this);
            case 0xBA:
                return std::make_shared<OPCodeBA>(addr, row, xls_content, this);
            case 0xBB:
                return std::make_shared<OPCodeBB>(addr, row, xls_content, this);
            case 0xBC:
                return std::make_shared<OPCodeBC>(addr, row, xls_content, this);
            case 0xBE:
                return std::make_shared<OPCodeBE>(addr, row, xls_content, this);
            case 0xBF:
                return std::make_shared<OPCodeBF>(addr, row, xls_content, this);
            case 0xC0:
                return std::make_shared<OPCodeC0>(addr, row, xls_content, this);
            case 0xC1:
                return std::make_shared<OPCodeC1>(addr, row, xls_content, this);
            case 0xC2:
                return std::make_shared<OPCodeC2>(addr, row, xls_content, this);
            case 0xC3:
                return std::make_shared<OPCodeC3>(addr, row, xls_content, this);
            case 0xC4:
                return std::make_shared<OPCodeC4>(addr, row, xls_content, this);
            case 0xC5:
                return std::make_shared<OPCodeC5>(addr, row, xls_content, this);
            case 0xC7:
                return std::make_shared<OPCodeC7>(addr, row, xls_content, this);
            case 0xC8:
                return std::make_shared<OPCodeC8>(addr, row, xls_content, this);
            case 0xC9:
                return std::make_shared<OPCodeC9>(addr, row, xls_content, this);
            case 0xCA:
                return std::make_shared<OPCodeCA>(addr, row, xls_content, this);
            case 256:
                return std::make_shared<CreateMonsters>(addr, row, xls_content, this);
            case 257:
                return std::make_shared<EffectsInstr>(addr, row, xls_content, this);
            case 258:
                return std::make_shared<ActionTable>(addr, row, xls_content, this);
            case 259:
                return std::make_shared<AlgoTable>(addr, row, xls_content, this);
            case 260:
                return std::make_shared<WeaponAttTable>(addr, row, xls_content, this);
            case 261:
                return std::make_shared<BreakTable>(addr, row, xls_content, this);
            case 262:
                return std::make_shared<SummonTable>(addr, row, xls_content, this);
            case 263:
                return std::make_shared<ReactionTable>(addr, row, xls_content, this);
            case 264:
                return std::make_shared<PartTable>(addr, row, xls_content, this);
            case 265:
                return std::make_shared<AnimeClipTable>(addr, row, xls_content, this);
            case 266:
                return std::make_shared<FieldMonsterData>(addr, row, xls_content, this);
            case 267:
                return std::make_shared<FieldFollowData>(addr, row, xls_content, this);
            case 268:
                return std::make_shared<FC_autoX>(addr, row, xls_content, this);
            case 269:
                return std::make_shared<BookData99>(addr, row, xls_content, this);
            case 270:
                return std::make_shared<BookDataX>(addr, row, xls_content, this);
            case 271:
                return std::make_shared<AddCollision>(addr, row, xls_content, this);
            case 272:
                return std::make_shared<ConditionTable>(addr, row, xls_content, this);
            case 273:
                return std::make_shared<AnimeClipData>(addr, row, xls_content, this);
            default:
                std::stringstream stream;
                stream << "L'OP code " << std::hex << opcode << " n'est pas défini !! " << this->scene_name;
                error = true;
                addr++;

                return {};
        }
    }

    ssd::Buffer create_header_bytes() override {

        ssd::Buffer header;

        ssd::Buffer scene_name_bytes = ssd::Buffer::fromStdString(scene_name);
        scene_name_bytes.push_back('\x0');
        size_t size_of_scene_name = scene_name_bytes.size();
        header.push_back(GetBytesFromInt(0x20));
        header.push_back(GetBytesFromInt(0x20));
        header.push_back(GetBytesFromInt(0x20 + size_of_scene_name));
        header.push_back(GetBytesFromInt(functions_parsed.size() * 4));
        header.push_back(GetBytesFromInt(0x20 + size_of_scene_name + functions_parsed.size() * 4));
        header.push_back(GetBytesFromInt(functions_parsed.size()));
        size_t length_of_names_section = 0;
        for (auto& fun : functions_parsed) {
            length_of_names_section = length_of_names_section + fun.name.size() + 1;
        }
        header.push_back(
          GetBytesFromInt(0x20 + size_of_scene_name + functions_parsed.size() * 4 + functions_parsed.size() * 2 + length_of_names_section));
        header.push_back(GetBytesFromInt(0xABCDEF00));
        header.push_back(scene_name_bytes);
        if (!functions_parsed.empty()) {
            ssd::Buffer position_names;
            ssd::Buffer actual_names;
            size_t offset_names = 0;
            for (auto& fun : functions_parsed) {
                header.push_back(GetBytesFromInt(fun.actual_addr));
                ssd::Buffer name = ssd::Buffer::fromStdString(fun.name);
                name.push_back('\x0');
                position_names.push_back(
                  GetBytesFromShort(0x20 + size_of_scene_name + functions_parsed.size() * 4 + functions_parsed.size() * 2 + offset_names));
                actual_names.push_back(name);
                offset_names = offset_names + name.size();
            }
            header.push_back(position_names);
            header.push_back(actual_names);
            int multiple = 4;
            if (functions_parsed[0].name.starts_with("_")) multiple = 0x10;
            size_t nb_byte_to_add = (((size_t)std::ceil((float)header.size() / (float)multiple))) * multiple - header.size();
            ssd::Buffer remaining;
            for (size_t i = 0; i < nb_byte_to_add; i++) {
                remaining.push_back('\x0');
            }
            header.push_back(remaining);
        }
        return header;
    }
};

#endif // TXINSTRUCTIONSSET_H
