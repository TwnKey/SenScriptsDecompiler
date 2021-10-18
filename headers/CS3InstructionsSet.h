#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "headers/functions.h"
#include "headers/utilities.h"

class CS3Builder : public Builder {
  public:
    CS3Builder() = default;

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

                    // there is a XOR,XOR EDI which causes a jump that ignores the strcpy
                    if (param != 0) instr->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    instr->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                } else if (control_byte3 == 0xFF) {
                    instr->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
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

                    std::shared_ptr<Instruction> instr2 = instr->maker.value()->create_instruction_from_dat(addr, content, 0);

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
        explicit CreateMonsters(int& addr)
          : Instruction(addr, "CreateMonsters", 256) { }
        CreateMonsters(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "CreateMonsters", 256, Maker) {
            int initial_addr = addr;
            if (Maker->goal < addr + 0x4C) {
                addr = initial_addr;
                throw ssd::exceptions::unexpected_operand();
            }
            ssd::Buffer map = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", map));

            ssd::Buffer remaining1 = ReadSubByteArray(content, addr, 0x10 - map.size());
            auto fill1 = Operande(addr, "fill", remaining1);
            fill1.set_bytes_to_fill(0x10);
            this->add_operande(fill1);

            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RSI + 0x28
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4))); // 0x2C

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x30
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x32
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x34
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x36

            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4))); // 0x38

            ssd::Buffer btl_name = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", btl_name));

            ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - btl_name.size());
            auto fill = Operande(addr, "fill", remaining);
            fill.set_bytes_to_fill(0x10);
            this->add_operande(fill);


            ssd::Buffer array = ReadSubByteArray(content, addr, 4);
            this->add_operande(Operande(addr, "int", array));
            int first = ReadIntegerFromByteArray(0, array);

            int cnt = 0;
            do {
                int counter = 0;
                if (Maker->goal < initial_addr + 0x4C + cnt * (0x90)) {
                    addr = initial_addr;
                    throw ssd::exceptions::unexpected_operand();
                }
                do {
                    counter++;
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->add_operande(Operande(addr, "string", monsters_name));

                    remaining = ReadSubByteArray(content, addr, 0x10 - monsters_name.size());
                    fill = Operande(addr, "fill", remaining);
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

                    remaining = ReadSubByteArray(content, addr, 12 - monsters_name.size());
                    fill = Operande(addr, "bytearray", remaining);
                    fill.set_bytes_to_fill(12);
                    this->add_operande(fill);
                }

                array = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "int", array));
                first = ReadIntegerFromByteArray(0, array);
                cnt++;
            } while (first != -1);

            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x18))); //??

        }
    };
    class EffectsInstr : public Instruction {
      public:
        explicit EffectsInstr(int& addr)
          : Instruction(addr, "EffectsInstr", 257) { }
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
        explicit ActionTable(int& addr)
          : Instruction(addr, "ActionTable", 258) { }
        ActionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ActionTable", 258, Maker) {
            int cnt = 0;
            int16_t shrt = 0;
            do {

                shrt = ReadShortFromByteArray(addr, content);
                if (shrt == -1) break;
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                this->add_operande(Operande(addr, "short", short_bytes));

                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 57
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 58->5B
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5C->5F
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 60->63
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 64
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 65
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 66
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 67
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 68
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 6A
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 6C
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 6E
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 70
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 74
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 78
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RSP+7C
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-80
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-7C
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-78
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-74
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-70
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-6C
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-68
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-64
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-60
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-5C
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-58
                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // RBP-54
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RBP-50
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                /*ssd::Buffer current_byte = ReadSubByteArray(content, addr,1);
                this->add_operande(Operande(addr,"byte", current_byte));*///Strange byte dealt in 14027dcf0

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

                // The last string can be 0x20 long and the following 0x20 can terminate the string, I think
                // it's their purpose here
                str = ReadStringSubByteArray(content, addr);
                this->add_operande(Operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x40 - str.size());
                fill = Operande(addr, "fill", remaining);
                fill.set_bytes_to_fill(0x40);
                this->add_operande(fill);

                cnt++;
            } while (cnt < 0x40);
            // also here, can't explain what is the purpose of those bytes
            while (shrt == -1) {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                this->add_operande(Operande(addr, "short", short_bytes));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0xC1);
                this->add_operande(Operande(addr, "bytearray", remaining));

                shrt = ReadShortFromByteArray(addr, content);
            }
        }
    };
    class AlgoTable : public Instruction {
      public:
        explicit AlgoTable(int& addr)
          : Instruction(addr, "AlgoTable", 259) { }
        AlgoTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "AlgoTable", 259, Maker) {
            int cnt = 0;

            // A complete guess for the moment; nothing looks like a pointer here though
            // The operands actually all look like bytes to me except the first one which seems like
            // a character ID so a short
            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                int16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->add_operande(Operande(addr, "short", short_bytes));

                if (shrt == 0) break;

                this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1E)));

                cnt++;
            } while (cnt < 0x40);
            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x1E)));
        }
    };
    class WeaponAttTable : public Instruction {
      public:
        explicit WeaponAttTable(int& addr)
          : Instruction(addr, "WeaponAttTable", 260) { }
        WeaponAttTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "WeaponAttTable", 260, Maker) {

            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 4)));
        }
    };
    class BreakTable : public Instruction {
      public:
        explicit BreakTable(int& addr)
          : Instruction(addr, "BreakTable", 261) { }
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
        explicit SummonTable(int& addr)
          : Instruction(addr, "SummonTable", 262) { }
        SummonTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "SummonTable", 262, Maker) {
            int cnt = 0;
            do {
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
            } while (cnt < 0x40);
            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x24 - 2)));
        }
    };
    class ReactionTable
      : public Instruction // 140142002
    {
      public:
        explicit ReactionTable(int& addr)
          : Instruction(addr, "ReactionTable", 263) { }
        ReactionTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "ReactionTable", 263, Maker) {
            int cnt = 0;
            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                uint16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->add_operande(Operande(addr, "short", short_bytes));
                if (shrt == 0x0) break;
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
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
            } while (cnt < 0x8);
            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x3A)));
        }
    };
    class PartTable
      : public Instruction // 14019797c
    {
      public:
        explicit PartTable(int& addr)
          : Instruction(addr, "PartTable", 264) { }
        PartTable(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "PartTable", 264, Maker) {
            int cnt = 0;
            do {
                ssd::Buffer int_bytes = ReadSubByteArray(content, addr, 4);
                uint32_t integer = ReadIntegerFromByteArray(0, int_bytes);
                this->add_operande(Operande(addr, "int", int_bytes));
                if (integer == 0xFF) break;

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
            } while (cnt < 0x4);
            this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x40)));
        }
    };

    class AnimeClipTable
      : public Instruction // from CS3
    {
      public:
        explicit AnimeClipTable(int& addr)
          : Instruction(addr, "AnimeClipTable", 265) { }
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
            // I think this is some sort of a table with ID at the beginning and then two names
        }
    };
    class AnimeClipData : public Instruction {
      public:
        explicit AnimeClipData(int& addr)
          : Instruction(addr, "AnimeClipData", 273) { }
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
        explicit FieldMonsterData(int& addr)
          : Instruction(addr, "FieldMonsterData", 266) { }
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
        explicit FieldFollowData(int& addr)
          : Instruction(addr, "FieldMonsterData", 267) { }
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
        explicit FC_autoX(int& addr)
          : Instruction(addr, "FC_autoX", 268) { }
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
        explicit BookData99(int& addr)
          : Instruction(addr, "BookData99", 269) { }
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
        explicit BookDataX(int& addr)
          : Instruction(addr, "BookDataX", 270) { }
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
        explicit OPCode0(int& addr)
          : Instruction(addr, "Instruction 0", 0) { }
        OPCode0(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Instruction 0", 0, Maker) {
            addr++;
        }
    };
    class OPCode1 : public Instruction {
      public:
        explicit OPCode1(int& addr)
          : Instruction(addr, "Return", 1) { }
        OPCode1(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Return", 1, Maker) {
            addr++;
        }
    };
    class OPCode2 : public Instruction {
      public:
        explicit OPCode2(int& addr)
          : Instruction(addr, "Call", 2) { }
        OPCode2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "Call", 2, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            ssd::Buffer function_name = ReadStringSubByteArray(content, addr);
            this->add_operande(Operande(addr, "string", function_name));
            ssd::Buffer control_byte2_arr = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte2_arr));

            int control_byte2 = (uint8_t)control_byte2_arr[0];

            if (control_byte2 != 0) {
                for (int i = 0; i < control_byte2; i++) {
                    fun_1403c90e0(addr, content, this, 0);
                }
            }
            switch ((unsigned char)control_byte[0]) {
                case 0xB:
                    break;
            }
        }
    };
    class OPCode3 : public Instruction {
      public:
        explicit OPCode3(int& addr)
          : Instruction(addr, "???", 3) { }
        OPCode3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 3, Maker) {
            addr++;
            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode4 : public Instruction {
      public:
        explicit OPCode4(int& addr)
          : Instruction(addr, "???", 4) { }
        OPCode4(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 4, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode5 : public Instruction {
      public:
        explicit OPCode5(int& addr)
          : Instruction(addr, "???", 5) { }
        OPCode5(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 5, Maker) {
            addr++;
            sub05(addr, content, this);
            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4))); // pointer
        }
    };
    class OPCode6 : public Instruction {
      public:
        explicit OPCode6(int& addr)
          : Instruction(addr, "???", 6) { }
        OPCode6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 6, Maker) {
            addr++;
            sub05(addr, content, this);
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);

            ssd::Buffer ptr;
            this->add_operande(Operande(addr, "byte", control_byte));

            for (int i = 0; i < (unsigned char)control_byte[0]; i++) {
                ssd::Buffer iVar3_arr = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "int", iVar3_arr));

                ptr = ReadSubByteArray(content, addr, 4);
                this->add_operande(Operande(addr, "pointer", ptr));
            }

            this->add_operande(Operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode7 : public Instruction {
      public:
        explicit OPCode7(int& addr)
          : Instruction(addr, "???", 7) { }
        OPCode7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                    fun_1403c90e0(addr, content, this, 0);

                    break;
                case 1:
                    fun_1403c90e0(addr, content, this, 0);
                    break;
                case 2:
                    fun_1403c90e0(addr, content, this, 0);
                    break;
            }
        }
    };
    class OPCode8 : public Instruction {
      public:
        explicit OPCode8(int& addr)
          : Instruction(addr, "???", 8) { }
        OPCode8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 8, Maker) {
            addr++;
            sub05(addr, content, this);
        }
    };
    class OPCodeA : public Instruction {
      public:
        explicit OPCodeA(int& addr)
          : Instruction(addr, "???", 0xA) { }
        OPCodeA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xA, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            sub05(addr, content, this);
        }
    };
    class OPCodeC : public Instruction {
      public:
        explicit OPCodeC(int& addr)
          : Instruction(addr, "???", 0xC) { }
        OPCodeC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xC, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode0D : public Instruction {
      public:
        explicit OPCode0D(int& addr)
          : Instruction(addr, "???", 0x0D) { }
        OPCode0D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x0D, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                    sub05(addr, content, this);
                    break;
            }
        }
    };
    class OPCodeE : public Instruction {
      public:
        explicit OPCodeE(int& addr)
          : Instruction(addr, "???", 0xE) { }
        OPCodeE(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0xE, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode10 : public Instruction {
      public:
        explicit OPCode10(int& addr)
          : Instruction(addr, "???", 0x10) { }
        OPCode10(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x10, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode11 : public Instruction {
      public:
        explicit OPCode11(int& addr)
          : Instruction(addr, "???", 0x11) { }
        OPCode11(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x11, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode12 : public Instruction {
      public:
        explicit OPCode12(int& addr)
          : Instruction(addr, "???", 0x12) { }
        OPCode12(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x12, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode13 : public Instruction {
      public:
        explicit OPCode13(int& addr)
          : Instruction(addr, "???", 0x13) { }
        OPCode13(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x13, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode14 : public Instruction {
      public:
        explicit OPCode14(int& addr)
          : Instruction(addr, "???", 0x14) { }
        OPCode14(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x14, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode15 : public Instruction {
      public:
        explicit OPCode15(int& addr)
          : Instruction(addr, "???", 0x15) { }
        OPCode15(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x15, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode16 : public Instruction {
      public:
        explicit OPCode16(int& addr)
          : Instruction(addr, "???", 0x16) { }
        OPCode16(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x16, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode17 : public Instruction {
      public:
        explicit OPCode17(int& addr)
          : Instruction(addr, "???", 0x17) { }
        OPCode17(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x17, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode18 : public Instruction {
      public:
        explicit OPCode18(int& addr)
          : Instruction(addr, "???", 0x18) { }
        OPCode18(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x18, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            sub05(addr, content, this);
        }
    };
    class OPCode1A : public Instruction {
      public:
        explicit OPCode1A(int& addr)
          : Instruction(addr, "???", 0x1A) { }
        OPCode1A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode1D : public Instruction {
      public:
        explicit OPCode1D(int& addr)
          : Instruction(addr, "???", 0x1D) { }
        OPCode1D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 6
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // E
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x12
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x16
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x1A
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x1E
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x22
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            // i think this one is the id of the battle function it triggers
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode1E : public Instruction {
      public:
        explicit OPCode1E(int& addr)
          : Instruction(addr, "???", 0x1E) { }
        OPCode1E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode1F : public Instruction {
      public:
        explicit OPCode1F(int& addr)
          : Instruction(addr, "???", 0x1F) { }
        OPCode1F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x1F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCode20 : public Instruction {
      public:
        explicit OPCode20(int& addr)
          : Instruction(addr, "???", 0x20) { }
        OPCode20(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x20, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
        }
    };
    class OPCode21 : public Instruction {
      public:
        explicit OPCode21(int& addr)
          : Instruction(addr, "???", 0x21) { }
        OPCode21(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x21, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode22 : public Instruction {
      public:
        explicit OPCode22(int& addr)
          : Instruction(addr, "???", 0x22) { }
        OPCode22(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x22, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            reading_dialog(addr, content, this);
        }
    };
    class OPCode23 : public Instruction {
      public:
        explicit OPCode23(int& addr)
          : Instruction(addr, "???", 0x23) { }
        OPCode23(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x23, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode24 : public Instruction {
      public:
        explicit OPCode24(int& addr)
          : Instruction(addr, "???", 0x24) { }
        OPCode24(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x24, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            reading_dialog(addr, content, this);
        }
    };
    class OPCode25 : public Instruction {
      public:
        explicit OPCode25(int& addr)
          : Instruction(addr, "???", 0x25) { }
        OPCode25(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x25, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode26 : public Instruction {
      public:
        explicit OPCode26(int& addr)
          : Instruction(addr, "???", 0x26) { }
        OPCode26(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x26, Maker) {
            addr++;
        }
    };
    class OPCode27 : public Instruction {
      public:
        explicit OPCode27(int& addr)
          : Instruction(addr, "???", 0x27) { }
        OPCode27(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x27, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode28 : public Instruction {
      public:
        explicit OPCode28(int& addr)
          : Instruction(addr, "???", 0x28) { }
        OPCode28(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x28, Maker) {
            addr++;
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode29 : public Instruction {
      public:
        explicit OPCode29(int& addr)
          : Instruction(addr, "???", 0x29) { }
        OPCode29(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x29, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", control_byte2));
            if ((unsigned char)control_byte2[0] < 4) {
                switch ((unsigned char)control_byte[0]) {
                    case 0:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 1:
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 2:
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 4:
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 5:
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 6:
                    case 7:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 8:
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        fun_1403c90e0(addr, content, this, 1);
                        fun_1403c90e0(addr, content, this, 1);
                        break;
                    case 0xB:
                        fun_1403c90e0(addr, content, this, 1);
                        break;
                    case 0xC:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0xD:
                    case 0xE:
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                }
            }
        }
    };
    class OPCode2A : public Instruction {
      public:
        explicit OPCode2A(int& addr)
          : Instruction(addr, "???", 0x2A) { }
        OPCode2A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case 2:
                case 3:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };

    class OPCode2B : public Instruction {
      public:
        explicit OPCode2B(int& addr)
          : Instruction(addr, "???", 0x2B) { }
        OPCode2B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            if ((unsigned char)control_byte[0] == 0) {
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
            if ((unsigned char)control_byte[0] == 1) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    class OPCode2C : public Instruction {
      public:
        explicit OPCode2C(int& addr)
          : Instruction(addr, "???", 0x2C) { }
        OPCode2C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            // le parsing du string est bizarre (parse 0xC) mais vérifié
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
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode2D : public Instruction {
      public:
        explicit OPCode2D(int& addr)
          : Instruction(addr, "???", 0x2D) { }
        OPCode2D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode2E : public Instruction {
      public:
        explicit OPCode2E(int& addr)
          : Instruction(addr, "???", 0x2E) { }
        OPCode2E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2E, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte2));

            switch ((unsigned char)control_byte[0]) {
                case 0x01:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5->8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 9->C
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // D

                    break;
                case 0x03:
                case 0x02:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // 5
                    break;
            }
        }
    };
    class OPCode2F : public Instruction {
      public:
        explicit OPCode2F(int& addr)
          : Instruction(addr, "???", 0x2F) { }
        OPCode2F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x2F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 1:
                case 0:
                case 2:
                case 4:
                case 5:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 6:
                case 7:

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 8:

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
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr))); // 16
                    break;
                case 9:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xA:
                case 0xB:
                case 0xC:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0xE:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0xF:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode30 : public Instruction {
      public:
        explicit OPCode30(int& addr)
          : Instruction(addr, "???", 0x30) { }
        OPCode30(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x30, Maker) {
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
        }
    };
    class OPCode31 : public Instruction {
      public:
        explicit OPCode31(int& addr)
          : Instruction(addr, "???", 0x31) { }
        OPCode31(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x31, Maker) {
            addr++;

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode32 : public Instruction {
      public:
        explicit OPCode32(int& addr)
          : Instruction(addr, "???", 0x32) { }
        OPCode32(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x32, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case '\n':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    return;

                case '\v':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    return;

                case '\f':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0); // 0xFF
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    return;
            }
            if (((unsigned char)control_byte[0] - 0xD) < 2) {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            } else if (1 < (control_byte[0] - 0xfU)) {
                switch ((unsigned char)control_byte[0]) {
                    case '\x11':
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case '\x12':

                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case '\x13':
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;

                    default:
                        if ((unsigned char)(control_byte[0] - 0x14U) < 2) {
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 3
                            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // F
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        }
                        break;
                }

            } else {
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
            switch ((unsigned char)control_byte[0]) {
                case '\x16':
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case '\x17':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\x18':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x19':

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\x1A':
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case '\x1B':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x1C':
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                case '\x1D':
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode33 : public Instruction {
      public:
        explicit OPCode33(int& addr)
          : Instruction(addr, "???", 0x33) { }
        OPCode33(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x33, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x01:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x02:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x03:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x04:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x07:
                case 0x05:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x06:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x08:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x09:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x0A:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x0C:
                case 0x0B:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x0D:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x0E:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x0F:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x10:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x13:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x14:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x15:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x16:

                    break;
                case 0x17:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x18:

                    break;
                case 0x1E:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x19:
                case 0x1A:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x1B:

                    break;
                case 0x20:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                    break;
                case 0x26:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x27:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x2C: {

                    break;
                }
                case 0x2D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1f:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x30:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x31:
                    break;
                case 0x32:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x34:
                    break;
                case 0x35:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x36:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x37:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x38:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x39:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3A:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x3B:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3D:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x4B:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x67:
                case 0x68:
                    break;
                case 0x48:
                case 0x6C:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x46:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                case 0x47:
                    break;
                case 0x4A:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x3c:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x6B:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x33:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x76:
                case 0x29:
                    break;
                case 0x77:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x25:

                    break;
                case 0xB5:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xBA:
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0xBB:
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);

                    break;
                case 0xBC:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 0xBD:

                    break;
                case 0x62: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    ssd::Buffer byte = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", byte));
                    /*switch ((unsigned char)byte[0]){
                        case 0:{
                            ssd::Buffer byte2 = ReadSubByteArray(content, addr, 1);
                            this->add_operande(Operande(addr,"byte", byte2));
                            this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr, 4)));


                            break;
                        }
                        default:
                        throw ssd::exceptions::not_analyzed_yet(byte.at(0));
                    }*/

                    break;
                }
                case 0x52: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x5A: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x5B: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x5C: {
                    ssd::Buffer byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", byte2));
                    /*this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr,"int", ReadSubByteArray(content, addr, 4)));*/

                    break;
                }
                case 0x61: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // rbx+2
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // rbx+3
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // rbx+7
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // rbx+B
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));  // rbx+F
                    break;
                }
                case 0x63: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x6E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x6F: {
                    break;
                }
                case 0x70: {
                    break;
                }
                case 0x73: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x74: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x78: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x7B: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x7D: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x7E: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x7F: {

                    break;
                }
                case 0x87: {
                    break;
                }
                case 0x83: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x85: {

                    break;
                }
                case 0x86: {

                    break;
                }
                case 0x88: {

                    break;
                }
                case 0x89: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x8A: {

                    break;
                }
                case 0x8E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x8F: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x90: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x91: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x92: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x94: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x98: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x9E: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0xA6: {
                    break;
                }
                case 0xA7: {
                    break;
                }
                case 0xA8: {
                    break;
                }
                case 0xA9: {
                    break;
                }
                case 0xAA: {
                    break;
                }
                case 0xAB: {
                    break;
                }
                case 0xAE: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xAF: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xB0: {
                    break;
                }
                case 0xB1: {
                    break;
                }
                case 0xB2: {
                    break;
                }
                case 0xB4: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                }
                case 0xB6: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0xB7: {
                    ssd::Buffer byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", byte2));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0xB8: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0xBE: {

                    break;
                }
                case 0xBF: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xC0: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xC1: {
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0xC2: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0xC3: {

                    break;
                }
                default:
                    throw ssd::exceptions::not_analyzed_yet(control_byte.at(0));
            }
        }
    };
    class OPCode34 : public Instruction {
      public:
        explicit OPCode34(int& addr)
          : Instruction(addr, "???", 0x34) { }
        OPCode34(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x34, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode35 : public Instruction {
      public:
        explicit OPCode35(int& addr)
          : Instruction(addr, "???", 0x35) { }
        OPCode35(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x35, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode36 : public Instruction {
      public:
        explicit OPCode36(int& addr)
          : Instruction(addr, "???", 0x36) { }
        OPCode36(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x36, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    break;
                case 2:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 3:
                case 0x14:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x4:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x5:
                case 0x16:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x6:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x7:
                case 0x17:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x8:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x9:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xA:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8->c
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // c->E
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // E->10
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 10->12
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 12->14
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 14->16
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xB:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0xC:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0xD:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0xE:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0xF:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                case 0x11:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x12:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x13: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x15:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x1f: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x19:
                case 0x18: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x1A: {
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 2->6
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 6->10
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10->14 (d)
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  //(e)
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  //(f)
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  //(0x10)
                    break;
                }
                case 0x1B: {

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x28:
                case 0x29: {

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x2A:
                case 0x2B: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x2C:
                case 0x2D: {
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2E: {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
            }
        }
    };
    class OPCode37 : public Instruction {
      public:
        explicit OPCode37(int& addr)
          : Instruction(addr, "???", 0x37) { }
        OPCode37(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x37, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode38 : public Instruction {
      public:
        explicit OPCode38(int& addr)
          : Instruction(addr, "???", 0x38) { }
        OPCode38(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x38, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };

    class OPCode39 : public Instruction {
      public:
        explicit OPCode39(int& addr)
          : Instruction(addr, "???", 0x39) { }
        OPCode39(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x39, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

    class OPCode3A : public Instruction {
      public:
        explicit OPCode3A(int& addr)
          : Instruction(addr, "???", 0x3A) { }
        OPCode3A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                case 0x4:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x6:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0x7:

                    break;
                case 0x8:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x9:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode3B : public Instruction {
      public:
        explicit OPCode3B(int& addr)
          : Instruction(addr, "???", 0x3B) { }
        OPCode3B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0xFB:
                case 0xFD:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xFE:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0xFF:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0:
                case 0x32:
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    fun_1403c90e0(addr, content, this, 1);

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0->4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4->8
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 8->A
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // A->C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C->10
                    /*this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));//10->12
                    this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr, 2)));//12->14*/
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10->14
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 14->18
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 18->1C

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr))); // x

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 1C+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 1E+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 20+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 22+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 24+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 26+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 28+x
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2A+x
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 2C+x

                    break;
                case 1:
                case 0x33:
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 2:
                case 0x34:
                    fun_1403c90e0(addr, content, this, 1);

                    break;
                case 3:
                case 0x35:
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 4:
                case 0x36:
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 5:
                case 0x37: // very weird one; caution

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
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
                case 0x38:
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
                case 0x3D:
                    break;
                case 0x39:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x3A:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0x3B:
                case 0x3C:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x3E:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 100:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x65:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 0->4
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 4->8
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 8->C
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // C->10
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 10->14
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 14->18
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 18->1C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 1C->20
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 20->24
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 24->28
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 28->2C
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 2C->30
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 30->34
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 34->38
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 38->3C
                    break;
                case 0x96:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                default:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode3C : public Instruction {
      public:
        explicit OPCode3C(int& addr)
          : Instruction(addr, "???", 0x3C) { }
        OPCode3C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x1:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x3:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x4:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
            }
        }
    };
    class OPCode3D : public Instruction {
      public:
        explicit OPCode3D(int& addr)
          : Instruction(addr, "???", 0x3D) { }
        OPCode3D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3D, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode3E : public Instruction {
      public:
        explicit OPCode3E(int& addr)
          : Instruction(addr, "???", 0x3E) { }
        OPCode3E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode3F : public Instruction {
      public:
        explicit OPCode3F(int& addr)
          : Instruction(addr, "???", 0x3F) { }
        OPCode3F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x3F, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode40 : public Instruction {
      public:
        explicit OPCode40(int& addr)
          : Instruction(addr, "???", 0x40) { }
        OPCode40(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x40, Maker) {
            addr++;

            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 1
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "short", control_short));                      // 3
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 9
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // D

            if ((uint16_t)(control + 0x1feU) < 3) {

                this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 11 -> 15
            }
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            if (control == -0x1fb) { // I'm not sure at all of this behavior, but at the same time I'm not even sure it
                                     // happens anywhere (lazy)
                while (content[addr] != '\0') {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
            }
        }
    };
    class OPCode41 : public Instruction {
      public:
        explicit OPCode41(int& addr)
          : Instruction(addr, "0x41", 0x41) { }
        OPCode41(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x41", 0x41, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode42 : public Instruction {
      public:
        explicit OPCode42(int& addr)
          : Instruction(addr, "???", 0x42) { }
        OPCode42(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x42, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));                        // 1
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 14
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 18 Not sure of the types here
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 1A
        }
    };
    class OPCode43 : public Instruction {
      public:
        explicit OPCode43(int& addr)
          : Instruction(addr, "???", 0x43) { }
        OPCode43(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x43, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
                case 0xFF:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 0xFE:
                case 0x05:
                case 0x69:
                case 0x06:
                case 0x6A:
                case 0x0A:
                case 0x0B:
                case 0x0C:

                    break;
                default:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode44 : public Instruction {
      public:
        explicit OPCode44(int& addr)
          : Instruction(addr, "0x44", 0x44) { }
        OPCode44(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x44", 0x44, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode45 : public Instruction {
      public:
        explicit OPCode45(int& addr)
          : Instruction(addr, "0x45", 0x45) { }
        OPCode45(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x45", 0x45, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode46 : public Instruction {
      public:
        explicit OPCode46(int& addr)
          : Instruction(addr, "0x46", 0x46) { }
        OPCode46(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x46", 0x46, Maker) {

            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 3:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 1:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };

    class OPCode47 : public Instruction {
      public:
        explicit OPCode47(int& addr)
          : Instruction(addr, "???", 0x47) { }
        OPCode47(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x47, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode48 : public Instruction {
      public:
        explicit OPCode48(int& addr)
          : Instruction(addr, "???", 0x48) { }
        OPCode48(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x48, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode49 : public Instruction {
      public:
        explicit OPCode49(int& addr)
          : Instruction(addr, "0x49", 0x49) { }
        OPCode49(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x49", 0x49, Maker) {

            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case 4:
                case 5:
                case 6:
                case 0x11:
                case '\t':
                case '\n':
                case '\v':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\b':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\f':
                case 0x17:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case '\r': // pas sûr
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x18)));
                    break;
                case '\x0e':
                case '\x0f':
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode4A : public Instruction {
      public:
        explicit OPCode4A(int& addr)
          : Instruction(addr, "0x4A", 0x4A) { }
        OPCode4A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4A", 0x4A, Maker) {

            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4B : public Instruction {
      public:
        explicit OPCode4B(int& addr)
          : Instruction(addr, "0x4B", 0x4B) { }
        OPCode4B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4B", 0x4B, Maker) {

            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            /*switch (control_shrt)  {
                case 0xfff8:

                    break;
                case 0xfff9:

                    break;

            }*/
        }
    };
    class OPCode4C : public Instruction {
      public:
        explicit OPCode4C(int& addr)
          : Instruction(addr, "0x4C", 0x4C) { }
        OPCode4C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4C", 0x4C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4D : public Instruction {
      public:
        explicit OPCode4D(int& addr)
          : Instruction(addr, "0x4D", 0x4D) { }
        OPCode4D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4D", 0x4D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4E : public Instruction {
      public:
        explicit OPCode4E(int& addr)
          : Instruction(addr, "0x4E", 0x4E) { }
        OPCode4E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4E", 0x4E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4F : public Instruction {
      public:
        explicit OPCode4F(int& addr)
          : Instruction(addr, "0x4F", 0x4F) { }
        OPCode4F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x4F", 0x4F, Maker) {

            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;

                case 3:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
            }
        }
    };
    class OPCode50 : public Instruction {
      public:
        explicit OPCode50(int& addr)
          : Instruction(addr, "0x50", 0x50) { }
        OPCode50(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x50", 0x50, Maker) {
            addr++;
            fun_1403c90e0(addr, content, this, 1);
        }
    };
    class OPCode51 : public Instruction {
      public:
        explicit OPCode51(int& addr)
          : Instruction(addr, "0x51", 0x51) { }
        OPCode51(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x51", 0x51, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode52 : public Instruction {
      public:
        explicit OPCode52(int& addr)
          : Instruction(addr, "0x52", 0x52) { }
        OPCode52(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x52", 0x52, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode53 : public Instruction {
      public:
        explicit OPCode53(int& addr)
          : Instruction(addr, "0x53", 0x53) { }
        OPCode53(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x53", 0x53, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode54 : public Instruction {
      public:
        explicit OPCode54(int& addr)
          : Instruction(addr, "???", 0x54) { }
        OPCode54(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x54, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2;
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x1:
                case 0x3:
                case 0x8:
                case 0x0:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x20 - 4)));
                    break;
                case 10:
                case 0xE:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xB:

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x18:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xD:

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x14:

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x15:

                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                case 0x17:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x30:
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0x24:
                case 0x22:

                case 0x28:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x19:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x1C:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x21:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x23:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x26:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;

                case 0x29:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x2B:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", control_byte2));
                    if ((int)control_byte2[0] == 0) {
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;
                case 0x2C:

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x31:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", control_byte2));
                    if ((int)control_byte2[0] == 0) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    } else if ((int)control_byte2[0] == 1) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;
                case 0x34:

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x35:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x37:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x38:

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x39:

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x3A:

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 0x14

                    break;
                case 0x3B:

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    break;

                case 0x3C:

                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 0xA)));
                    break;
                case 0x3E:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x40:
                case 0x4B:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x42:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x44:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x45:
                case 0x46:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x47:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x48:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x49:
                case 0x4A:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x4C:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x4D:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x4F:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", control_byte2)); // 1
                    switch ((int)control_byte2[0]) {
                        case 0:
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            break;
                        case 1:
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                            break;
                        case 2:
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                            break;
                        case 3:
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            break;
                        case 4:
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                            break;
                        case '\n':
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 3
                            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 4
                            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 10
                            break;
                    }
                    break;
                case 0x50:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x51:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    // this->add_operande(Operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    // this->add_operande(Operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x52:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 0x53:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode55 : public Instruction {
      public:
        explicit OPCode55(int& addr)
          : Instruction(addr, "0x55", 0x55) { }
        OPCode55(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x55", 0x55, Maker) {
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
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode56 : public Instruction {
      public:
        explicit OPCode56(int& addr)
          : Instruction(addr, "0x56", 0x56) { }
        OPCode56(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x56", 0x56, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 14
        }
    };
    class OPCode57 : public Instruction {
      public:
        explicit OPCode57(int& addr)
          : Instruction(addr, "0x57", 0x57) { }
        OPCode57(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x57", 0x57, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode58 : public Instruction {
      public:
        explicit OPCode58(int& addr)
          : Instruction(addr, "0x58", 0x58) { }
        OPCode58(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x58", 0x58, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCode5A : public Instruction {
      public:
        explicit OPCode5A(int& addr)
          : Instruction(addr, "0x5A", 0x5A) { }
        OPCode5A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x5A", 0x5A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode5B : public Instruction {
      public:
        explicit OPCode5B(int& addr)
          : Instruction(addr, "0x5B", 0x5B) { }
        OPCode5B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x5B", 0x5B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
    class OPCode5C : public Instruction {
      public:
        explicit OPCode5C(int& addr)
          : Instruction(addr, "???", 0x5C) { }
        OPCode5C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode5D : public Instruction {
      public:
        explicit OPCode5D(int& addr)
          : Instruction(addr, "???", 0x5D) { }
        OPCode5D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5D, Maker) {
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
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode5E : public Instruction {
      public:
        explicit OPCode5E(int& addr)
          : Instruction(addr, "???", 0x5E) { }
        OPCode5E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x5E, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 1:
                    break;
            }
        }
    };
    class OPCode60 : public Instruction {
      public:
        explicit OPCode60(int& addr)
          : Instruction(addr, "???", 0x60) { }
        OPCode60(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x60, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode61 : public Instruction {
      public:
        explicit OPCode61(int& addr)
          : Instruction(addr, "???", 0x61) { }
        OPCode61(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x61, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 14
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 18
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 1C
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode62 : public Instruction {
      public:
        explicit OPCode62(int& addr)
          : Instruction(addr, "???", 0x62) { }
        OPCode62(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x62, Maker) {
            addr++;
        }
    };
    class OPCode63 : public Instruction {
      public:
        explicit OPCode63(int& addr)
          : Instruction(addr, "???", 0x63) { }
        OPCode63(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x63, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCode64 : public Instruction {
      public:
        explicit OPCode64(int& addr)
          : Instruction(addr, "???", 0x64) { }
        OPCode64(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x64, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode65 : public Instruction {
      public:
        explicit OPCode65(int& addr)
          : Instruction(addr, "???", 0x65) { }
        OPCode65(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x65, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode66 : public Instruction {
      public:
        explicit OPCode66(int& addr)
          : Instruction(addr, "???", 0x66) { }
        OPCode66(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x66, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case 4:
                case 8:
                case 6:
                case 9:
                case 10:
                case 0xC:
                case 0xE:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode67 : public Instruction {
      public:
        explicit OPCode67(int& addr)
          : Instruction(addr, "???", 0x67) { }
        OPCode67(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x67, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode68 : public Instruction {
      public:
        explicit OPCode68(int& addr)
          : Instruction(addr, "???", 0x68) { }
        OPCode68(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x68, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case '\v':
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 2:
                case 3:
                case 4:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\a':
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\b':
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\x18':
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\x1E':
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode69 : public Instruction {
      public:
        explicit OPCode69(int& addr)
          : Instruction(addr, "???", 0x69) { }
        OPCode69(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x69, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 1:
                case 4:
                case 7:
                case 8:
                case 9:
                case 0xB:
                case 0x13:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 2:
                case 3:
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 10:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xC:
                case 0xD:
                case 0x12:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 6:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode6A : public Instruction {
      public:
        explicit OPCode6A(int& addr)
          : Instruction(addr, "???", 0x6A) { }
        OPCode6A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 3:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // not sure?
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 2:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                case 4:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 5:
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
    };
    class OPCode6B : public Instruction {
      public:
        explicit OPCode6B(int& addr)
          : Instruction(addr, "0x6B", 0x6B) { }
        OPCode6B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x6B", 0x6B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode6C : public Instruction {
      public:
        explicit OPCode6C(int& addr)
          : Instruction(addr, "???", 0x6C) { }
        OPCode6C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6C, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode6E : public Instruction {
      public:
        explicit OPCode6E(int& addr)
          : Instruction(addr, "???", 0x6E) { }
        OPCode6E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x6E, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode6F : public Instruction {
      public:
        explicit OPCode6F(int& addr)
          : Instruction(addr, "0x6F", 0x6F) { }
        OPCode6F(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x6F", 0x6F, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode70 : public Instruction {
      public:
        explicit OPCode70(int& addr)
          : Instruction(addr, "???", 0x70) { }
        OPCode70(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x70, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 1:
                case 7:
                case 0xB:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                case 9:
                case 0xC:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 3:
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 6:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 8:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 10:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode72
      : public Instruction // not sure at all
    {
      public:
        explicit OPCode72(int& addr)
          : Instruction(addr, "???", 0x72) { }
        OPCode72(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x72, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            uint16_t control_shrt = ReadShortFromByteArray(0, control_short);
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            if ((unsigned char)(control_byte[0] - 1U) < 2) {
                this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            } else {
                if ((unsigned char)(control_byte[0] - 3U) < 2) {
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
            }
            if (control_shrt != 0xFFFF) {
                if (control_shrt < 0x100) {
                    switch ((unsigned char)control_byte[0]) {
                        case '\x06':
                            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            break;
                    }
                }
            }
        }
    };
    class OPCode73 : public Instruction {
      public:
        explicit OPCode73(int& addr)
          : Instruction(addr, "???", 0x73) { }
        OPCode73(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x73, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 1:

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;

                default:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
            }
        }
    };
    class OPCode74 : public Instruction {
      public:
        explicit OPCode74(int& addr)
          : Instruction(addr, "???", 0x74) { }
        OPCode74(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x74, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 0x14:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 3:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
            }
        }
    };
    class OPCode75 : public Instruction {
      public:
        explicit OPCode75(int& addr)
          : Instruction(addr, "???", 0x75) { }
        OPCode75(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x75, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", control_byte2));
            if ((unsigned char)control_byte2[0] < 4) {
                switch ((unsigned char)control_byte[0]) {
                    case 0:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 1:
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 2:
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 4:
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 5:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 6:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 7:
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                }
            }
        }
    };
    class OPCode76 : public Instruction {
      public:
        explicit OPCode76(int& addr)
          : Instruction(addr, "???", 0x76) { }
        OPCode76(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x76, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode77 : public Instruction {
      public:
        explicit OPCode77(int& addr)
          : Instruction(addr, "???", 0x77) { }
        OPCode77(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x77, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode78 : public Instruction {
      public:
        explicit OPCode78(int& addr)
          : Instruction(addr, "???", 0x78) { }
        OPCode78(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x78, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode79 : public Instruction {
      public:
        explicit OPCode79(int& addr)
          : Instruction(addr, "???", 0x79) { }
        OPCode79(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x79, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
                case 0x5:
                    this->add_operande(Operande(addr, "bytearray", ReadSubByteArray(content, addr, 6))); //??
                    break;
                case 0x7:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode7A : public Instruction {
      public:
        explicit OPCode7A(int& addr)
          : Instruction(addr, "???", 0x7A) { }
        OPCode7A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

                case 0:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 1:
                case 2:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
            }
        }
    };
    class OPCode7B : public Instruction {
      public:
        explicit OPCode7B(int& addr)
          : Instruction(addr, "???", 0x7B) { }
        OPCode7B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

                case 1:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode7C : public Instruction {
      public:
        explicit OPCode7C(int& addr)
          : Instruction(addr, "???", 0x7C) { }
        OPCode7C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

                case 0:

                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 3
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 5
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 7
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 9
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // B
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // D
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // f
                    break;
            }
        }
    };
    class OPCode7D : public Instruction {
      public:
        explicit OPCode7D(int& addr)
          : Instruction(addr, "???", 0x7D) { }
        OPCode7D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode7E : public Instruction {
      public:
        explicit OPCode7E(int& addr)
          : Instruction(addr, "???", 0x7E) { }
        OPCode7E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x7E, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:

                    break;
                case 2:
                case 6:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 3:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 4:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 5:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode80 : public Instruction {
      public:
        explicit OPCode80(int& addr)
          : Instruction(addr, "???", 0x80) { }
        OPCode80(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x80, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode82 : public Instruction {
      public:
        explicit OPCode82(int& addr)
          : Instruction(addr, "???", 0x82) { }
        OPCode82(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x82, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode83 : public Instruction {
      public:
        explicit OPCode83(int& addr)
          : Instruction(addr, "???", 0x83) { }
        OPCode83(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x83, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode84 : public Instruction {
      public:
        explicit OPCode84(int& addr)
          : Instruction(addr, "???", 0x84) { }
        OPCode84(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "???", 0x84, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

                case 3: { // not sure
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->add_operande(Operande(addr, "byte", control_byte2));
                    break;
                }
                default: {
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                }
            }
        }
    };
    class OPCode86 : public Instruction {
      public:
        explicit OPCode86(int& addr)
          : Instruction(addr, "0x86", 0x86) { }
        OPCode86(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x86", 0x86, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 4
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 6
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 8
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // A
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // c
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode87 : public Instruction {
      public:
        explicit OPCode87(int& addr)
          : Instruction(addr, "0x87", 0x87) { }
        OPCode87(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x87", 0x87, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode88 : public Instruction {
      public:
        explicit OPCode88(int& addr)
          : Instruction(addr, "0x88", 0x88) { }
        OPCode88(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x88", 0x88, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode89 : public Instruction {
      public:
        explicit OPCode89(int& addr)
          : Instruction(addr, "0x89", 0x89) { }
        OPCode89(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x89", 0x89, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode8A : public Instruction {
      public:
        explicit OPCode8A(int& addr)
          : Instruction(addr, "0x8A", 0x8A) { }
        OPCode8A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8A", 0x8A, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 1: // NOT SURE!
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 2: // NOT SURE!
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 3:
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
                    break;
                case 0xFE:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xFF:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                default:
                    if (((unsigned char)control_byte[0] - 0x32U) < 2) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    }
                    break;
            }
        }
    };
    class OPCode8B : public Instruction {
      public:
        explicit OPCode8B(int& addr)
          : Instruction(addr, "0x8B", 0x8B) { }
        OPCode8B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8B", 0x8B, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode8C : public Instruction {
      public:
        explicit OPCode8C(int& addr)
          : Instruction(addr, "0x8C", 0x8C) { }
        OPCode8C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8C", 0x8C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 3:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode8D : public Instruction {
      public:
        explicit OPCode8D(int& addr)
          : Instruction(addr, "0x8D", 0x8D) { }
        OPCode8D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8D", 0x8D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode8E : public Instruction {
      public:
        explicit OPCode8E(int& addr)
          : Instruction(addr, "0x8E", 0x8E) { }
        OPCode8E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8E", 0x8E, Maker) {
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
    class OPCode8F : public Instruction {
      public:
        explicit OPCode8F(int& addr)
          : Instruction(addr, "0x8F", 0x8F) { }
        OPCode8F(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x8F", 0x8F, Maker) {
            addr++;
        }
    };
    class OPCode98 : public Instruction {
      public:
        explicit OPCode98(int& addr)
          : Instruction(addr, "0x98", 0x98) { }
        OPCode98(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x98", 0x98, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->add_operande(Operande(addr, "short", control_short));
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            switch (control_shrt) {
                case 1:
                case 2:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 3:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3E8:
                case 0x3E9:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode90 : public Instruction {
      public:
        explicit OPCode90(int& addr)
          : Instruction(addr, "0x90", 0x90) { }
        OPCode90(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x90", 0x90, Maker) {
            addr++;
            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode91 : public Instruction {
      public:
        explicit OPCode91(int& addr)
          : Instruction(addr, "0x91", 0x91) { }
        OPCode91(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x91", 0x91, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                case 5:
                case 0xA:
                case 0xB:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode92 : public Instruction {
      public:
        explicit OPCode92(int& addr)
          : Instruction(addr, "0x92", 0x92) { }
        OPCode92(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x92", 0x92, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode93 : public Instruction {
      public:
        explicit OPCode93(int& addr)
          : Instruction(addr, "0x93", 0x93) { }
        OPCode93(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x93", 0x93, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCode94 : public Instruction {
      public:
        explicit OPCode94(int& addr)
          : Instruction(addr, "0x94", 0x94) { }
        OPCode94(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x94", 0x94, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCode95 : public Instruction {
      public:
        explicit OPCode95(int& addr)
          : Instruction(addr, "0x95", 0x95) { }
        OPCode95(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x95", 0x95, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode97 : public Instruction {
      public:
        explicit OPCode97(int& addr)
          : Instruction(addr, "0x97", 0x97) { }
        OPCode97(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x97", 0x97, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode99 : public Instruction {
      public:
        explicit OPCode99(int& addr)
          : Instruction(addr, "0x99", 0x99) { }
        OPCode99(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x99", 0x99, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode9A : public Instruction {
      public:
        explicit OPCode9A(int& addr)
          : Instruction(addr, "0x9A", 0x9A) { }
        OPCode9A(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x9A", 0x9A, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode9B : public Instruction {
      public:
        explicit OPCode9B(int& addr)
          : Instruction(addr, "0x9B", 0x9B) { }
        OPCode9B(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x9B", 0x9B, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode9C : public Instruction {
      public:
        explicit OPCode9C(int& addr)
          : Instruction(addr, "0x9C", 0x9C) { }
        OPCode9C(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x9C", 0x9C, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x00:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x02:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCode9D : public Instruction {
      public:
        explicit OPCode9D(int& addr)
          : Instruction(addr, "0x9D", 0x9D) { }
        OPCode9D(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x9D", 0x9D, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode9E : public Instruction {
      public:
        explicit OPCode9E(int& addr)
          : Instruction(addr, "0x9E", 0x9E) { }
        OPCode9E(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0x9E", 0x9E, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0xF:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x11:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x12:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCodeA0 : public Instruction {
      public:
        explicit OPCodeA0(int& addr)
          : Instruction(addr, "0xA0", 0xA0) { }
        OPCodeA0(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA0", 0xA0, Maker) {
            addr++;
        }
    };
    class OPCodeA1 : public Instruction {
      public:
        explicit OPCodeA1(int& addr)
          : Instruction(addr, "0xA1", 0xA1) { }
        OPCodeA1(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA1", 0xA1, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA3 : public Instruction {
      public:
        explicit OPCodeA3(int& addr)
          : Instruction(addr, "0xA3", 0xA3) { }
        OPCodeA3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA3", 0xA3, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA4 : public Instruction {
      public:
        explicit OPCodeA4(int& addr)
          : Instruction(addr, "0xA4", 0xA4) { }
        OPCodeA4(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA4", 0xA4, Maker) {
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
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2))); // x21
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA6 : public Instruction {
      public:
        explicit OPCodeA6(int& addr)
          : Instruction(addr, "0xA6", 0xA6) { }
        OPCodeA6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA6", 0xA6, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA8 : public Instruction {
      public:
        explicit OPCodeA8(int& addr)
          : Instruction(addr, "0xA8", 0xA8) { }
        OPCodeA8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA8", 0xA8, Maker) {
            addr++;
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA9 : public Instruction {
      public:
        explicit OPCodeA9(int& addr)
          : Instruction(addr, "0xA9", 0xA9) { }
        OPCodeA9(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xA9", 0xA9, Maker) {
            addr++;
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeAA : public Instruction {
      public:
        explicit OPCodeAA(int& addr)
          : Instruction(addr, "0xAA", 0xAA) { }
        OPCodeAA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAA", 0xAA, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeAB : public Instruction {
      public:
        explicit OPCodeAB(int& addr)
          : Instruction(addr, "0xAB", 0xAB) { }
        OPCodeAB(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAB", 0xAB, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    for (int i = 4; i <= 0x36; i++) {
                        this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    }
                    break;

                case 0x2:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    for (int i = 3; i <= 0x65; i += 2) {
                        this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;
                case 0x3:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeAC : public Instruction {
      public:
        explicit OPCodeAC(int& addr)
          : Instruction(addr, "0xAC", 0xAC) { }
        OPCodeAC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAC", 0xAC, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                case 0x1:

                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x3:
                case 0x4:
                    fun_1403c90e0(addr, content, this, 1);

                    break;
                case 0x5:
                case '\a':
                case '\t':

                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCodeAD : public Instruction {
      public:
        explicit OPCodeAD(int& addr)
          : Instruction(addr, "0xAD", 0xAD) { }
        OPCodeAD(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAD", 0xAD, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x00:
                case 0x01:
                case 0x03:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x02:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeAE : public Instruction {
      public:
        explicit OPCodeAE(int& addr)
          : Instruction(addr, "0xAE", 0xAE) { }
        OPCodeAE(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAE", 0xAE, Maker) {
            addr++;

            this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeAF : public Instruction {
      public:
        explicit OPCodeAF(int& addr)
          : Instruction(addr, "0xAF", 0xAF) { }
        OPCodeAF(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xAF", 0xAF, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCodeB1 : public Instruction {
      public:
        explicit OPCodeB1(int& addr)
          : Instruction(addr, "0xB1", 0xB1) { }
        OPCodeB1(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB1", 0xB1, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeB2 : public Instruction {
      public:
        explicit OPCodeB2(int& addr)
          : Instruction(addr, "0xB2", 0xB2) { }
        OPCodeB2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB2", 0xB2, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeB3 : public Instruction {
      public:
        explicit OPCodeB3(int& addr)
          : Instruction(addr, "0xB3", 0xB3) { }
        OPCodeB3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB3", 0xB3, Maker) {
            addr++;
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeB4 : public Instruction {
      public:
        explicit OPCodeB4(int& addr)
          : Instruction(addr, "0xB4", 0xB4) { }
        OPCodeB4(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB4", 0xB4, Maker) {
            addr++;
        }
    };
    class OPCodeB5 : public Instruction {
      public:
        explicit OPCodeB5(int& addr)
          : Instruction(addr, "0xB5", 0xB5) { }
        OPCodeB5(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB5", 0xB5, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeB6 : public Instruction {
      public:
        explicit OPCodeB6(int& addr)
          : Instruction(addr, "0xB6", 0xB6) { }
        OPCodeB6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB6", 0xB6, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeB7 : public Instruction {
      public:
        explicit OPCodeB7(int& addr)
          : Instruction(addr, "0xB7", 0xB7) { }
        OPCodeB7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB7", 0xB7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeBA : public Instruction {
      public:
        explicit OPCodeBA(int& addr)
          : Instruction(addr, "0xBA", 0xBA) { }
        OPCodeBA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xBA", 0xBA, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x14:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCodeB8 : public Instruction {
      public:
        explicit OPCodeB8(int& addr)
          : Instruction(addr, "0xB8", 0xB8) { }
        OPCodeB8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB8", 0xB8, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            fun_1403c90e0(addr, content, this, 1);
            fun_1403c90e0(addr, content, this, 1);
        }
    };
    class OPCodeB9 : public Instruction {
      public:
        explicit OPCodeB9(int& addr)
          : Instruction(addr, "0xB9", 0xB9) { }
        OPCodeB9(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xB9", 0xB9, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            switch ((unsigned char)control_byte[0]) {
                case 0:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 1:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeBB : public Instruction {
      public:
        explicit OPCodeBB(int& addr)
          : Instruction(addr, "0xBB", 0xBB) { }
        OPCodeBB(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xBB", 0xBB, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeBC : public Instruction {
      public:
        explicit OPCodeBC(int& addr)
          : Instruction(addr, "0xBC", 0xBC) { }
        OPCodeBC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xBC", 0xBC, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            fun_1403c90e0(addr, content, this, 1);
            switch ((unsigned char)control_byte[0]) {
                case 0:
                case 1:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                    break;
                case 2:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                case 5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 7:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 8:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 9:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 10:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xb:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xc:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xd:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xe:
                    fun_1403c90e0(addr, content, this, 1);
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0xf:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x10:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x11:
                case 0x12:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeBE : public Instruction {
      public:
        explicit OPCodeBE(int& addr)
          : Instruction(addr, "0xBE", 0xBE) { }
        OPCodeBE(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xBE", 0xBE, Maker) {
            addr++;
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeC0 : public Instruction {
      public:
        explicit OPCodeC0(int& addr)
          : Instruction(addr, "0xC0", 0xC0) { }
        OPCodeC0(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC0", 0xC0, Maker) {
            addr++;
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            this->add_operande(Operande(addr, "short", control_short));
            switch (control_shrt) {
                case 0x2:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x3:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x4:
                    this->add_operande(Operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                default:
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeC2 : public Instruction {
      public:
        explicit OPCodeC2(int& addr)
          : Instruction(addr, "0xC2", 0xC2) { }
        OPCodeC2(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC2", 0xC2, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCodeC3 : public Instruction {
      public:
        explicit OPCodeC3(int& addr)
          : Instruction(addr, "0xC3", 0xC3) { }
        OPCodeC3(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC3", 0xC3, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x1:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCodeC4 : public Instruction {
      public:
        explicit OPCodeC4(int& addr)
          : Instruction(addr, "0xC4", 0xC4) { }
        OPCodeC4(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC4", 0xC4, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:

                    break;
                case 0x1:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x3:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x4:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    fun_1403c90e0(addr, content, this, 0);

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->add_operande(Operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                    break;
                case 0x5:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCodeC5 : public Instruction {
      public:
        explicit OPCodeC5(int& addr)
          : Instruction(addr, "0xC5", 0xC5) { }
        OPCodeC5(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC5", 0xC5, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };
    class OPCodeC6 : public Instruction {
      public:
        explicit OPCodeC6(int& addr)
          : Instruction(addr, "0xC6", 0xC6) { }
        OPCodeC6(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC6", 0xC6, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                case 0x3:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x4:
                case 0x5:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x6:
                case 0x7:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCodeC7 : public Instruction {
      public:
        explicit OPCodeC7(int& addr)
          : Instruction(addr, "0xC7", 0xC7) { }
        OPCodeC7(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC7", 0xC7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

                case 0x1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeC8 : public Instruction {
      public:
        explicit OPCodeC8(int& addr)
          : Instruction(addr, "0xC8", 0xC8) { }
        OPCodeC8(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC8", 0xC8, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeC9 : public Instruction {
      public:
        explicit OPCodeC9(int& addr)
          : Instruction(addr, "0xC9", 0xC9) { }
        OPCodeC9(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xC9", 0xC9, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                case 0x1:
                    this->add_operande(Operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
        }
    };
    class OPCodeCA : public Instruction {
      public:
        explicit OPCodeCA(int& addr)
          : Instruction(addr, "0xCA", 0xCA) { }
        OPCodeCA(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xCA", 0xCA, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                    this->add_operande(Operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
            }
        }
    };
    class OPCodeCB : public Instruction {
      public:
        explicit OPCodeCB(int& addr)
          : Instruction(addr, "0xCB", 0xCB) { }
        OPCodeCB(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xCB", 0xCB, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCodeCC : public Instruction {
      public:
        explicit OPCodeCC(int& addr)
          : Instruction(addr, "0xCC", 0xCC) { }
        OPCodeCC(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xCC", 0xCC, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
        }
    };
    class OPCodeCD : public Instruction {
      public:
        explicit OPCodeCD(int& addr)
          : Instruction(addr, "0xCD", 0xCD) { }
        OPCodeCD(int& addr, ssd::Buffer& content, Builder* Maker)
          : Instruction(addr, "0xCD", 0xCD, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->add_operande(Operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                    this->add_operande(Operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }
        }
    };

    std::shared_ptr<Instruction> create_instruction_from_dat(int& addr, ssd::Buffer& dat_content, int function_type) override {
        int OP = (dat_content[addr] & 0xFF);

        if (function_type == 0) { // the function is read with OPCodes

            switch (OP) {
                case 0x00:
                    return std::make_shared<OPCode0>(addr, dat_content, this);
                case 0x01:
                    return std::make_shared<OPCode1>(addr, dat_content, this);
                case 0x02:
                    return std::make_shared<OPCode2>(addr, dat_content, this);
                case 0x03:
                    return std::make_shared<OPCode3>(addr, dat_content, this);
                case 0x04:
                    return std::make_shared<OPCode4>(addr, dat_content, this);
                case 0x05:
                    return std::make_shared<OPCode5>(addr, dat_content, this);
                case 0x06:
                    return std::make_shared<OPCode6>(addr, dat_content, this);
                case 0x07:
                    return std::make_shared<OPCode7>(addr, dat_content, this);
                case 0x08:
                    return std::make_shared<OPCode8>(addr, dat_content, this);
                case 0x0A:
                    return std::make_shared<OPCodeA>(addr, dat_content, this);
                case 0x0C:
                    return std::make_shared<OPCodeC>(addr, dat_content, this);
                case 0x0D:
                    return std::make_shared<OPCode0D>(addr, dat_content, this);
                case 0x0E:
                    return std::make_shared<OPCodeE>(addr, dat_content, this);
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
                case 0x1A:
                    return std::make_shared<OPCode1A>(addr, dat_content, this);
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
                case 0x4E:
                    return std::make_shared<OPCode4E>(addr, dat_content, this);
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
                case 0x54:
                    return std::make_shared<OPCode54>(addr, dat_content, this);
                case 0x55:
                    return std::make_shared<OPCode55>(addr, dat_content, this);
                case 0x56:
                    return std::make_shared<OPCode56>(addr, dat_content, this);
                case 0x57:
                    return std::make_shared<OPCode57>(addr, dat_content, this);
                case 0x58:
                    return std::make_shared<OPCode58>(addr, dat_content, this);
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
                case 0x6E:
                    return std::make_shared<OPCode6E>(addr, dat_content, this);
                case 0x6F:
                    return std::make_shared<OPCode6F>(addr, dat_content, this);
                case 0x70:
                    return std::make_shared<OPCode70>(addr, dat_content, this);
                case 0x72:
                    return std::make_shared<OPCode72>(addr, dat_content, this);
                case 0x73:
                    return std::make_shared<OPCode73>(addr, dat_content, this);
                case 0x74:
                    return std::make_shared<OPCode74>(addr, dat_content, this);
                case 0x75:
                    return std::make_shared<OPCode75>(addr, dat_content, this);
                case 0x76:
                    return std::make_shared<OPCode76>(addr, dat_content, this);
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
                case 0x7C:
                    return std::make_shared<OPCode7C>(addr, dat_content, this);
                case 0x7D:
                    return std::make_shared<OPCode7D>(addr, dat_content, this);
                case 0x7E:
                    return std::make_shared<OPCode7E>(addr, dat_content, this);
                case 0x80:
                    return std::make_shared<OPCode80>(addr, dat_content, this);
                case 0x82:
                    return std::make_shared<OPCode82>(addr, dat_content, this);
                case 0x83:
                    return std::make_shared<OPCode83>(addr, dat_content, this);
                case 0x84:
                    return std::make_shared<OPCode84>(addr, dat_content, this);
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
                case 0x8F:
                    return std::make_shared<OPCode8F>(addr, dat_content, this);
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
                case 0x9C:
                    return std::make_shared<OPCode9C>(addr, dat_content, this);
                case 0x9D:
                    return std::make_shared<OPCode9D>(addr, dat_content, this);
                case 0x9E:
                    return std::make_shared<OPCode9E>(addr, dat_content, this);
                case 0xA0:
                    return std::make_shared<OPCodeA0>(addr, dat_content, this);
                case 0xA1:
                    return std::make_shared<OPCodeA1>(addr, dat_content, this);
                case 0xA3:
                    return std::make_shared<OPCodeA3>(addr, dat_content, this);
                case 0xA4:
                    return std::make_shared<OPCodeA4>(addr, dat_content, this);
                case 0xA6:
                    return std::make_shared<OPCodeA6>(addr, dat_content, this);
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
                case 0xAE:
                    return std::make_shared<OPCodeAE>(addr, dat_content, this);
                case 0xAF:
                    return std::make_shared<OPCodeAF>(addr, dat_content, this);
                case 0xB1:
                    return std::make_shared<OPCodeB1>(addr, dat_content, this);
                case 0xB2:
                    return std::make_shared<OPCodeB2>(addr, dat_content, this);
                case 0xB3:
                    return std::make_shared<OPCodeB3>(addr, dat_content, this);
                case 0xB4:
                    return std::make_shared<OPCodeB4>(addr, dat_content, this);
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
                case 0xC0:
                    return std::make_shared<OPCodeC0>(addr, dat_content, this);
                case 0xC2:
                    return std::make_shared<OPCodeC2>(addr, dat_content, this);
                case 0xC3:
                    return std::make_shared<OPCodeC3>(addr, dat_content, this);
                case 0xC4:
                    return std::make_shared<OPCodeC4>(addr, dat_content, this);
                case 0xC5:
                    return std::make_shared<OPCodeC5>(addr, dat_content, this);
                case 0xC6:
                    return std::make_shared<OPCodeC6>(addr, dat_content, this);
                case 0xC7:
                    return std::make_shared<OPCodeC7>(addr, dat_content, this);
                case 0xC8:
                    return std::make_shared<OPCodeC8>(addr, dat_content, this);
                case 0xC9:
                    return std::make_shared<OPCodeC9>(addr, dat_content, this);
                case 0xCA:
                    return std::make_shared<OPCodeCA>(addr, dat_content, this);
                case 0xCB:
                    return std::make_shared<OPCodeCB>(addr, dat_content, this);
                case 0xCC:
                    return std::make_shared<OPCodeCC>(addr, dat_content, this);
                case 0xCD:
                    return std::make_shared<OPCodeCD>(addr, dat_content, this);

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
    std::shared_ptr<Instruction> create_instruction_from_xlsx(int& addr, int opcode) override {
        switch (opcode) {
            case 0x00:
                return std::make_shared<OPCode0>(addr);
            case 0x01:
                return std::make_shared<OPCode1>(addr);
            case 0x02:
                return std::make_shared<OPCode2>(addr);
            case 0x03:
                return std::make_shared<OPCode3>(addr);
            case 0x04:
                return std::make_shared<OPCode4>(addr);
            case 0x05:
                return std::make_shared<OPCode5>(addr);
            case 0x06:
                return std::make_shared<OPCode6>(addr);
            case 0x07:
                return std::make_shared<OPCode7>(addr);
            case 0x08:
                return std::make_shared<OPCode8>(addr);
            case 0x0A:
                return std::make_shared<OPCodeA>(addr);
            case 0x0C:
                return std::make_shared<OPCodeC>(addr);
            case 0x0D:
                return std::make_shared<OPCode0D>(addr);
            case 0x0E:
                return std::make_shared<OPCodeE>(addr);
            case 0x10:
                return std::make_shared<OPCode10>(addr);
            case 0x11:
                return std::make_shared<OPCode11>(addr);
            case 0x12:
                return std::make_shared<OPCode12>(addr);
            case 0x13:
                return std::make_shared<OPCode13>(addr);
            case 0x14:
                return std::make_shared<OPCode14>(addr);
            case 0x15:
                return std::make_shared<OPCode15>(addr);
            case 0x16:
                return std::make_shared<OPCode16>(addr);
            case 0x17:
                return std::make_shared<OPCode17>(addr);
            case 0x18:
                return std::make_shared<OPCode18>(addr);
            case 0x1A:
                return std::make_shared<OPCode1A>(addr);
            case 0x1D:
                return std::make_shared<OPCode1D>(addr);
            case 0x1E:
                return std::make_shared<OPCode1E>(addr);
            case 0x1F:
                return std::make_shared<OPCode1F>(addr);
            case 0x20:
                return std::make_shared<OPCode20>(addr);
            case 0x21:
                return std::make_shared<OPCode21>(addr);
            case 0x22:
                return std::make_shared<OPCode22>(addr);
            case 0x23:
                return std::make_shared<OPCode23>(addr);
            case 0x24:
                return std::make_shared<OPCode24>(addr);
            case 0x25:
                return std::make_shared<OPCode25>(addr);
            case 0x26:
                return std::make_shared<OPCode26>(addr);
            case 0x27:
                return std::make_shared<OPCode27>(addr);
            case 0x28:
                return std::make_shared<OPCode28>(addr);
            case 0x29:
                return std::make_shared<OPCode29>(addr);
            case 0x2A:
                return std::make_shared<OPCode2A>(addr);
            case 0x2B:
                return std::make_shared<OPCode2B>(addr);
            case 0x2C:
                return std::make_shared<OPCode2C>(addr);
            case 0x2D:
                return std::make_shared<OPCode2D>(addr);
            case 0x2E:
                return std::make_shared<OPCode2E>(addr);
            case 0x2F:
                return std::make_shared<OPCode2F>(addr);
            case 0x30:
                return std::make_shared<OPCode30>(addr);
            case 0x31:
                return std::make_shared<OPCode31>(addr);
            case 0x32:
                return std::make_shared<OPCode32>(addr);
            case 0x33:
                return std::make_shared<OPCode33>(addr);
            case 0x34:
                return std::make_shared<OPCode34>(addr);
            case 0x35:
                return std::make_shared<OPCode35>(addr);
            case 0x36:
                return std::make_shared<OPCode36>(addr);
            case 0x37:
                return std::make_shared<OPCode37>(addr);
            case 0x38:
                return std::make_shared<OPCode38>(addr);
            case 0x39:
                return std::make_shared<OPCode39>(addr);
            case 0x3A:
                return std::make_shared<OPCode3A>(addr);
            case 0x3B:
                return std::make_shared<OPCode3B>(addr);
            case 0x3C:
                return std::make_shared<OPCode3C>(addr);
            case 0x3D:
                return std::make_shared<OPCode3D>(addr);
            case 0x3E:
                return std::make_shared<OPCode3E>(addr);
            case 0x3F:
                return std::make_shared<OPCode3F>(addr);
            case 0x40:
                return std::make_shared<OPCode40>(addr);
            case 0x41:
                return std::make_shared<OPCode41>(addr);
            case 0x42:
                return std::make_shared<OPCode42>(addr);
            case 0x43:
                return std::make_shared<OPCode43>(addr);
            case 0x44:
                return std::make_shared<OPCode44>(addr);
            case 0x45:
                return std::make_shared<OPCode45>(addr);
            case 0x46:
                return std::make_shared<OPCode46>(addr);
            case 0x47:
                return std::make_shared<OPCode47>(addr);
            case 0x48:
                return std::make_shared<OPCode48>(addr);
            case 0x49:
                return std::make_shared<OPCode49>(addr);
            case 0x4A:
                return std::make_shared<OPCode4A>(addr);
            case 0x4B:
                return std::make_shared<OPCode4B>(addr);
            case 0x4C:
                return std::make_shared<OPCode4C>(addr);
            case 0x4D:
                return std::make_shared<OPCode4D>(addr);
            case 0x4E:
                return std::make_shared<OPCode4E>(addr);
            case 0x4F:
                return std::make_shared<OPCode4F>(addr);
            case 0x50:
                return std::make_shared<OPCode50>(addr);
            case 0x51:
                return std::make_shared<OPCode51>(addr);
            case 0x52:
                return std::make_shared<OPCode52>(addr);
            case 0x53:
                return std::make_shared<OPCode53>(addr);
            case 0x54:
                return std::make_shared<OPCode54>(addr);
            case 0x55:
                return std::make_shared<OPCode55>(addr);
            case 0x56:
                return std::make_shared<OPCode56>(addr);
            case 0x57:
                return std::make_shared<OPCode57>(addr);
            case 0x58:
                return std::make_shared<OPCode58>(addr);
            case 0x5A:
                return std::make_shared<OPCode5A>(addr);
            case 0x5B:
                return std::make_shared<OPCode5B>(addr);
            case 0x5C:
                return std::make_shared<OPCode5C>(addr);
            case 0x5D:
                return std::make_shared<OPCode5D>(addr);
            case 0x5E:
                return std::make_shared<OPCode5E>(addr);
            case 0x60:
                return std::make_shared<OPCode60>(addr);
            case 0x61:
                return std::make_shared<OPCode61>(addr);
            case 0x62:
                return std::make_shared<OPCode62>(addr);
            case 0x63:
                return std::make_shared<OPCode63>(addr);
            case 0x64:
                return std::make_shared<OPCode64>(addr);
            case 0x65:
                return std::make_shared<OPCode65>(addr);
            case 0x66:
                return std::make_shared<OPCode66>(addr);
            case 0x67:
                return std::make_shared<OPCode67>(addr);
            case 0x68:
                return std::make_shared<OPCode68>(addr);
            case 0x69:
                return std::make_shared<OPCode69>(addr);
            case 0x6A:
                return std::make_shared<OPCode6A>(addr);
            case 0x6B:
                return std::make_shared<OPCode6B>(addr);
            case 0x6C:
                return std::make_shared<OPCode6C>(addr);
            case 0x6E:
                return std::make_shared<OPCode6E>(addr);
            case 0x6F:
                return std::make_shared<OPCode6F>(addr);
            case 0x70:
                return std::make_shared<OPCode70>(addr);
            case 0x72:
                return std::make_shared<OPCode72>(addr);
            case 0x73:
                return std::make_shared<OPCode74>(addr);
            case 0x74:
                return std::make_shared<OPCode74>(addr);
            case 0x75:
                return std::make_shared<OPCode75>(addr);
            case 0x76:
                return std::make_shared<OPCode76>(addr);
            case 0x77:
                return std::make_shared<OPCode77>(addr);
            case 0x78:
                return std::make_shared<OPCode78>(addr);
            case 0x79:
                return std::make_shared<OPCode79>(addr);
            case 0x7A:
                return std::make_shared<OPCode7A>(addr);
            case 0x7B:
                return std::make_shared<OPCode7B>(addr);
            case 0x7C:
                return std::make_shared<OPCode7C>(addr);
            case 0x7D:
                return std::make_shared<OPCode7D>(addr);
            case 0x7E:
                return std::make_shared<OPCode7E>(addr);
            case 0x80:
                return std::make_shared<OPCode80>(addr);
            case 0x82:
                return std::make_shared<OPCode82>(addr);
            case 0x83:
                return std::make_shared<OPCode83>(addr);
            case 0x84:
                return std::make_shared<OPCode84>(addr);
            case 0x86:
                return std::make_shared<OPCode86>(addr);
            case 0x87:
                return std::make_shared<OPCode87>(addr);
            case 0x88:
                return std::make_shared<OPCode88>(addr);
            case 0x89:
                return std::make_shared<OPCode89>(addr);
            case 0x8A:
                return std::make_shared<OPCode8A>(addr);
            case 0x8B:
                return std::make_shared<OPCode8B>(addr);
            case 0x8C:
                return std::make_shared<OPCode8C>(addr);
            case 0x8D:
                return std::make_shared<OPCode8D>(addr);
            case 0x8E:
                return std::make_shared<OPCode8E>(addr);
            case 0x8F:
                return std::make_shared<OPCode8F>(addr);
            case 0x90:
                return std::make_shared<OPCode90>(addr);
            case 0x91:
                return std::make_shared<OPCode91>(addr);
            case 0x92:
                return std::make_shared<OPCode92>(addr);
            case 0x93:
                return std::make_shared<OPCode93>(addr);
            case 0x94:
                return std::make_shared<OPCode94>(addr);
            case 0x95:
                return std::make_shared<OPCode95>(addr);
            case 0x97:
                return std::make_shared<OPCode97>(addr);
            case 0x98:
                return std::make_shared<OPCode98>(addr);
            case 0x99:
                return std::make_shared<OPCode99>(addr);
            case 0x9A:
                return std::make_shared<OPCode9A>(addr);
            case 0x9B:
                return std::make_shared<OPCode9B>(addr);
            case 0x9C:
                return std::make_shared<OPCode9C>(addr);
            case 0x9D:
                return std::make_shared<OPCode9D>(addr);
            case 0x9E:
                return std::make_shared<OPCode9E>(addr);
            case 0xA0:
                return std::make_shared<OPCodeA0>(addr);
            case 0xA1:
                return std::make_shared<OPCodeA1>(addr);
            case 0xA3:
                return std::make_shared<OPCodeA3>(addr);
            case 0xA4:
                return std::make_shared<OPCodeA4>(addr);
            case 0xA6:
                return std::make_shared<OPCodeA6>(addr);
            case 0xA8:
                return std::make_shared<OPCodeA8>(addr);
            case 0xA9:
                return std::make_shared<OPCodeA9>(addr);
            case 0xAA:
                return std::make_shared<OPCodeAA>(addr);
            case 0xAB:
                return std::make_shared<OPCodeAB>(addr);
            case 0xAC:
                return std::make_shared<OPCodeAC>(addr);
            case 0xAD:
                return std::make_shared<OPCodeAD>(addr);
            case 0xAE:
                return std::make_shared<OPCodeAE>(addr);
            case 0xAF:
                return std::make_shared<OPCodeAF>(addr);
            case 0xB1:
                return std::make_shared<OPCodeB1>(addr);
            case 0xB2:
                return std::make_shared<OPCodeB2>(addr);
            case 0xB3:
                return std::make_shared<OPCodeB3>(addr);
            case 0xB4:
                return std::make_shared<OPCodeB4>(addr);
            case 0xB5:
                return std::make_shared<OPCodeB5>(addr);
            case 0xB6:
                return std::make_shared<OPCodeB6>(addr);
            case 0xB7:
                return std::make_shared<OPCodeB7>(addr);
            case 0xB8:
                return std::make_shared<OPCodeB8>(addr);
            case 0xB9:
                return std::make_shared<OPCodeB9>(addr);
            case 0xBA:
                return std::make_shared<OPCodeBA>(addr);
            case 0xBB:
                return std::make_shared<OPCodeBB>(addr);
            case 0xBC:
                return std::make_shared<OPCodeBC>(addr);
            case 0xBE:
                return std::make_shared<OPCodeBE>(addr);
            case 0xC0:
                return std::make_shared<OPCodeC0>(addr);
            case 0xC2:
                return std::make_shared<OPCodeC2>(addr);
            case 0xC3:
                return std::make_shared<OPCodeC3>(addr);
            case 0xC4:
                return std::make_shared<OPCodeC4>(addr);
            case 0xC5:
                return std::make_shared<OPCodeC5>(addr);
            case 0xC6:
                return std::make_shared<OPCodeC6>(addr);
            case 0xC7:
                return std::make_shared<OPCodeC7>(addr);
            case 0xC8:
                return std::make_shared<OPCodeC8>(addr);
            case 0xC9:
                return std::make_shared<OPCodeC9>(addr);
            case 0xCA:
                return std::make_shared<OPCodeCA>(addr);
            case 0xCB:
                return std::make_shared<OPCodeCB>(addr);
            case 0xCC:
                return std::make_shared<OPCodeCC>(addr);
            case 0xCD:
                return std::make_shared<OPCodeCD>(addr);

            case 256:
                return std::make_shared<CreateMonsters>(addr);
            case 257:
                return std::make_shared<EffectsInstr>(addr);
            case 258:
                return std::make_shared<ActionTable>(addr);
            case 259:
                return std::make_shared<AlgoTable>(addr);
            case 260:
                return std::make_shared<WeaponAttTable>(addr);
            case 261:
                return std::make_shared<BreakTable>(addr);
            case 262:
                return std::make_shared<SummonTable>(addr);
            case 263:
                return std::make_shared<ReactionTable>(addr);
            case 264:
                return std::make_shared<PartTable>(addr);
            case 265:
                return std::make_shared<AnimeClipTable>(addr);
            case 266:
                return std::make_shared<FieldMonsterData>(addr);
            case 267:
                return std::make_shared<FieldFollowData>(addr);
            case 268:
                return std::make_shared<FC_autoX>(addr);
            case 269:
                return std::make_shared<BookData99>(addr);
            case 270:
                return std::make_shared<BookDataX>(addr);
            case 273:
                return std::make_shared<AnimeClipData>(addr);
            default:
                return nullptr;
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

#endif // CS3INSTRUCTIONSSET_H
