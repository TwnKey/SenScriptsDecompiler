#ifndef CS1INSTRUCTIONSSET_H
#define CS1INSTRUCTIONSSET_H
#include "headers/functions.h"
#include "headers/utilities.h"

#include <cstdint>

class CS1Builder : public Builder {
  public:
    CS1Builder() = default;

    std::set<std::string> CS1UIFiles = { "battle_menu", "camp_menu",   "camp_menu_v", "note_menu",   "note_menu_v",
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
        unsigned char control_byte3 = control_byte3_arr[0];

        if (control_byte3 == '\x11') {
            instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            instr->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

        } else if (control_byte3 != '3') {

            if (control_byte3 == '\"') {
                instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                instr->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

            } else if (control_byte3 != 0x44) {

                if (control_byte3 == 0xDD) {

                    // there is a XOR,XOR EDI which causes a jump that ignores the strcpy
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
        CreateMonsters()
          : Instruction(-1, 256, nullptr) {}
        CreateMonsters(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "CreateMonsters", 256, Maker) {}
        CreateMonsters(int addr, Builder* Maker)
          : Instruction(addr, "CreateMonsters", 256, Maker) {}
        CreateMonsters(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "CreateMonsters", 256, Maker) {
            int initial_addr = addr;
            if (Maker->goal < addr + 0x20) {
                addr = initial_addr;
                throw ssd::exceptions::unexpected_operand();
            }
            int first = ReadIntegerFromByteArray(addr, content);

            if (first == -1){
                this->add_operande(
                  Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1C)));
                return;
            }

            ssd::Buffer map = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", map));

            ssd::Buffer remaining1 = ReadSubByteArray(content, addr, 0x10 - map.size());
            auto fill1 = Operande(addr, "fill", remaining1);
            fill1.set_bytes_to_fill(0x10);
            this->add_operande(fill1);

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x10
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x1A

            // 0x20

            int cnt = 0;
            do {
                if (Maker->goal < initial_addr + 0x20 + cnt * (0x90)) {
                    addr = initial_addr;
                    throw ssd::exceptions::unexpected_operand();
                }
                ssd::Buffer array = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "int", array));

                int counter = 0;

                do {
                    counter++;
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", monsters_name));

                    ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - monsters_name.size());
                    auto fill = Operande(addr, "fill", remaining);
                    fill.set_bytes_to_fill(0x10);
                    this->add_operande(fill);

                } while (counter < 0x8);
                for (int idx_byte = 0; idx_byte < 8; idx_byte++) {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
                if ((unsigned char)content[addr] == 0) {
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 8))); //??
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
        EffectsInstr()
          : Instruction(-1, 257, nullptr) {}
        EffectsInstr(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "EffectsInstr", 257, Maker) {}
        EffectsInstr(int addr, Builder* Maker)
          : Instruction(addr, "EffectsInstr", 257, Maker) {}
        EffectsInstr(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "EffectsInstr", 257, Maker) {
            unsigned char current_byte = content[addr];
            while (current_byte != 0x01) {

                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));

                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());

                auto fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x20);
                this->add_operande(fill);

                current_byte = content[addr];
            }
        }
    };
    class ActionTable : public Instruction {
      public:
        ActionTable()
          : Instruction(-1, 258, nullptr) {}
        ActionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "ActionTable", 258, Maker) {}
        ActionTable(int addr, Builder* Maker)
          : Instruction(addr, "ActionTable", 258, Maker) {}
        ActionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ActionTable", 258, Maker) {

            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {

                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                this->add_operande(Operande(addr, "short", short_bytes));                        // 2
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 3
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 4
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 5
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 6
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 7
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 8
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 9
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // A
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // B
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // C
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // D
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // F
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 11
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 13
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 15
                // here ok
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 17
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 19

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
        AddCollision()
          : Instruction(-1, 271, nullptr) {}
        AddCollision(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AddCollision", 271, Maker) {}
        AddCollision(int addr, Builder* Maker)
          : Instruction(addr, "AddCollision", 271, Maker) {}
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
    class AlgoTable : public Instruction {
      public:
        AlgoTable()
          : Instruction(-1, 259, nullptr) {}
        AlgoTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AlgoTable", 259, Maker) {}
        AlgoTable(int addr, Builder* Maker)
          : Instruction(addr, "AlgoTable", 259, Maker) {}
        AlgoTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AlgoTable", 259, Maker) {
            int cnt = 0;
            unsigned char current_byte = content[addr];
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // 3
            while (cnt < current_byte) {

                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);

                this->add_operande(Operande(addr, "short", short_bytes));

                this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1E)));
                int16_t shrt = ReadShortFromByteArray(0, short_bytes);
                if (shrt == -1) this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x8)));
                cnt++;
            }
        }
    };
    class WeaponAttTable : public Instruction {
      public:
        WeaponAttTable()
          : Instruction(-1, 260, nullptr) {}
        WeaponAttTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "WeaponAttTable", 260, Maker) {}
        WeaponAttTable(int addr, Builder* Maker)
          : Instruction(addr, "WeaponAttTable", 260, Maker) {}
        WeaponAttTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "WeaponAttTable", 260, Maker) {

            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 4)));
        }
    };
    class BreakTable : public Instruction {
      public:
        BreakTable()
          : Instruction(-1, 261, nullptr) {}
        BreakTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BreakTable", 261, Maker) {}
        BreakTable(int addr, Builder* Maker)
          : Instruction(addr, "BreakTable", 261, Maker) {}
        BreakTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "BreakTable", 261, Maker) {
            int cnt = 0;
            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                int16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->add_operande(Operande(addr, "short", short_bytes));
                if (shrt == 0) break;
                ssd::Buffer short_bytes2 = ReadSubByteArray(content, addr, 2);

                this->add_operande(Operande(addr, "short", short_bytes2)); // not sure if two single bytes
                // or one short, guessing its one short

                cnt++;
            } while (cnt < 0x40);
            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x2)));
        }
    };
    class SummonTable
      : public Instruction // 140142002
    {
      public:
        SummonTable()
          : Instruction(-1, 262, nullptr) {}
        SummonTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "SummonTable", 262, Maker) {}
        SummonTable(int addr, Builder* Maker)
          : Instruction(addr, "SummonTable", 262, Maker) {}
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
        ReactionTable()
          : Instruction(-1, 263, nullptr) {}
        ReactionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "ReactionTable", 263, Maker) {}
        ReactionTable(int addr, Builder* Maker)
          : Instruction(addr, "ReactionTable", 263, Maker) {}
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
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                cnt++;
            }
        }
    };
    class PartTable
      : public Instruction // 14019797c
    {
      public:
        PartTable()
          : Instruction(-1, 264, nullptr) {}
        PartTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "PartTable", 264, Maker) {}
        PartTable(int addr, Builder* Maker)
          : Instruction(addr, "PartTable", 264, Maker) {}
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
      : public Instruction // 140253d20
    {
      public:
        AnimeClipTable()
          : Instruction(-1, 265, nullptr) {}
        AnimeClipTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "AnimeClipTable", 265, Maker) {}
        AnimeClipTable(int addr, Builder* Maker)
          : Instruction(addr, "AnimeClipTable", 265, Maker) {}
        AnimeClipTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AnimeClipTable", 265, Maker) {

            int first_integer = ReadIntegerFromByteArray(addr, content);

            while (first_integer != 0) {
                ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "int", first_integer_bytes)); // 14025b3c2

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
                first_integer = ReadIntegerFromByteArray(addr, content);
            }
            ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
            this->add_operande(Operande(addr, "int", first_integer_bytes));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            // I think this is some sort of a table with ID at the beginning and then two names
        }
    };
    class FieldMonsterData
      : public Instruction // 00000001402613C2 probably trigger only for monsters on the field
    {
      public:
        FieldMonsterData()
          : Instruction(-1, 266, nullptr) {}
        FieldMonsterData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FieldMonsterData", 266, Maker) {}
        FieldMonsterData(int addr, Builder* Maker)
          : Instruction(addr, "FieldMonsterData", 266, Maker) {}
        FieldMonsterData(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "FieldMonsterData", 266, Maker) {

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            int sentinel = ReadIntegerFromByteArray(addr, content);
            while (sentinel!=1){
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                sentinel = ReadIntegerFromByteArray(addr, content);
            }
        }
    };
    class FieldFollowData
      : public Instruction //
    {
      public:
        FieldFollowData()
          : Instruction(-1, 267, nullptr) {}
        FieldFollowData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FieldMonsterData", 267, Maker) {}
        FieldFollowData(int addr, Builder* Maker)
          : Instruction(addr, "FieldMonsterData", 267, Maker) {}
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
        FC_autoX()
          : Instruction(-1, 268, nullptr) {}
        FC_autoX(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "FC_autoX", 268, Maker) {}
        FC_autoX(int addr, Builder* Maker)
          : Instruction(addr, "FC_autoX", 268, Maker) {}
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
        BookData99()
          : Instruction(-1, 269, nullptr) {}
        BookData99(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BookData99", 269, Maker) {}
        BookData99(int addr, Builder* Maker)
          : Instruction(addr, "BookData99", 269, Maker) {}
        BookData99(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "BookData99", 269, Maker) {
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class BookDataX
      : public Instruction // 0000000140464549
    {
      public:
        BookDataX()
          : Instruction(-1, 270, nullptr) {}
        BookDataX(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "BookDataX", 270, Maker) {}
        BookDataX(int addr, Builder* Maker)
          : Instruction(addr, "BookDataX", 270, Maker) {}
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
        OPCode0()
          : Instruction(-1, 0, nullptr) {}
        OPCode0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "Instruction 0", 0, Maker) {}
        OPCode0(int addr, Builder* Maker)
          : Instruction(addr, "Instruction 0", 0, Maker) {}
        OPCode0(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Instruction 0", 0, Maker) {
            addr++;
        }
    };
    class OPCode1 : public Instruction {
      public:
        OPCode1()
          : Instruction(-1, 1, nullptr) {}
        OPCode1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "Return", 1, Maker) {}
        OPCode1(int addr, Builder* Maker)
          : Instruction(addr, "Return", 1, Maker) {}
        OPCode1(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Return", 1, Maker) {
            addr++;
        }
    };

    class UI_OP13 : public Instruction {
      public:
        UI_OP13()
          : Instruction(-1, 0x13, nullptr) {}
        UI_OP13(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x13, Maker) {}
        UI_OP13(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x13, Maker) {}
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
                case 0x32:
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
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
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
            }
        }
    };
    // SCENARIO FILES INSTRUCTIONS
    // 0x05
    class OPCode05 : public Instruction {
      public:
        OPCode05()
          : Instruction(-1, 0x05, nullptr) {}
        OPCode05(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x05, Maker) {}
        OPCode05(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x05, Maker) {}
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
        OPCode02()
          : Instruction(-1, 0x02, nullptr) {}
        OPCode02(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x02, Maker) {}
        OPCode02(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x02, Maker) {}
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
        OPCode03()
          : Instruction(-1, 0x03, nullptr) {}
        OPCode03(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x03, Maker) {}
        OPCode03(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x03, Maker) {}
        OPCode03(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x03, Maker) {
            addr++;
            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x04
    class OPCode04 : public Instruction {
      public:
        OPCode04()
          : Instruction(-1, 0x04, nullptr) {}
        OPCode04(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x04, Maker) {}
        OPCode04(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x04, Maker) {}
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
        OPCode06()
          : Instruction(-1, 0x06, nullptr) {}
        OPCode06(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x06, Maker) {}
        OPCode06(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x06, Maker) {}
        OPCode06(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x06, Maker) {
            addr++;
            sub05(addr, content, this);
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            if ((unsigned char)control_byte[0] != 0) {

                for (unsigned char i = 0; i < control_byte[0]; i++) {

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
        OPCode07()
          : Instruction(-1, 0x07, nullptr) {}
        OPCode07(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x07, Maker) {}
        OPCode07(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x07, Maker) {}
        OPCode07(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x07, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x08
    class OPCode08 : public Instruction {
      public:
        OPCode08()
          : Instruction(-1, 0x08, nullptr) {}
        OPCode08(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x08, Maker) {}
        OPCode08(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x08, Maker) {}
        OPCode08(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x08, Maker) {
            addr++;
            sub05(addr, content, this);
        }
    };
    // 0x0A
    class OPCode0A : public Instruction {
      public:
        OPCode0A()
          : Instruction(-1, 0x0A, nullptr) {}
        OPCode0A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0A, Maker) {}
        OPCode0A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x0A, Maker) {}
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
        OPCode0C()
          : Instruction(-1, 0x0C, nullptr) {}
        OPCode0C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0C, Maker) {}
        OPCode0C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x0C, Maker) {}
        OPCode0C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x0D
    class OPCode0D : public Instruction {
      public:
        OPCode0D()
          : Instruction(-1, 0x0D, nullptr) {}
        OPCode0D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0D, Maker) {}
        OPCode0D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x0D, Maker) {}
        OPCode0D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x0E
    class OPCode0E : public Instruction {
      public:
        OPCode0E()
          : Instruction(-1, 0x0E, nullptr) {}
        OPCode0E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0E, Maker) {}
        OPCode0E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x0E, Maker) {}
        OPCode0E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x0F
    class OPCode0F : public Instruction {
      public:
        OPCode0F()
          : Instruction(-1, 0x0F, nullptr) {}
        OPCode0F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x0F, Maker) {}
        OPCode0F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x0F, Maker) {}
        OPCode0F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x10
    class OPCode10 : public Instruction {
      public:
        OPCode10()
          : Instruction(-1, 0x10, nullptr) {}
        OPCode10(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x10, Maker) {}
        OPCode10(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x10, Maker) {}
        OPCode10(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x10, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x11
    class OPCode11 : public Instruction {
      public:
        OPCode11()
          : Instruction(-1, 0x11, nullptr) {}
        OPCode11(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x11, Maker) {}
        OPCode11(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x11, Maker) {}
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
        OPCode12()
          : Instruction(-1, 0x12, nullptr) {}
        OPCode12(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x12, Maker) {}
        OPCode12(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x12, Maker) {}
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
        OPCode13()
          : Instruction(-1, 0x13, nullptr) {}
        OPCode13(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x13, Maker) {}
        OPCode13(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x13, Maker) {}
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

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };

    // 0x14
    class OPCode14 : public Instruction {
      public:
        OPCode14()
          : Instruction(-1, 0x14, nullptr) {}
        OPCode14(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x14, Maker) {}
        OPCode14(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x14, Maker) {}
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
        OPCode15()
          : Instruction(-1, 0x15, nullptr) {}
        OPCode15(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x15, Maker) {}
        OPCode15(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x15, Maker) {}
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
        OPCode16()
          : Instruction(-1, 0x16, nullptr) {}
        OPCode16(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x16, Maker) {}
        OPCode16(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x16, Maker) {}
        OPCode16(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x16, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x17
    class OPCode17 : public Instruction {
      public:
        OPCode17()
          : Instruction(-1, 0x17, nullptr) {}
        OPCode17(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x17, Maker) {}
        OPCode17(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x17, Maker) {}
        OPCode17(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x17, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x18
    class OPCode18 : public Instruction {
      public:
        OPCode18()
          : Instruction(-1, 0x18, nullptr) {}
        OPCode18(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x18, Maker) {}
        OPCode18(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x18, Maker) {}
        OPCode18(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x18, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            // this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            reading_dialog(addr, content, this);
        }
    };
    // 0x19
    class OPCode19 : public Instruction {
      public:
        OPCode19()
          : Instruction(-1, 0x19, nullptr) {}
        OPCode19(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x19, Maker) {}
        OPCode19(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x19, Maker) {}
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
        OPCode1A()
          : Instruction(-1, 0x1A, nullptr) {}
        OPCode1A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1A, Maker) {}
        OPCode1A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1A, Maker) {}
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
        OPCode1B()
          : Instruction(-1, 0x1B, nullptr) {}
        OPCode1B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1B, Maker) {}
        OPCode1B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1B, Maker) {}
        OPCode1B(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1B, Maker) {
            addr++;
        }
    };
    // 0x1C
    class OPCode1C : public Instruction {
      public:
        OPCode1C()
          : Instruction(-1, 0x1C, nullptr) {}
        OPCode1C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1C, Maker) {}
        OPCode1C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1C, Maker) {}
        OPCode1C(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1C, Maker) {
            addr++;
        }
    };
    // 0x1D
    class OPCode1D : public Instruction {
      public:
        OPCode1D()
          : Instruction(-1, 0x1D, nullptr) {}
        OPCode1D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1D, Maker) {}
        OPCode1D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1D, Maker) {}
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
        OPCode1E()
          : Instruction(-1, 0x1E, nullptr) {}
        OPCode1E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1E, Maker) {}
        OPCode1E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1E, Maker) {}
        OPCode1E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x1F
    class OPCode1F : public Instruction {
      public:
        OPCode1F()
          : Instruction(-1, 0x1F, nullptr) {}
        OPCode1F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x1F, Maker) {}
        OPCode1F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x1F, Maker) {}
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
                }
            }
        }
    };
    // 0x20
    class OPCode20 : public Instruction {
      public:
        OPCode20()
          : Instruction(-1, 0x20, nullptr) {}
        OPCode20(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x20, Maker) {}
        OPCode20(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x20, Maker) {}
        OPCode20(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x20, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x02:
                case 0x01:
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
        OPCode21()
          : Instruction(-1, 0x21, nullptr) {}
        OPCode21(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x21, Maker) {}
        OPCode21(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x21, Maker) {}
        OPCode21(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x21, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            switch ((unsigned char)control_byte[0]) {
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
        OPCode22()
          : Instruction(-1, 0x22, nullptr) {}
        OPCode22(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x22, Maker) {}
        OPCode22(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x22, Maker) {}
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
        }
    };
    // 0x23
    class OPCode23 : public Instruction {
      public:
        OPCode23()
          : Instruction(-1, 0x23, nullptr) {}
        OPCode23(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x23, Maker) {}
        OPCode23(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x23, Maker) {}
        OPCode23(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x23, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x24
    class OPCode24 : public Instruction {
      public:
        OPCode24()
          : Instruction(-1, 0x24, nullptr) {}
        OPCode24(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x24, Maker) {}
        OPCode24(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x24, Maker) {}
        OPCode24(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x24, Maker) {
            addr++;

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x25
    class OPCode25 : public Instruction {
      public:
        OPCode25()
          : Instruction(-1, 0x25, nullptr) {}
        OPCode25(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x25, Maker) {}
        OPCode25(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x25, Maker) {}
        OPCode25(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x25, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x26
    class OPCode26 : public Instruction {
      public:
        OPCode26()
          : Instruction(-1, 0x26, nullptr) {}
        OPCode26(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x26, Maker) {}
        OPCode26(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x26, Maker) {}
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
        OPCode27()
          : Instruction(-1, 0x27, nullptr) {}
        OPCode27(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x27, Maker) {}
        OPCode27(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x27, Maker) {}
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
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
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
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x11:
                case 0x10:
                case 0x0F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x12: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x13:
                case 0x14: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x15: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0xD:
                case 0xE: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
        }
    };
    // 0x28
    class OPCode28 : public Instruction {
      public:
        OPCode28()
          : Instruction(-1, 0x28, nullptr) {}
        OPCode28(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x28, Maker) {}
        OPCode28(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x28, Maker) {}
        OPCode28(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x28, Maker) {
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
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case '\b':
                case 0x06:
                case 0x04:
                case 0x02: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case '\a':
                case 0x05: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 'm':
                case 'l':
                case 'k':
                case 'f':
                case 'e':
                case 'c':
                case '\x1C':
                case '\x1A':
                case '\x17':
                case '\x19':
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
                case 'y': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case '\x0f':
                case '\n': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case '\f':
                case '4':
                case 'Z':
                case '\v': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '\x0e': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 'b':
                case '\x11':
                case 'z': {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case '\x12': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    for (int i = 0; i < 8; i++) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;
                }
                case '\x13': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '\x15': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '2': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case '5': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case '6': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case '7': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case '<': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 'F': {

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 'P': {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case '[': {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '\\': {

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 'a': {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 'd': {

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 'n': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '~':
                case 'q': {

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 'r': {

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case '{': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 'i': {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 'j': {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x29
    class OPCode29 : public Instruction {
      public:
        OPCode29()
          : Instruction(-1, 0x29, nullptr) {}
        OPCode29(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x29, Maker) {}
        OPCode29(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x29, Maker) {}
        OPCode29(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x29, Maker) {
            addr++;

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x2A
    class OPCode2A : public Instruction {
      public:
        OPCode2A()
          : Instruction(-1, 0x2A, nullptr) {}
        OPCode2A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2A, Maker) {}
        OPCode2A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2A, Maker) {}
        OPCode2A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x2B
    class OPCode2B : public Instruction {
      public:
        OPCode2B()
          : Instruction(-1, 0x2B, nullptr) {}
        OPCode2B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2B, Maker) {}
        OPCode2B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2B, Maker) {}
        OPCode2B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

    // 0x2C
    class OPCode2C : public Instruction {
      public:
        OPCode2C()
          : Instruction(-1, 0x2C, nullptr) {}
        OPCode2C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2C, Maker) {}
        OPCode2C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2C, Maker) {}
        OPCode2C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x2D
    class OPCode2D : public Instruction {
      public:
        OPCode2D()
          : Instruction(-1, 0x2D, nullptr) {}
        OPCode2D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2D, Maker) {}
        OPCode2D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2D, Maker) {}
        OPCode2D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2D, Maker) {
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
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

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
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x2E
    class OPCode2E : public Instruction {
      public:
        OPCode2E()
          : Instruction(-1, 0x2E, nullptr) {}
        OPCode2E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2E, Maker) {}
        OPCode2E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2E, Maker) {}
        OPCode2E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x2F
    class OPCode2F : public Instruction {
      public:
        OPCode2F()
          : Instruction(-1, 0x2F, nullptr) {}
        OPCode2F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x2F, Maker) {}
        OPCode2F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x2F, Maker) {}
        OPCode2F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x30
    class OPCode30 : public Instruction {
      public:
        OPCode30()
          : Instruction(-1, 0x30, nullptr) {}
        OPCode30(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x30, Maker) {}
        OPCode30(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x30, Maker) {}
        OPCode30(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x30, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
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
        OPCode31()
          : Instruction(-1, 0x31, nullptr) {}
        OPCode31(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x31, Maker) {}
        OPCode31(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x31, Maker) {}
        OPCode31(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x31, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            if ((unsigned char)control_byte[0] > 0xFD) {
                if ((unsigned char)control_byte[0] == 0xFE) {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                } else if ((unsigned char)control_byte[0] == 0xFF) {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                }

                return;
            }

            switch ((unsigned char)control_byte[0]) {
                case 0x00:
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
                case 4:
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
                case 0x14:
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
                case 0x3b:
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
                case 0x96:
                case 0x39:
                case 0x06:
                case 0x38:
                case 0x05:
                case 0x37:
                case 0x34:
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x35:
                case 0x03: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3A: {
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
                case 0xFD: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
                default: {
                }
            }
        }
    };
    // 0x32
    class OPCode32 : public Instruction {
      public:
        OPCode32()
          : Instruction(-1, 0x32, nullptr) {}
        OPCode32(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x32, Maker) {}
        OPCode32(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x32, Maker) {}
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
        OPCode33()
          : Instruction(-1, 0x33, nullptr) {}
        OPCode33(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x33, Maker) {}
        OPCode33(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x33, Maker) {}
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
        OPCode34()
          : Instruction(-1, 0x34, nullptr) {}
        OPCode34(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x34, Maker) {}
        OPCode34(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x34, Maker) {}
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
        OPCode35()
          : Instruction(-1, 0x35, nullptr) {}
        OPCode35(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x35, Maker) {}
        OPCode35(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x35, Maker) {}
        OPCode35(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x35, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x36
    class OPCode36 : public Instruction {
      public:
        OPCode36()
          : Instruction(-1, 0x36, nullptr) {}
        OPCode36(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x36, Maker) {}
        OPCode36(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x36, Maker) {}
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
        }
    };
    // 0x37
    class OPCode37 : public Instruction {
      public:
        OPCode37()
          : Instruction(-1, 0x37, nullptr) {}
        OPCode37(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x37, Maker) {}
        OPCode37(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x37, Maker) {}
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
        OPCode38()
          : Instruction(-1, 0x38, nullptr) {}
        OPCode38(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x38, Maker) {}
        OPCode38(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x38, Maker) {}
        OPCode38(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x38, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x39
    class OPCode39 : public Instruction {
      public:
        OPCode39()
          : Instruction(-1, 0x39, nullptr) {}
        OPCode39(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x39, Maker) {}
        OPCode39(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x39, Maker) {}
        OPCode39(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x39, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            if ((code != 0xC) && (code != 0x5) && (code != 0x69) && (code != 0xA) && (code != 0xB) && (code != 0xFF) && (code != 0xFE)) {
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            }
        }
    };
    // 0x3A
    class OPCode3A : public Instruction {
      public:
        OPCode3A()
          : Instruction(-1, 0x3A, nullptr) {}
        OPCode3A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3A, Maker) {}
        OPCode3A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3A, Maker) {}
        OPCode3A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x3B
    class OPCode3B : public Instruction {
      public:
        OPCode3B()
          : Instruction(-1, 0x3B, nullptr) {}
        OPCode3B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3B, Maker) {}
        OPCode3B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3B, Maker) {}
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
    static int FUN_00644350(uint16_t first_two_bytes) {
        if (first_two_bytes < 0x10) {
            return 0;
        }
        if (first_two_bytes == 0xFFFD) return 0;
        return 1;
    }

    class OPCode3C : public Instruction {
      public:
        OPCode3C()
          : Instruction(-1, 0x3C, nullptr) {}
        OPCode3C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3C, Maker) {}
        OPCode3C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3C, Maker) {}
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
            uint16_t short2 = ReadShortFromByteArray(0, control_short2);

            int var2 = FUN_00644350(short1);
            int var3 = FUN_00644350(short2);
            if (((var2 == 0) || (var3 == 0)) || (var2 == var3)) {
                if (short2 == 0xFFFF) {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                }
            }
        }
    };
    // 0x3D
    class OPCode3D : public Instruction {
      public:
        OPCode3D()
          : Instruction(-1, 0x3D, nullptr) {}
        OPCode3D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3D, Maker) {}
        OPCode3D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3D, Maker) {}
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
        OPCode3E()
          : Instruction(-1, 0x3E, nullptr) {}
        OPCode3E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3E, Maker) {}
        OPCode3E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3E, Maker) {}
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
        OPCode3F()
          : Instruction(-1, 0x3F, nullptr) {}
        OPCode3F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x3F, Maker) {}
        OPCode3F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x3F, Maker) {}
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
            }
        }
    };

    // 0x40
    class OPCode40 : public Instruction {
      public:
        OPCode40()
          : Instruction(-1, 0x40, nullptr) {}
        OPCode40(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x40, Maker) {}
        OPCode40(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x40, Maker) {}
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
        OPCode41()
          : Instruction(-1, 0x41, nullptr) {}
        OPCode41(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x41, Maker) {}
        OPCode41(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x41, Maker) {}
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
        OPCode42()
          : Instruction(-1, 0x42, nullptr) {}
        OPCode42(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x42, Maker) {}
        OPCode42(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x42, Maker) {}
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
        OPCode43()
          : Instruction(-1, 0x43, nullptr) {}
        OPCode43(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x43, Maker) {}
        OPCode43(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x43, Maker) {}
        OPCode43(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x43, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x44
    class OPCode44 : public Instruction {
      public:
        OPCode44()
          : Instruction(-1, 0x44, nullptr) {}
        OPCode44(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x44, Maker) {}
        OPCode44(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x44, Maker) {}
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
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04:
                case 0x05: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x45
    class OPCode45 : public Instruction {
      public:
        OPCode45()
          : Instruction(-1, 0x45, nullptr) {}
        OPCode45(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x45, Maker) {}
        OPCode45(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x45, Maker) {}
        OPCode45(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x45, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x46
    class OPCode46 : public Instruction {
      public:
        OPCode46()
          : Instruction(-1, 0x46, nullptr) {}
        OPCode46(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x46, Maker) {}
        OPCode46(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x46, Maker) {}
        OPCode46(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x46, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x47
    class OPCode47 : public Instruction {
      public:
        OPCode47()
          : Instruction(-1, 0x47, nullptr) {}
        OPCode47(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x47, Maker) {}
        OPCode47(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x47, Maker) {}
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
        OPCode48()
          : Instruction(-1, 0x48, nullptr) {}
        OPCode48(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x48, Maker) {}
        OPCode48(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x48, Maker) {}
        OPCode48(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x48, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x49
    class OPCode49 : public Instruction {
      public:
        OPCode49()
          : Instruction(-1, 0x49, nullptr) {}
        OPCode49(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x49, Maker) {}
        OPCode49(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x49, Maker) {}
        OPCode49(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x49, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x0A:
                case 0x04:
                case 0x03:
                case 0x02:
                case 0x01:
                case 0x00: {
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
                case '\r': {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x14: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
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
                case 0x1C: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case '!': {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case '(':
                case '$':
                case '#': {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case '&': {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case ')': {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x4A
    class OPCode4A : public Instruction {
      public:
        OPCode4A()
          : Instruction(-1, 0x4A, nullptr) {}
        OPCode4A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4A, Maker) {}
        OPCode4A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4A, Maker) {}
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
        OPCode4B()
          : Instruction(-1, 0x4B, nullptr) {}
        OPCode4B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4B, Maker) {}
        OPCode4B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4B, Maker) {}
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
        OPCode4C()
          : Instruction(-1, 0x4C, nullptr) {}
        OPCode4C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4C, Maker) {}
        OPCode4C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4C, Maker) {}
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
        OPCode4D()
          : Instruction(-1, 0x4D, nullptr) {}
        OPCode4D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4D, Maker) {}
        OPCode4D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4D, Maker) {}
        OPCode4D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x4E
    class OPCode4E : public Instruction {
      public:
        OPCode4E()
          : Instruction(-1, 0x4E, nullptr) {}
        OPCode4E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4E, Maker) {}
        OPCode4E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4E, Maker) {}
        OPCode4E(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x4E, Maker) {
            addr++;
        }
    };
    // 0x4F
    class OPCode4F : public Instruction {
      public:
        OPCode4F()
          : Instruction(-1, 0x4F, nullptr) {}
        OPCode4F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x4F, Maker) {}
        OPCode4F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x4F, Maker) {}
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
        OPCode50()
          : Instruction(-1, 0x50, nullptr) {}
        OPCode50(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x50, Maker) {}
        OPCode50(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x50, Maker) {}
        OPCode50(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x50, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x51
    class OPCode51 : public Instruction {
      public:
        OPCode51()
          : Instruction(-1, 0x51, nullptr) {}
        OPCode51(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x51, Maker) {}
        OPCode51(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x51, Maker) {}
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
        OPCode52()
          : Instruction(-1, 0x52, nullptr) {}
        OPCode52(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x52, Maker) {}
        OPCode52(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x52, Maker) {}
        OPCode52(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x52, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x53
    class OPCode53 : public Instruction {
      public:
        OPCode53()
          : Instruction(-1, 0x53, nullptr) {}
        OPCode53(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x53, Maker) {}
        OPCode53(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x53, Maker) {}
        OPCode53(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x53, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x55
    class OPCode55 : public Instruction {
      public:
        OPCode55()
          : Instruction(-1, 0x55, nullptr) {}
        OPCode55(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x55, Maker) {}
        OPCode55(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x55, Maker) {}
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
        OPCode56()
          : Instruction(-1, 0x56, nullptr) {}
        OPCode56(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x56, Maker) {}
        OPCode56(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x56, Maker) {}
        OPCode56(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x56, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x57
    class OPCode57 : public Instruction {
      public:
        OPCode57()
          : Instruction(-1, 0x57, nullptr) {}
        OPCode57(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x57, Maker) {}
        OPCode57(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x57, Maker) {}
        OPCode57(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x57, Maker) {
            addr++;
        }
    };
    // 0x58
    class OPCode58 : public Instruction {
      public:
        OPCode58()
          : Instruction(-1, 0x58, nullptr) {}
        OPCode58(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x58, Maker) {}
        OPCode58(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x58, Maker) {}
        OPCode58(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x58, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x59
    class OPCode59 : public Instruction {
      public:
        OPCode59()
          : Instruction(-1, 0x59, nullptr) {}
        OPCode59(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x59, Maker) {}
        OPCode59(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x59, Maker) {}
        OPCode59(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x59, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x5A
    class OPCode5A : public Instruction {
      public:
        OPCode5A()
          : Instruction(-1, 0x5A, nullptr) {}
        OPCode5A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5A, Maker) {}
        OPCode5A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5A, Maker) {}
        OPCode5A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x5B
    class OPCode5B : public Instruction {
      public:
        OPCode5B()
          : Instruction(-1, 0x5B, nullptr) {}
        OPCode5B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5B, Maker) {}
        OPCode5B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5B, Maker) {}
        OPCode5B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x04:
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x5C
    class OPCode5C : public Instruction {
      public:
        OPCode5C()
          : Instruction(-1, 0x5C, nullptr) {}
        OPCode5C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5C, Maker) {}
        OPCode5C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5C, Maker) {}
        OPCode5C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x5D
    class OPCode5D : public Instruction {
      public:
        OPCode5D()
          : Instruction(-1, 0x5D, nullptr) {}
        OPCode5D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5D, Maker) {}
        OPCode5D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5D, Maker) {}
        OPCode5D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5D, Maker) {
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
            }
        }
    };
    // 0x5E
    class OPCode5E : public Instruction {
      public:
        OPCode5E()
          : Instruction(-1, 0x5E, nullptr) {}
        OPCode5E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5E, Maker) {}
        OPCode5E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5E, Maker) {}
        OPCode5E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5E, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0B:
                case 0x09:
                case 0x07:
                case 0x08:
                case 0x04:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x05:
                case 0x03:
                case 0x02: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x06: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x0D:
                case 0x0C: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    // 0x5F
    class OPCode5F : public Instruction {
      public:
        OPCode5F()
          : Instruction(-1, 0x5F, nullptr) {}
        OPCode5F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x5F, Maker) {}
        OPCode5F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x5F, Maker) {}
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
            }
        }
    };
    // 0x60
    class OPCode60 : public Instruction {
      public:
        OPCode60()
          : Instruction(-1, 0x60, nullptr) {}
        OPCode60(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x60, Maker) {}
        OPCode60(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x60, Maker) {}
        OPCode60(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x60, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x61
    class OPCode61 : public Instruction {
      public:
        OPCode61()
          : Instruction(-1, 0x61, nullptr) {}
        OPCode61(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x61, Maker) {}
        OPCode61(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x61, Maker) {}
        OPCode61(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x61, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x62
    class OPCode62 : public Instruction {
      public:
        OPCode62()
          : Instruction(-1, 0x62, nullptr) {}
        OPCode62(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x62, Maker) {}
        OPCode62(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x62, Maker) {}
        OPCode62(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x62, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x63
    class OPCode63 : public Instruction {
      public:
        OPCode63()
          : Instruction(-1, 0x63, nullptr) {}
        OPCode63(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x63, Maker) {}
        OPCode63(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x63, Maker) {}
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
        OPCode64()
          : Instruction(-1, 0x64, nullptr) {}
        OPCode64(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x64, Maker) {}
        OPCode64(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x64, Maker) {}
        OPCode64(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x64, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x65
    class OPCode65 : public Instruction {
      public:
        OPCode65()
          : Instruction(-1, 0x65, nullptr) {}
        OPCode65(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x65, Maker) {}
        OPCode65(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x65, Maker) {}
        OPCode65(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x65, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x05:
                case 0x03:
                case 0x00: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x06: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x09:
                case 0x07:
                case 0x04:
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
                case 0x08: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };

    // 0x66
    class OPCode66 : public Instruction {
      public:
        OPCode66()
          : Instruction(-1, 0x66, nullptr) {}
        OPCode66(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x66, Maker) {}
        OPCode66(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x66, Maker) {}
        OPCode66(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x66, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x67
    class OPCode67 : public Instruction {
      public:
        OPCode67()
          : Instruction(-1, 0x67, nullptr) {}
        OPCode67(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x67, Maker) {}
        OPCode67(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x67, Maker) {}
        OPCode67(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x67, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            uint16_t control = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "short", control_short));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            switch (code) {
                case 0x02:
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x03:
                case 0x04: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
            if ((control <= 0x96) && (code == 0x6)) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    // 0x68
    class OPCode68 : public Instruction {
      public:
        OPCode68()
          : Instruction(-1, 0x68, nullptr) {}
        OPCode68(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x68, Maker) {}
        OPCode68(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x68, Maker) {}
        OPCode68(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x68, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x01: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
            }
        }
    };
    // 0x69
    class OPCode69 : public Instruction {
      public:
        OPCode69()
          : Instruction(-1, 0x69, nullptr) {}
        OPCode69(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x69, Maker) {}
        OPCode69(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x69, Maker) {}
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

                    break;
                }
                case 0x03: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x05: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    // 0x6A
    class OPCode6A : public Instruction {
      public:
        OPCode6A()
          : Instruction(-1, 0x6A, nullptr) {}
        OPCode6A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6A, Maker) {}
        OPCode6A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6A, Maker) {}
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
                }
            }
        }
    };
    // 0x6B
    class OPCode6B : public Instruction {
      public:
        OPCode6B()
          : Instruction(-1, 0x6B, nullptr) {}
        OPCode6B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6B, Maker) {}
        OPCode6B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6B, Maker) {}
        OPCode6B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x6C
    class OPCode6C : public Instruction {
      public:
        OPCode6C()
          : Instruction(-1, 0x6C, nullptr) {}
        OPCode6C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6C, Maker) {}
        OPCode6C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6C, Maker) {}
        OPCode6C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6C, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x6D
    class OPCode6D : public Instruction {
      public:
        OPCode6D()
          : Instruction(-1, 0x6D, nullptr) {}
        OPCode6D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6D, Maker) {}
        OPCode6D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6D, Maker) {}
        OPCode6D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0x6E
    class OPCode6E : public Instruction {
      public:
        OPCode6E()
          : Instruction(-1, 0x6E, nullptr) {}
        OPCode6E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6E, Maker) {}
        OPCode6E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6E, Maker) {}
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
            }
        }
    };
    // 0x6F
    class OPCode6F : public Instruction {
      public:
        OPCode6F()
          : Instruction(-1, 0x6F, nullptr) {}
        OPCode6F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x6F, Maker) {}
        OPCode6F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x6F, Maker) {}
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
        OPCode70()
          : Instruction(-1, 0x70, nullptr) {}
        OPCode70(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x70, Maker) {}
        OPCode70(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x70, Maker) {}
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
    class OPCode71 : public Instruction {
      public:
        OPCode71()
          : Instruction(-1, 0x71, nullptr) {}
        OPCode71(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x71, Maker) {}
        OPCode71(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x71, Maker) {}
        OPCode71(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x71, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    // 0x72
    class OPCode72 : public Instruction {
      public:
        OPCode72()
          : Instruction(-1, 0x72, nullptr) {}
        OPCode72(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x72, Maker) {}
        OPCode72(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x72, Maker) {}
        OPCode72(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x72, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x73
    class OPCode73 : public Instruction {
      public:
        OPCode73()
          : Instruction(-1, 0x73, nullptr) {}
        OPCode73(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x73, Maker) {}
        OPCode73(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x73, Maker) {}
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
            }
        }
    };
    // 0x74
    class OPCode74 : public Instruction {
      public:
        OPCode74()
          : Instruction(-1, 0x74, nullptr) {}
        OPCode74(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x74, Maker) {}
        OPCode74(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x74, Maker) {}
        OPCode74(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x74, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    // 0x77
    class OPCode77 : public Instruction {
      public:
        OPCode77()
          : Instruction(-1, 0x77, nullptr) {}
        OPCode77(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x77, Maker) {}
        OPCode77(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x77, Maker) {}
        OPCode77(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x77, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x78
    class OPCode78 : public Instruction {
      public:
        OPCode78()
          : Instruction(-1, 0x78, nullptr) {}
        OPCode78(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x78, Maker) {}
        OPCode78(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x78, Maker) {}
        OPCode78(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x78, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x79
    class OPCode79 : public Instruction {
      public:
        OPCode79()
          : Instruction(-1, 0x79, nullptr) {}
        OPCode79(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x79, Maker) {}
        OPCode79(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x79, Maker) {}
        OPCode79(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x79, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x7A
    class OPCode7A : public Instruction {
      public:
        OPCode7A()
          : Instruction(-1, 0x7A, nullptr) {}
        OPCode7A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7A, Maker) {}
        OPCode7A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x7A, Maker) {}
        OPCode7A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7A, Maker) {
            addr++;

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x7B
    class OPCode7B : public Instruction {
      public:
        OPCode7B()
          : Instruction(-1, 0x7B, nullptr) {}
        OPCode7B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7B, Maker) {}
        OPCode7B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x7B, Maker) {}
        OPCode7B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7B, Maker) {
            addr++;

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
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
        OPCode7D()
          : Instruction(-1, 0x7D, nullptr) {}
        OPCode7D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7D, Maker) {}
        OPCode7D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x7D, Maker) {}
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
        OPCode7E()
          : Instruction(-1, 0x7E, nullptr) {}
        OPCode7E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7E, Maker) {}
        OPCode7E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x7E, Maker) {}
        OPCode7E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x7F
    class OPCode7F : public Instruction {
      public:
        OPCode7F()
          : Instruction(-1, 0x7F, nullptr) {}
        OPCode7F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x7F, Maker) {}
        OPCode7F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x7F, Maker) {}
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
        OPCode80()
          : Instruction(-1, 0x80, nullptr) {}
        OPCode80(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x80, Maker) {}
        OPCode80(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x80, Maker) {}
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
        OPCode81()
          : Instruction(-1, 0x81, nullptr) {}
        OPCode81(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x81, Maker) {}
        OPCode81(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x81, Maker) {}
        OPCode81(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x81, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x82
    class OPCode82 : public Instruction {
      public:
        OPCode82()
          : Instruction(-1, 0x82, nullptr) {}
        OPCode82(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x82, Maker) {}
        OPCode82(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x82, Maker) {}
        OPCode82(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x82, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x83
    class OPCode83 : public Instruction {
      public:
        OPCode83()
          : Instruction(-1, 0x83, nullptr) {}
        OPCode83(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x83, Maker) {}
        OPCode83(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x83, Maker) {}
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
        }
    };
    // 0x84
    class OPCode84 : public Instruction {
      public:
        OPCode84()
          : Instruction(-1, 0x84, nullptr) {}
        OPCode84(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x84, Maker) {}
        OPCode84(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x84, Maker) {}
        OPCode84(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x84, Maker) {
            addr++;
        }
    };
    // 0x85
    class OPCode85 : public Instruction {
      public:
        OPCode85()
          : Instruction(-1, 0x85, nullptr) {}
        OPCode85(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x85, Maker) {}
        OPCode85(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x85, Maker) {}
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
        OPCode86()
          : Instruction(-1, 0x86, nullptr) {}
        OPCode86(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x86, Maker) {}
        OPCode86(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x86, Maker) {}
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
            }
        }
    };
    // 0x87
    class OPCode87 : public Instruction {
      public:
        OPCode87()
          : Instruction(-1, 0x87, nullptr) {}
        OPCode87(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x87, Maker) {}
        OPCode87(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x87, Maker) {}
        OPCode87(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x87, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x88
    class OPCode88 : public Instruction {
      public:
        OPCode88()
          : Instruction(-1, 0x88, nullptr) {}
        OPCode88(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x88, Maker) {}
        OPCode88(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x88, Maker) {}
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
        OPCode89()
          : Instruction(-1, 0x89, nullptr) {}
        OPCode89(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x89, Maker) {}
        OPCode89(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x89, Maker) {}
        OPCode89(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x89, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            }
        }
    };
    // 0x8A
    class OPCode8A : public Instruction {
      public:
        OPCode8A()
          : Instruction(-1, 0x8A, nullptr) {}
        OPCode8A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8A, Maker) {}
        OPCode8A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x8A, Maker) {}
        OPCode8A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) {
                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            }
        }
    };
    // 0x8B
    class OPCode8B : public Instruction {
      public:
        OPCode8B()
          : Instruction(-1, 0x8B, nullptr) {}
        OPCode8B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8B, Maker) {}
        OPCode8B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x8B, Maker) {}
        OPCode8B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x8C
    class OPCode8C : public Instruction {
      public:
        OPCode8C()
          : Instruction(-1, 0x8C, nullptr) {}
        OPCode8C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8C, Maker) {}
        OPCode8C(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x8C, Maker) {}
        OPCode8C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x8D
    class OPCode8D : public Instruction {
      public:
        OPCode8D()
          : Instruction(-1, 0x8D, nullptr) {}
        OPCode8D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8D, Maker) {}
        OPCode8D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x8D, Maker) {}
        OPCode8D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x8E
    class OPCode8E : public Instruction {
      public:
        OPCode8E()
          : Instruction(-1, 0x8E, nullptr) {}
        OPCode8E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x8E, Maker) {}
        OPCode8E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x8E, Maker) {}
        OPCode8E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x8E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x90
    class OPCode90 : public Instruction {
      public:
        OPCode90()
          : Instruction(-1, 0x90, nullptr) {}
        OPCode90(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x90, Maker) {}
        OPCode90(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x90, Maker) {}
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
        OPCode91()
          : Instruction(-1, 0x91, nullptr) {}
        OPCode91(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x91, Maker) {}
        OPCode91(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x91, Maker) {}
        OPCode91(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x91, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
        }
    };
    // 0x92
    class OPCode92 : public Instruction {
      public:
        OPCode92()
          : Instruction(-1, 0x92, nullptr) {}
        OPCode92(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x92, Maker) {}
        OPCode92(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x92, Maker) {}
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
        OPCode93()
          : Instruction(-1, 0x93, nullptr) {}
        OPCode93(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x93, Maker) {}
        OPCode93(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x93, Maker) {}
        OPCode93(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x93, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    // 0x94
    class OPCode94 : public Instruction {
      public:
        OPCode94()
          : Instruction(-1, 0x94, nullptr) {}
        OPCode94(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x94, Maker) {}
        OPCode94(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x94, Maker) {}
        OPCode94(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x94, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x95
    class OPCode95 : public Instruction {
      public:
        OPCode95()
          : Instruction(-1, 0x95, nullptr) {}
        OPCode95(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x95, Maker) {}
        OPCode95(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x95, Maker) {}
        OPCode95(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x95, Maker) {
            addr++;
        }
    };
    // 0x96
    class OPCode96 : public Instruction {
      public:
        OPCode96()
          : Instruction(-1, 0x96, nullptr) {}
        OPCode96(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x96, Maker) {}
        OPCode96(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x96, Maker) {}
        OPCode96(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x96, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x97
    class OPCode97 : public Instruction {
      public:
        OPCode97()
          : Instruction(-1, 0x97, nullptr) {}
        OPCode97(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x97, Maker) {}
        OPCode97(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x97, Maker) {}
        OPCode97(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x97, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x98
    class OPCode98 : public Instruction {
      public:
        OPCode98()
          : Instruction(-1, 0x98, nullptr) {}
        OPCode98(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x98, Maker) {}
        OPCode98(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x98, Maker) {}
        OPCode98(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x98, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    // 0x99
    class OPCode99 : public Instruction {
      public:
        OPCode99()
          : Instruction(-1, 0x99, nullptr) {}
        OPCode99(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x99, Maker) {}
        OPCode99(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x99, Maker) {}
        OPCode99(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x99, Maker) {
            addr++;
        }
    };
    // 0x9A
    class OPCode9A : public Instruction {
      public:
        OPCode9A()
          : Instruction(-1, 0x9A, nullptr) {}
        OPCode9A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9A, Maker) {}
        OPCode9A(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x9A, Maker) {}
        OPCode9A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9A, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                case 0x02: {

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x03:
                case 0x04: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
            }
        }
    };
    // 0x9B
    class OPCode9B : public Instruction {
      public:
        OPCode9B()
          : Instruction(-1, 0x9B, nullptr) {}
        OPCode9B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9B, Maker) {}
        OPCode9B(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x9B, Maker) {}
        OPCode9B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x9D
    class OPCode9D : public Instruction {
      public:
        OPCode9D()
          : Instruction(-1, 0x9D, nullptr) {}
        OPCode9D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9D, Maker) {}
        OPCode9D(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x9D, Maker) {}
        OPCode9D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    // 0x9E
    class OPCode9E : public Instruction {
      public:
        OPCode9E()
          : Instruction(-1, 0x9E, nullptr) {}
        OPCode9E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9E, Maker) {}
        OPCode9E(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x9E, Maker) {}
        OPCode9E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    // 0x9F
    class OPCode9F : public Instruction {
      public:
        OPCode9F()
          : Instruction(-1, 0x9F, nullptr) {}
        OPCode9F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0x9F, Maker) {}
        OPCode9F(int addr, Builder* Maker)
          : Instruction(addr, "???", 0x9F, Maker) {}
        OPCode9F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x9F, Maker) {
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

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    // 0xFF
    class OPCodeFF : public Instruction {
      public:
        OPCodeFF()
          : Instruction(-1, 0xFF, nullptr) {}
        OPCodeFF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
          : Instruction(addr, idx_row, doc, "???", 0xFF, Maker) {}
        OPCodeFF(int addr, Builder* Maker)
          : Instruction(addr, "???", 0xFF, Maker) {}
        OPCodeFF(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xFF, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

    std::shared_ptr<Instruction> create_instruction_from_dat(int& addr, ssd::Buffer& dat_content, int function_type) override {
        int OP = (dat_content[addr] & 0xFF);

        if (CS1UIFiles.contains(scene_name) && (OP == 0x13)) {
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
                case 0x21:
                    return std::make_shared<OPCode21>(addr, dat_content, this);
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
                // case 0x4E: return std::make_shared<OPCode4E>(addr,dat_content,this);
                case 0x4F:
                    return std::make_shared<OPCode4F>(addr, dat_content, this);
                // case 0x50: return std::make_shared<OPCode50>(addr,dat_content,this);
                case 0x51:
                    return std::make_shared<OPCode51>(addr, dat_content, this);
                case 0x52:
                    return std::make_shared<OPCode52>(addr, dat_content, this);
                case 0x53:
                    return std::make_shared<OPCode53>(addr, dat_content, this);

                case 0x55:
                    return std::make_shared<OPCode55>(addr, dat_content, this);
                case 0x56:
                    return std::make_shared<OPCode56>(addr, dat_content, this);
                case 0x57:
                    return std::make_shared<OPCode57>(addr, dat_content, this);
                case 0x58:
                    return std::make_shared<OPCode58>(addr, dat_content, this);
                case 0x59:
                    return std::make_shared<OPCode59>(addr, dat_content, this);
                case 0x5A:
                    return std::make_shared<OPCode5A>(addr, dat_content, this);
                case 0x5B:
                    return std::make_shared<OPCode5B>(addr, dat_content, this);
                case 0x5C:
                    return std::make_shared<OPCode5C>(addr, dat_content, this);
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
                case 0x62:
                    return std::make_shared<OPCode62>(addr, dat_content, this);
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
                case 0x71:
                    return std::make_shared<OPCode71>(addr, dat_content, this);
                case 0x72:
                    return std::make_shared<OPCode72>(addr, dat_content, this);
                case 0x73:
                    return std::make_shared<OPCode73>(addr, dat_content, this);
                case 0x74:
                    return std::make_shared<OPCode74>(addr, dat_content, this);

                case 0x77:
                    return std::make_shared<OPCode77>(addr, dat_content, this);
                case 0x78:
                    return std::make_shared<OPCode78>(addr, dat_content, this);
                case 0x79:
                    return std::make_shared<OPCode79>(addr, dat_content, this);
                case 0x7A:
                    return std::make_shared<OPCode7A>(addr, dat_content, this);
                case 0x7B:
                    return std::make_shared<OPCode7B>(addr, dat_content, this);

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
                case 0x8B:
                    return std::make_shared<OPCode8B>(addr, dat_content, this);
                case 0x8C:
                    return std::make_shared<OPCode8C>(addr, dat_content, this);
                case 0x8D:
                    return std::make_shared<OPCode8D>(addr, dat_content, this);
                case 0x8E:
                    return std::make_shared<OPCode8E>(addr, dat_content, this);

                case 0x90:
                    return std::make_shared<OPCode90>(addr, dat_content, this);
                case 0x91:
                    return std::make_shared<OPCode91>(addr, dat_content, this);
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
                case 0x98:
                    return std::make_shared<OPCode98>(addr, dat_content, this);
                case 0x99:
                    return std::make_shared<OPCode99>(addr, dat_content, this);
                case 0x9A:
                    return std::make_shared<OPCode9A>(addr, dat_content, this);
                case 0x9B:
                    return std::make_shared<OPCode9B>(addr, dat_content, this);
                case 0x9D:
                    return std::make_shared<OPCode9D>(addr, dat_content, this);
                case 0x9E:
                    return std::make_shared<OPCode9E>(addr, dat_content, this);
                case 0x9F:
                    return std::make_shared<OPCode9F>(addr, dat_content, this);
                default:
                    throw ssd::exceptions::bad_opcode(OP, addr);
            }
        } else {
            std::shared_ptr<Instruction> res;
            if (function_type == 1) { // the function is a "CreateMonsters" function

                res = std::make_shared<CreateMonsters>(addr, dat_content, this);
            } else if (function_type == 2) { // the function is a "effect" function

                res = std::make_shared<EffectsInstr>(addr, dat_content, this);
            } else if (function_type == 3) {

                res = std::make_shared<ActionTable>(addr, dat_content, this);
            } else if (function_type == 4) {

                res = std::make_shared<AlgoTable>(addr, dat_content, this);
            } else if (function_type == 5) {

                res = std::make_shared<WeaponAttTable>(addr, dat_content, this);
            } else if (function_type == 6) {

                res = std::make_shared<BreakTable>(addr, dat_content, this);
            } else if (function_type == 7) {

                res = std::make_shared<SummonTable>(addr, dat_content, this);
            } else if (function_type == 8) {

                res = std::make_shared<ReactionTable>(addr, dat_content, this);
            } else if (function_type == 9) {

                res = std::make_shared<PartTable>(addr, dat_content, this);
            } else if (function_type == 10) {

                res = std::make_shared<AnimeClipTable>(addr, dat_content, this);
            } else if (function_type == 11) {

                res = std::make_shared<FieldMonsterData>(addr, dat_content, this);
            } else if (function_type == 12) {

                res = std::make_shared<FieldFollowData>(addr, dat_content, this);
            } else if (function_type == 13) {

                res = std::make_shared<FC_autoX>(addr, dat_content, this);
            } else if (function_type == 14) {

                res = std::make_shared<BookData99>(addr, dat_content, this);
            } else if (function_type == 15) {

                res = std::make_shared<BookDataX>(addr, dat_content, this);
            } else if (function_type == 16) {

                res = std::make_shared<AddCollision>(addr, dat_content, this);
            }
            if ((uint8_t)dat_content[addr] != 1) throw ssd::exceptions::past_the_end_addr();
            if (this->goal < addr) throw ssd::exceptions::past_the_end_addr();
            return res;
        }

        return {};
    }
    bool create_header_from_dat(ssd::Buffer& dat_content) override {

        // Header structure:
        // The first 0x20 is something used to check if the file is a valid file: it will always be that value
        // The second 0x20 is the offset for the file name (shouldn't change too)
        // The next integer is the position of the first pointer
        // The 4th: probably the length in bytes of the pointer section
        // The fifth: probably the position of the "names positions" section (right after the pointer section)
        // The sixth: the number of functions inside the file
        // the seventh: the position one byte after the 0x00 separator at the end of the functions section
        // the eighth: 0xABCDEF00 => seems to always be there (no idea why)
        // Then the name of the file
        // Then the pointer section
        // Then the "names positions" section
        // Then the functions section
        // Done; here the "function" objects holds the number of functions, the addr, name positions
        // everything else can be deduced to recreate the header
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
    std::shared_ptr<Instruction> create_instruction_from_xlsx(int& addr, int row, QXlsx::Document& xls_content) override {

        uint OP = xls_content.read(row + 1, 2).toInt();

        if (CS1UIFiles.contains(scene_name) && (OP == 0x13)) {
            return std::make_shared<UI_OP13>(addr, row, xls_content, this);
        }

        switch (OP) {
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
            case 0x21:
                return std::make_shared<OPCode21>(addr, row, xls_content, this);
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
            // case 0x4E: return std::make_shared<OPCode4E>(addr,row, xls_content,this);
            case 0x4F:
                return std::make_shared<OPCode4F>(addr, row, xls_content, this);
            // case 0x50: return std::make_shared<OPCode50>(addr,row, xls_content,this);
            case 0x51:
                return std::make_shared<OPCode51>(addr, row, xls_content, this);
            case 0x52:
                return std::make_shared<OPCode52>(addr, row, xls_content, this);
            case 0x53:
                return std::make_shared<OPCode53>(addr, row, xls_content, this);

            case 0x55:
                return std::make_shared<OPCode55>(addr, row, xls_content, this);
            case 0x56:
                return std::make_shared<OPCode56>(addr, row, xls_content, this);
            case 0x57:
                return std::make_shared<OPCode57>(addr, row, xls_content, this);
            case 0x58:
                return std::make_shared<OPCode58>(addr, row, xls_content, this);
            case 0x59:
                return std::make_shared<OPCode59>(addr, row, xls_content, this);
            case 0x5A:
                return std::make_shared<OPCode5A>(addr, row, xls_content, this);
            case 0x5B:
                return std::make_shared<OPCode5B>(addr, row, xls_content, this);
            case 0x5C:
                return std::make_shared<OPCode5C>(addr, row, xls_content, this);
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
            case 0x62:
                return std::make_shared<OPCode62>(addr, row, xls_content, this);
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
            case 0x71:
                return std::make_shared<OPCode71>(addr, row, xls_content, this);
            case 0x72:
                return std::make_shared<OPCode72>(addr, row, xls_content, this);
            case 0x73:
                return std::make_shared<OPCode73>(addr, row, xls_content, this);
            case 0x74:
                return std::make_shared<OPCode74>(addr, row, xls_content, this);

            case 0x77:
                return std::make_shared<OPCode77>(addr, row, xls_content, this);
            case 0x78:
                return std::make_shared<OPCode78>(addr, row, xls_content, this);
            case 0x79:
                return std::make_shared<OPCode79>(addr, row, xls_content, this);
            case 0x7A:
                return std::make_shared<OPCode7A>(addr, row, xls_content, this);
            case 0x7B:
                return std::make_shared<OPCode7B>(addr, row, xls_content, this);

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
            case 0x8B:
                return std::make_shared<OPCode8B>(addr, row, xls_content, this);
            case 0x8C:
                return std::make_shared<OPCode8C>(addr, row, xls_content, this);
            case 0x8D:
                return std::make_shared<OPCode8D>(addr, row, xls_content, this);
            case 0x8E:
                return std::make_shared<OPCode8E>(addr, row, xls_content, this);

            case 0x90:
                return std::make_shared<OPCode90>(addr, row, xls_content, this);
            case 0x91:
                return std::make_shared<OPCode91>(addr, row, xls_content, this);
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
            case 0x98:
                return std::make_shared<OPCode98>(addr, row, xls_content, this);
            case 0x99:
                return std::make_shared<OPCode99>(addr, row, xls_content, this);
            case 0x9A:
                return std::make_shared<OPCode9A>(addr, row, xls_content, this);
            case 0x9B:
                return std::make_shared<OPCode9B>(addr, row, xls_content, this);
            case 0x9D:
                return std::make_shared<OPCode9D>(addr, row, xls_content, this);
            case 0x9E:
                return std::make_shared<OPCode9E>(addr, row, xls_content, this);
            case 0x9F:
                return std::make_shared<OPCode9F>(addr, row, xls_content, this);

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
            default:
                std::stringstream stream;
                stream << "L'OP code " << std::hex << OP << " n'est pas défini !! " << this->scene_name;
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

#endif // CS1INSTRUCTIONSSET_H
