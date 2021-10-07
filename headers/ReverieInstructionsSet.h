#ifndef REVERIEINSTRUCTIONSSET_H
#define REVERIEINSTRUCTIONSSET_H
#include "headers/functions.h"
#include "headers/utilities.h"

class ReverieBuilder : public Builder {
public:
    ReverieBuilder() = default;
    int add_operandes(int& addr, ssd::Buffer& content, Instruction* instr, int size) {
        int plVar15 = addr;
        int lVar5 = 0;
        int lVar7 = -1;
        do {
            instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            lVar5 = lVar7 + size;
            lVar7 = lVar7 + 1;
        } while (content[plVar15 + lVar5] != '\0');
        return lVar7;
    }

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
                instr->AddOperande(operande(addr, "byte", current_op_value));
                addr++;

                return;
            case 0x01:

                if (start_text) {
                    current_op_value.push_back(current_byte);
                } else {

                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr, "byte", current_op_value));
                    current_op_value.clear();
                }
                addr++;
                break;
            case 0x02:
                start_text = false;
                if (current_op_value.size() > 0) {
                    instr->AddOperande(operande(addr_, "dialog", current_op_value));
                }
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr, "byte", current_op_value));
                current_op_value.clear();
                addr++;
                break;
            case 0x10:
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr, "byte", current_op_value));
                current_op_value.clear();
                addr++;
                instr->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x17:
            case 0x19:
                start_text = false;
                if (current_op_value.size() > 0) {
                    instr->AddOperande(operande(addr_, "dialog", current_op_value));
                }
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr, "byte", current_op_value));
                current_op_value.clear();
                addr++;
                instr->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x11:
            case 0x12:
                if (current_op_value.size() > 0) {
                    instr->AddOperande(operande(addr_, "dialog", current_op_value));
                }
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr, "byte", current_op_value));
                current_op_value.clear();
                addr++;
                instr->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
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
                        break;
                    }

                } else if ((((current_byte + 0xb7) & 0xdf) == 0) || (current_byte == 0x50) || (current_byte == 0x54) ||
                           (current_byte == 0x57) || (current_byte == 0x53) || (current_byte == 0x73) || (current_byte == 0x43) ||
                           (current_byte == 99) || (current_byte == 0x78) || (current_byte == 0x79) || (current_byte == 0x47) ||
                           (current_byte == 0x44) || (current_byte == 0x55) || (current_byte == 0x52)) {
                    current_op_value.push_back(current_byte);
                    addr++;
                    current_byte = content[addr];
                    break;
                }
                break;
            default:
                if (current_byte < 0x20) {
                    if (current_op_value.size() > 0) {
                        instr->AddOperande(operande(addr_, "dialog", current_op_value));
                    }
                    start_text = false;
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr, "byte", current_op_value));
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
        instr->AddOperande(operande(addr, "byte", control_byte3_arr));
        auto control_byte3 = (unsigned char)control_byte3_arr[0];
        if (control_byte3 == '\x55') {
            instr->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
        if (control_byte3 == '\x11') {
            instr->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

        } else if (control_byte3 != '3') {

            if (control_byte3 == '\"') {
                instr->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                instr->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

            } else if (control_byte3 != 0x44) {

                if (control_byte3 == 0xDD) {

                    // there is a XOR,XOR EDI which causes a jump that ignores the strcpy
                    if (param != 0) instr->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    instr->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                } else if (control_byte3 == 0xFF) {
                    instr->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                } else {
                    if (control_byte3 != 0xEE) {

                    } else {
                        instr->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    }
                }

            } else {
                instr->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                if (param != 0) instr->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            }

        } else {
            instr->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    }
    static void sub05(int& addr, ssd::Buffer& content, Instruction* instr) {

        ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
        instr->AddOperande(operande(addr, "byte", control_byte));

        while ((int)control_byte[0] != 1) {
            if (addr > std::ssize(content)) throw ssd::exceptions::unspecified_recoverable();
            switch ((unsigned char)control_byte[0]) {

            case 0x0:
            case 0x24:
                instr->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x1c: {
                // the next byte is the OP code for a new instruction

                std::shared_ptr<Instruction> instr2 = instr->Maker->CreateInstructionFromDAT(addr, content, 0);

                operande op = operande(addr, "instruction", instr2->getBytes());
                instr->AddOperande(op);

                break;
            }
            case 0x1e:
                instr->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1f:
            case 0x20:
            case 0x23:

                instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x21:
                instr->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                instr->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x25:
                instr->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            default:;
            }

            control_byte = ReadSubByteArray(content, addr, 1);

            instr->AddOperande(operande(addr, "byte", control_byte));
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
            if (Maker->goal < addr + 0x60) {
                throw ssd::exceptions::unexpected_operand();
                return;
            }
            ssd::Buffer map = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", map));

            ssd::Buffer remaining1 = ReadSubByteArray(content, addr, 0x10 - map.size());
            operande fill1 = operande(addr, "fill", remaining1);
            fill1.setBytesToFill(0x10);
            this->AddOperande(fill1);
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RSI + 0x28
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4))); // 0x2C

            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x30
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x32
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x34
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0x36

            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4))); // 0x38

            ssd::Buffer btl_name = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", btl_name));

            ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - btl_name.size());
            operande fill = operande(addr, "fill", remaining);
            fill.setBytesToFill(0x20);
            this->AddOperande(fill);


            ssd::Buffer array = ReadSubByteArray(content, addr, 4);
            this->AddOperande(operande(addr, "int", array));
            int first = ReadIntegerFromByteArray(0, array);

            int cnt = 0;
            while (first != -1)
            {
                int counter = 0;
                if (Maker->goal < initial_addr + 0x60 + cnt * (0x90)) {
                    throw ssd::exceptions::unexpected_operand();
                    return;
                }
                do {
                    counter++;
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->AddOperande(operande(addr, "string", monsters_name));

                    remaining = ReadSubByteArray(content, addr, 0x10 - monsters_name.size());
                    fill = operande(addr, "fill", remaining);
                    fill.setBytesToFill(0x10);
                    this->AddOperande(fill);

                } while (counter < 0x8);
                for (int idx_byte = 0; idx_byte < 8; idx_byte++) {
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
                if ((unsigned char)content[addr] == 0) {
                    this->AddOperande(operande(addr, "bytearray", ReadSubByteArray(content, addr, 8))); //??
                } else {
                    ssd::Buffer monsters_name = ReadStringSubByteArray(content, addr);
                    this->AddOperande(operande(addr, "string", monsters_name));

                    remaining = ReadSubByteArray(content, addr, 12 - monsters_name.size() - 1);
                    fill = operande(addr, "bytearray", remaining);
                    fill.setBytesToFill(12);
                    this->AddOperande(fill);
                }

                array = ReadSubByteArray(content, addr, 4);
                this->AddOperande(operande(addr, "int", array));
                first = ReadIntegerFromByteArray(0, array);
                cnt++;
            }

            this->AddOperande(operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x18))); //??

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
                if (Maker->goal < addr + 0x28) throw ssd::exceptions::unspecified_recoverable();

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));

                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());

                operande fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                current_byte = content[addr];
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
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            int cnt = 0;
            while (cnt < current_byte) {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                cnt++;
            }
        }
    };
    class ActionTable : public Instruction { //1401ae2c5
    public:
        ActionTable()
            : Instruction(-1, 258, nullptr) {}
        ActionTable(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "ActionTable", 258, Maker) {}
        ActionTable(int addr, Builder* Maker)
            : Instruction(addr, "ActionTable", 258, Maker) {}
        ActionTable(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "ActionTable", 258, Maker) {
            int cnt = 0;
            int16_t shrt = 0;
            do {

                shrt = ReadShortFromByteArray(addr, content);
                if (shrt == 1) break;
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                this->AddOperande(operande(addr, "short", short_bytes));

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));


                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - str.size());
                operande fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x10);
                this->AddOperande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                // The last string can be 0x20 long and the following 0x20 can terminate the string, I think
                // it's their purpose here
                str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x40 - str.size());
                fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x40);
                this->AddOperande(fill);

                cnt++;
            } while (cnt < 0x40);


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

            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                int16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->AddOperande(operande(addr, "short", short_bytes));

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                ssd::Buffer int_bytes2 = ReadSubByteArray(content, addr, 4);
                uint32_t int2 = ReadShortFromByteArray(0, int_bytes2);
                this->AddOperande(operande(addr, "int", int_bytes2));

                ssd::Buffer int_bytes3 = ReadSubByteArray(content, addr, 4);
                uint32_t int3 = ReadShortFromByteArray(0, int_bytes3);
                this->AddOperande(operande(addr, "int", int_bytes3));

                ssd::Buffer int_bytes4 = ReadSubByteArray(content, addr, 4);
                uint32_t int4 = ReadShortFromByteArray(0, int_bytes4);
                this->AddOperande(operande(addr, "int", int_bytes4));

                ssd::Buffer int_bytes5 = ReadSubByteArray(content, addr, 4);
                uint32_t int5 = ReadShortFromByteArray(0, int_bytes5);
                this->AddOperande(operande(addr, "int", int_bytes5));


                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                if ((shrt == -1)&&(int2 == 1) && (int3 == 2) && (int4 == 3) && (int5 == 4)) break;

                cnt++;
            } while (cnt < 0x40);

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

            this->AddOperande(operande(addr, "bytearray", ReadSubByteArray(content, addr, 4)));
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
                this->AddOperande(operande(addr, "short", short_bytes));
                if (shrt ==-1) break;
                ssd::Buffer short_bytes2 = ReadSubByteArray(content, addr, 2);

                this->AddOperande(operande(addr, "short", short_bytes2));

                cnt++;
            } while (cnt < 0x40);
            this->AddOperande(operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x2)));
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
            int cnt = 0;
            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                uint16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->AddOperande(operande(addr, "short", short_bytes));
                if (shrt == 0xFFFF) break;
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                operande fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);
                cnt++;
            } while (cnt < 0x40);
            this->AddOperande(operande(addr, "bytearray", ReadSubByteArray(content, addr, 0x24 - 2)));
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
            do {
                ssd::Buffer short_bytes = ReadSubByteArray(content, addr, 2);
                uint16_t shrt = ReadShortFromByteArray(0, short_bytes);
                this->AddOperande(operande(addr, "short", short_bytes));
                if (shrt == 0xFFFF) break;
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                cnt++;
            } while (cnt < 0x8);
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
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
            int cnt = 0;
            do {
                ssd::Buffer int_bytes = ReadSubByteArray(content, addr, 4);
                uint integer = ReadIntegerFromByteArray(0, int_bytes);
                this->AddOperande(operande(addr, "int", int_bytes));
                if (integer == 0x0000FFFF) break;

                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                operande fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
                fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                cnt++;
            } while (cnt < 0x4);
            ssd::Buffer str = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", str));
            ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
            operande fill = operande(addr, "fill", remaining);
            fill.setBytesToFill(0x20);
            this->AddOperande(fill);

            str = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", str));
            remaining = ReadSubByteArray(content, addr, 0x20 - str.size());
            fill = operande(addr, "fill", remaining);
            fill.setBytesToFill(0x20);
            this->AddOperande(fill);
        }
    };

    class AnimeClipTable
            : public Instruction // from CS4
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
            auto itt_current_fun = find_function_by_ID(Maker->FunctionsToParse, Maker->idx_current_fun);
            while (first_integer != 0) {
                itt_current_fun->AddInstruction(std::make_shared<AnimeClipData>(addr, content, Maker));
                first_integer = ReadIntegerFromByteArray(addr, content);
            }
            ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
            this->AddOperande(operande(addr, "int", first_integer_bytes));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class AnimeClipData : public Instruction {
    public:
        AnimeClipData()
            : Instruction(-1, 273, nullptr) {}
        AnimeClipData(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "AnimeClipData", 273, Maker) {}
        AnimeClipData(int addr, Builder* Maker)
            : Instruction(addr, "AnimeClipData", 273, Maker) {}
        AnimeClipData(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "AnimeClipData", 273, Maker) {

            ssd::Buffer first_integer_bytes = ReadSubByteArray(content, addr, 4);
            this->AddOperande(operande(addr, "int", first_integer_bytes));
            ssd::Buffer str = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", str));
            ssd::Buffer remaining = ReadSubByteArray(content, addr, (0x20) - str.size());
            operande fill = operande(addr, "fill", remaining);
            fill.setBytesToFill((0x20));
            this->AddOperande(fill);
            str = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", str));
            remaining = ReadSubByteArray(content, addr, (0x20) - str.size());
            fill = operande(addr, "fill", remaining);
            fill.setBytesToFill((0x20));
            this->AddOperande(fill);
        }
    };

    class FieldMonsterData
            : public Instruction
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


            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            int sentinel = ReadIntegerFromByteArray(addr, content);
            while (sentinel!=1){
                //There is nothing like this in the exe (I believe),
                //most of the FieldMonsterData should have 5 float,
                //but rob002 has only 4. it doesn't crash the game.Idk if the 5th floating point is accessed at all
                //To help readability, I prefer turning the 4 float FieldMonsterData into an actual FieldMonsterData function
                //instead of letting the tool fail his parsing and write the FieldMonsterData as an Op code funciton in the XLSX
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
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

            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
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
            this->AddOperande(operande(addr, "string", things));
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
            ssd::Buffer nb_books_byte = ReadSubByteArray(content, addr, 2);


            this->AddOperande(operande(addr, "short", nb_books_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
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
            this->AddOperande(operande(addr, "short", control_short));

            if (control > 0) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                ssd::Buffer title = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", title));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x10 - title.size());
                operande fill = operande(addr, "bytearray", remaining);
                fill.setBytesToFill(0x10);
                this->AddOperande(fill);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x12
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x14
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x16
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x18
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1A
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1C
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x1E
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x20
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x22
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // RCX+0x24
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            } else {
                if ((unsigned char)content[addr] != 1) {
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                }
            }
        }
    };
    class StyleName : public Instruction {
    public:
        StyleName()
            : Instruction(-1, 274, nullptr) {}
        StyleName(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "StyleName", 274, Maker) {}
        StyleName(int addr, Builder* Maker)
            : Instruction(addr, "StyleName", 274, Maker) {}
        StyleName(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "StyleName", 274, Maker) {

            unsigned char current_byte = content[addr];
            while (current_byte != 0x01) {

                ssd::Buffer str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr, "string", str));
                ssd::Buffer remaining = ReadSubByteArray(content, addr, 0x40 - str.size());
                operande fill = operande(addr, "fill", remaining);
                fill.setBytesToFill(0x40);
                this->AddOperande(fill);
                current_byte = content[addr];
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
    class OPCode2 : public Instruction {
    public:
        OPCode2()
            : Instruction(-1, 2, nullptr) {}
        OPCode2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "Call", 2, Maker) {}
        OPCode2(int addr, Builder* Maker)
            : Instruction(addr, "Call", 2, Maker) {}
        OPCode2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "Call", 2, Maker) {
            addr++;

            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            ssd::Buffer function_name = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr, "string", function_name));
            ssd::Buffer control_byte2_arr = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte2_arr));


            int control_byte2 = (uint8_t)control_byte2_arr[0];

            if (control_byte2 != 0) {
                for (int i = 0; i < control_byte2; i++) {
                    fun_1403c90e0(addr, content, this, 0);
                }
            }
        }
    };
    class OPCode3 : public Instruction {
    public:
        OPCode3()
            : Instruction(-1, 3, nullptr) {}
        OPCode3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 3, Maker) {}
        OPCode3(int addr, Builder* Maker)
            : Instruction(addr, "???", 3, Maker) {}
        OPCode3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode4 : public Instruction {
    public:
        OPCode4()
            : Instruction(-1, 4, nullptr) {}
        OPCode4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 4, Maker) {}
        OPCode4(int addr, Builder* Maker)
            : Instruction(addr, "???", 4, Maker) {}
        OPCode4(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 4, Maker) {
            addr++;
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode5 : public Instruction {
    public:
        OPCode5()
            : Instruction(-1, 5, nullptr) {}
        OPCode5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 5, Maker) {}
        OPCode5(int addr, Builder* Maker)
            : Instruction(addr, "???", 5, Maker) {}
        OPCode5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 5, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            sub05(addr, content, this);
            this->AddOperande(operande(addr, "pointer", ReadSubByteArray(content, addr, 4))); // pointer
        }
    };
    class OPCode6 : public Instruction {
    public:
        OPCode6()
            : Instruction(-1, 6, nullptr) {}
        OPCode6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 6, Maker) {}
        OPCode6(int addr, Builder* Maker)
            : Instruction(addr, "???", 6, Maker) {}
        OPCode6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            sub05(addr, content, this);

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);

            ssd::Buffer ptr;
            this->AddOperande(operande(addr, "byte", control_byte));

            for (int i = 0; i < (unsigned char)control_byte[0]; i++) {
                ssd::Buffer iVar3_arr = ReadSubByteArray(content, addr, 4);
                this->AddOperande(operande(addr, "int", iVar3_arr));

                ptr = ReadSubByteArray(content, addr, 4);
                this->AddOperande(operande(addr, "pointer", ptr));
            }

            this->AddOperande(operande(addr, "pointer", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode7 : public Instruction {
    public:
        OPCode7()
            : Instruction(-1, 0x7, nullptr) {}
        OPCode7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 7, Maker) {}
        OPCode7(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x7, Maker) {}
        OPCode7(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x7, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

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
            case 3:
                fun_1403c90e0(addr, content, this, 0);
                break;
            }
        }
    };
    class OPCode8 : public Instruction {
    public:
        OPCode8()
            : Instruction(-1, 8, nullptr) {}
        OPCode8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 8, Maker) {}
        OPCode8(int addr, Builder* Maker)
            : Instruction(addr, "???", 8, Maker) {}
        OPCode8(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 8, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
    class OPCode9 : public Instruction {
    public:
        OPCode9()
            : Instruction(-1, 9, nullptr) {}
        OPCode9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 9, Maker) {}
        OPCode9(int addr, Builder* Maker)
            : Instruction(addr, "???", 9, Maker) {}
        OPCode9(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 9, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA : public Instruction {
    public:
        OPCodeA()
            : Instruction(-1, 0xA, nullptr) {}
        OPCodeA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0xA, Maker) {}
        OPCodeA(int addr, Builder* Maker)
            : Instruction(addr, "???", 0xA, Maker) {}
        OPCodeA(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0xA, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            sub05(addr, content, this);
        }
    };
    class OPCodeB : public Instruction {
    public:
        OPCodeB()
            : Instruction(-1, 0xB, nullptr) {}
        OPCodeB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0xB, Maker) {}
        OPCodeB(int addr, Builder* Maker)
            : Instruction(addr, "???", 0xB, Maker) {}
        OPCodeB(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0xB, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            ssd::Buffer control_byte2;
            do {
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                switch ((unsigned char)control_byte2[0]) {
                case 1:
                    return;
                case 2:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                    break;
                case 0x13:

                    break;
                case 0x1C:

                    break;
                }
            } while (true);
        }
    };
    class OPCodeC : public Instruction {
    public:
        OPCodeC()
            : Instruction(-1, 0xC, nullptr) {}
        OPCodeC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0xC, Maker) {}
        OPCodeC(int addr, Builder* Maker)
            : Instruction(addr, "???", 0xC, Maker) {}
        OPCodeC(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0xC, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            switch ((unsigned char)control_byte[0]) {
            case 0:
                sub05(addr, content, this);
                break;
            }
        }
    };
    class OPCodeE : public Instruction {
    public:
        OPCodeE()
            : Instruction(-1, 0xE, nullptr) {}
        OPCodeE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0xE, Maker) {}
        OPCodeE(int addr, Builder* Maker)
            : Instruction(addr, "???", 0xE, Maker) {}
        OPCodeE(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0xE, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            sub05(addr, content, this);
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 6
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // E
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x12
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x16
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x1A
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x1E
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0x22
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            for (uint8_t idx = 0; idx < control_byte[0]; idx++) fun_1403c90e0(addr, content, this, 0);

        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            reading_dialog(addr, content, this);
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            auto variant = (unsigned char)control_byte[0];
            switch (variant) {
            case 0:
            case 5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            default:
                if ((uint8_t)(variant - 6U) < 3){
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                }
                else if (2 < (uint8_t)(variant - 10U)){
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
                else{
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                }
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            reading_dialog(addr, content, this);

        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode26 : public Instruction {
    public:
        OPCode26()
            : Instruction(-1, 0x26, nullptr) {}
        OPCode26(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x26, Maker) {}
        OPCode26(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x26, Maker) {}
        OPCode26(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
        : Instruction(addr, "???", 0x26, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));

        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", control_byte2));
            uint8_t variant = control_byte[0];
            if ((unsigned char)control_byte2[0] < 4) {
                switch (variant) {
                case 0:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 1:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 5:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                    break;
                case 6:
                case 7:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 8:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0x9:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0xB:
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0xC:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0xD:
                case 0xE:
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x11:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x12:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x16:
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                default:
                    if ((variant - 0x13U) < 3) {
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    }
                    break;
                }
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
            case 2:
            case 3:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 4:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 5:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };

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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            if ((unsigned char)control_byte[0] == 0) {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
            if ((unsigned char)control_byte[0] == 1) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte2));

            switch ((unsigned char)control_byte[0]) {
            case 0x01:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5->8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 9->C
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // D

                break;
            case 0x03:
            case 0x02:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // 5
                break;
            case 0x04:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5
                break;
            case 0x05:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 1:
            case 0:
            case 2:
            case 4:
            case 5:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 6:
            case 7:

                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            case 8:

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr))); // 16
                break;
            case 9:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xA:
            case 0xB:
            case 0xC:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xE:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xF:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x10:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
            case 2:
            case 3:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));

            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
            case '\n':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                return;

            case '\v':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                return;

            case '\f':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0); // 0xFF
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                return;
            case '$':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0); // 0xFF
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                return;
            }
            if (((unsigned char)control_byte[0] - 0xD) < 2) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            } else if (1 < (control_byte[0] - 0xfU)) {
                switch ((unsigned char)control_byte[0]) {
                case '\x11':
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\x12':

                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x13':
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;

                default:
                    if ((unsigned char)(control_byte[0] - 0x14U) < 2) {
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 3
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // F
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    }
                    break;
                }

            } else {

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
            switch ((unsigned char)control_byte[0]) {
            case '\x16':
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case '\x17':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case '\x18':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case '\x19':

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case '\x1A':
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case '\x1B':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case '\x1C':
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            case '\x1D':
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case '\x1E':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case '\x1F':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x21:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x22:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }

        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x00:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x01:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x02:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x03:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x04:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x07:
            case 0x05:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x06:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x08:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x09:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0C:
            case 0x0B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x0D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0E:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x0F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x10:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x11:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x12: {
                ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                switch ((uint)control_byte2[0]) {
                case 0:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                case 1:
                    break;
                default:

                    break;
                }

                break;
            }
            case 0x13:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x14:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x15:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x16:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x17:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x18:

                break;
            case 0x1E:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x19:
            case 0x1A:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x1B:

                break;
            case 0x20:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x21:
            case 0x22:
            case 0x23:
            case 0x24:
                break;
            case 0x26:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x27:

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x2A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x2C: {

                break;
            }
            case 0x2D: {

                break;
            }
            case 0x2E: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x2F: { // warning on this one, not sure if two or one short
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x1f:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x31:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x30:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x32:

                break;
            case 0x34:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x35:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x36:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x37:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x38:

                break;
            case 0x39:

                break;
            case 0x3A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x3B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x3C:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x4C:

                break;
            case 0x3D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3E:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x40:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x41:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x42:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x49:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x4B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x67:
            case 0x68:
                break;
            case 0x48:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x6C:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x46:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            case 0x47:
                break;
            case 0x4A:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x6B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x33:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;
            case 0x76:
            case 0x29:
                break;
            case 0x77:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x25:

                break;
            case 0xB5:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            case 0x62: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                ssd::Buffer byte = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", byte));

                break;
            }
            case 0x52: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
            case 0x53: {
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x54: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0x55: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x56: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x5A: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0x5B: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0x5C: {
                ssd::Buffer byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", byte2));

                break;
            }
            case 0x5E: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x61: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x63: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x64: {
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x65:
            case 0x66: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x6E: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0x6F: {
                break;
            }
            case 0x70: {
                break;
            }
            case 0x73: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x74: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x78: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x7B: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x7D: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x7E: {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
            case 0x7F: {

                break;
            }
            case 0x80: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x81: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x82: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0x87: {
                break;
            }
            case 0x83: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
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
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x8A: {

                break;
            }
            case 0x8E: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x8F: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0x90: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x91: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x92: {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0x94: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x97: {

                break;
            }
            case 0x98: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x9E: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0xA6: {
                break;
            }
            case 0xA7: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0xA8: {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xA9: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xAA: {
                break;
            }
            case 0xAB: {
                break;
            }
            case 0xAD: {
                break;
            }
            case 0xAE: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0xAF: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
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
            case 0xB3: {

                break;
            }
            case 0xB4: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                break;
            }
            case 0xB6: {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xB7: {
                ssd::Buffer byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", byte2));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0xB8: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0xBA:
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xBB:
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);

                break;
            case 0xBC:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;

            case 0xBD:

                break;
            case 0xBE: {

                break;
            }
            case 0xBF: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0xC0: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0xC1: {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xC2: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0xC3: {

                break;
            }
            case 0xC4: {
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0xC5: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xC6: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0xC8:
            case 0xC7: {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0xCB: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0xCC: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0xC9: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);

                break;
            }
            case 0xCA: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0xCD: {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0xCE: {

                break;
            }
            case 0xD2: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
            case 0xD3: {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
            case 0xD4: {
                ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                switch ((uint)control_byte2[0]) {
                case 0:
                case 1:
                    break;
                case 5:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 2:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 3:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                default:
                    throw ssd::exceptions::not_analyzed_yet(control_byte2.at(0));
                }

                break;

            }
            case 0xD5:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            default:
                throw ssd::exceptions::not_analyzed_yet(control_byte.at(0));
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                break;
            case 2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 3:
            case 0x14:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x5:
            case 0x16:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x6:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 0x7:
            case 0x17:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x8:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x9:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xA:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8->c
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // c->E
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // E->10
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 10->12
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 12->14
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 14->16
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xB:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xC:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xD:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xE:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xF:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 0x11:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x12:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x13: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x15:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1f: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x19:
            case 0x18: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x1A: {
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x1B: {

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0x1D:
            case 0x1C:{
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }

            case 0x21: {

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x28:
            case 0x29: {

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            }
            case 0x2A:
            case 0x2B: {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
            case 0x2C:
            case 0x2D: {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
            case 0x2E: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x2F: {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };

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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
            case 0x4:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x7:

                break;
            case 0x8:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x9:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
            case 0xFB:

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xFE:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0xFF:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0:
            case 0x32:
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 8->A
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // A->C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr))); // x
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 1:
            case 0x33:
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 2:
            case 0x34:
                fun_1403c90e0(addr, content, this, 1);

                break;
            case 3:
            case 0x35:
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 4:
            case 0x36:
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 5:
            case 0x37:

                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            case 0xFD:
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
            case 0x39:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x3A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                break;
            case 0x3B:
            case 0x3C:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x3E:
            case 0x3F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 100:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x65:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 0->4
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 4->8
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 8->C
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // C->10
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 10->14
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 14->18
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 18->1C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 1C->20
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 20->24
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 24->28
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 28->2C
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 2C->30
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 30->34
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 34->38
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));   // 38->3C
                break;
            case 0x3D:
                break;

            case 0x96:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            default:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 0x1:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x3:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x5:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr, "short", control_short));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            if (control == -0x1ee) {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            } else if (control == -0x1ed) {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 1
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr, "short", control_short));                      // 3
            if ((uint16_t)control == 0xFE15) {
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
            }
            else{
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 5
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 9
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // D

                if ((uint16_t)(control + 0x1feU) < 3) {

                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 11 -> 15
                }
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
            }
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 5
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 7
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // b
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // f
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 10
            if ((uint16_t)control == 0xFE05) {
                //while (content[addr] != '\0') {
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                //}
            }
        }
    };
    class OPCode41 : public Instruction {
    public:
        OPCode41()
            : Instruction(-1, 0x41, nullptr) {}
        OPCode41(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x41", 0x41, Maker) {}
        OPCode41(int addr, Builder* Maker)
            : Instruction(addr, "0x41", 0x41, Maker) {}
        OPCode41(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x41", 0x41, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte)); // 1
            if ((unsigned char)control_byte[0] == 0x04) {
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte)); // 1
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
            case 0xFF:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xFE:
            case 0x05:
            case 0x69:
            case 0x06:
            case 0x6A:
            case 0x6B:
            case 0x0A:
            case 0x0B:
            case 0x0C:

                break;
            case 0x0E:
            case 0x0D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            default:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode44 : public Instruction {
    public:
        OPCode44()
            : Instruction(-1, 0x44, nullptr) {}
        OPCode44(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x44", 0x44, Maker) {}
        OPCode44(int addr, Builder* Maker)
            : Instruction(addr, "0x44", 0x44, Maker) {}
        OPCode44(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x44", 0x44, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode45 : public Instruction {
    public:
        OPCode45()
            : Instruction(-1, 0x45, nullptr) {}
        OPCode45(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x45", 0x45, Maker) {}
        OPCode45(int addr, Builder* Maker)
            : Instruction(addr, "0x45", 0x45, Maker) {}
        OPCode45(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x45", 0x45, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode46 : public Instruction {
    public:
        OPCode46()
            : Instruction(-1, 0x46, nullptr) {}
        OPCode46(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x46", 0x46, Maker) {}
        OPCode46(int addr, Builder* Maker)
            : Instruction(addr, "0x46", 0x46, Maker) {}
        OPCode46(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x46", 0x46, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };

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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;

            }
        }
    };
    class OPCode49 : public Instruction {
    public:
        OPCode49()
            : Instruction(-1, 0x49, nullptr) {}
        OPCode49(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x49", 0x49, Maker) {}
        OPCode49(int addr, Builder* Maker)
            : Instruction(addr, "0x49", 0x49, Maker) {}
        OPCode49(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x49", 0x49, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 0:
            case 1:
            case 4:
            case 5:
            case 6:
            case 0x11:
            case 9:
            case 0x0A:
            case 0x0B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;

            case 8:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xc:
            case 0x16:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x17:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xd:

                for (int i = 0; i < 0x18; i++) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
                break;
            case 0x0E:
            case 0x0F:
            case 0x12:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            case 0x1D:
            case 0x1C:
            case 0x1B:
            case 2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x14: //30
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x15:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1E:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1F:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCode4A : public Instruction {
    public:
        OPCode4A()
            : Instruction(-1, 0x4A, nullptr) {}
        OPCode4A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4A", 0x4A, Maker) {}
        OPCode4A(int addr, Builder* Maker)
            : Instruction(addr, "0x4A", 0x4A, Maker) {}
        OPCode4A(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4A", 0x4A, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4B : public Instruction {
    public:
        OPCode4B()
            : Instruction(-1, 0x4B, nullptr) {}
        OPCode4B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4B", 0x4B, Maker) {}
        OPCode4B(int addr, Builder* Maker)
            : Instruction(addr, "0x4B", 0x4B, Maker) {}
        OPCode4B(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4B", 0x4B, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr, "short", control_short));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

        }
    };
    class OPCode4C : public Instruction {
    public:
        OPCode4C()
            : Instruction(-1, 0x4C, nullptr) {}
        OPCode4C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4C", 0x4C, Maker) {}
        OPCode4C(int addr, Builder* Maker)
            : Instruction(addr, "0x4C", 0x4C, Maker) {}
        OPCode4C(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4C", 0x4C, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4D : public Instruction {
    public:
        OPCode4D()
            : Instruction(-1, 0x4D, nullptr) {}
        OPCode4D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4D", 0x4D, Maker) {}
        OPCode4D(int addr, Builder* Maker)
            : Instruction(addr, "0x4D", 0x4D, Maker) {}
        OPCode4D(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4D", 0x4D, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4E : public Instruction {
    public:
        OPCode4E()
            : Instruction(-1, 0x4E, nullptr) {}
        OPCode4E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4E", 0x4E, Maker) {}
        OPCode4E(int addr, Builder* Maker)
            : Instruction(addr, "0x4E", 0x4E, Maker) {}
        OPCode4E(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4E", 0x4E, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode4F : public Instruction {
    public:
        OPCode4F()
            : Instruction(-1, 0x4F, nullptr) {}
        OPCode4F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x4F", 0x4F, Maker) {}
        OPCode4F(int addr, Builder* Maker)
            : Instruction(addr, "0x4F", 0x4F, Maker) {}
        OPCode4F(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x4F", 0x4F, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;

            case 3:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x06:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCode50 : public Instruction {
    public:
        OPCode50()
            : Instruction(-1, 0x50, nullptr) {}
        OPCode50(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x50", 0x50, Maker) {}
        OPCode50(int addr, Builder* Maker)
            : Instruction(addr, "0x50", 0x50, Maker) {}
        OPCode50(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x50", 0x50, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            fun_1403c90e0(addr, content, this, 0);
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode51 : public Instruction {
    public:
        OPCode51()
            : Instruction(-1, 0x51, nullptr) {}
        OPCode51(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x51", 0x51, Maker) {}
        OPCode51(int addr, Builder* Maker)
            : Instruction(addr, "0x51", 0x51, Maker) {}
        OPCode51(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x51", 0x51, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode52 : public Instruction {
    public:
        OPCode52()
            : Instruction(-1, 0x52, nullptr) {}
        OPCode52(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x52", 0x52, Maker) {}
        OPCode52(int addr, Builder* Maker)
            : Instruction(addr, "0x52", 0x52, Maker) {}
        OPCode52(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x52", 0x52, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode53 : public Instruction {
    public:
        OPCode53()
            : Instruction(-1, 0x53, nullptr) {}
        OPCode53(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x53", 0x53, Maker) {}
        OPCode53(int addr, Builder* Maker)
            : Instruction(addr, "0x53", 0x53, Maker) {}
        OPCode53(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x53", 0x53, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode54 : public Instruction {
    public:
        OPCode54()
            : Instruction(-1, 0x54, nullptr) {}
        OPCode54(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x54, Maker) {}
        OPCode54(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x54, Maker) {}
        OPCode54(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x54, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2;
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 10:
            case 0xE:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xB:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x18:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xD:

                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x13:
                break;
            case 0x14:

                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x15:

                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;
            case 0x17:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x1A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x30:
                fun_1403c90e0(addr, content, this, 1);
                break;
            case 0x24:
            case 0x22:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x19:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x1C:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x21:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x23:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x26:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;

            case 0x29:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x2B:
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x2C:

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x31:
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                if ((int)control_byte2[0] == 0) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                } else if ((int)control_byte2[0] == 1) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                } else if ((int)control_byte2[0] == 2) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
                break;
            case 0x34:

                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x35:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;
            case 0x36:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x37:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x38:

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x39:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x3A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 0x14

                break;
            case 0x58:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 0x14
                break;
            case 0x3B:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 0
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 0x3E:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;

            case 0x40:
            case 0x4B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x42:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x44:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x45:
            case 0x46:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x47:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x48:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x49:
            case 0x4A:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x4C:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x4D:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x4E:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x4F:
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2)); // 1
                switch ((int)control_byte2[0]) {
                case 0:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 1:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 2:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 3:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 4:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\n':
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 3
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));  // 4
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 10
                    break;
                }
                break;
            case 0x50:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x51:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;
            case 0x52:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 0x53:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x54:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x56:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x57:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x5A:
                fun_1403c90e0(addr, content, this, 1);
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            case 0x5B:
            case 0x5C:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x5D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x5F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x60:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x61:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x62:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 99:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x6F:
            case 0x6E:
            case 0x6D:
            case 0x6C:
            case 0x6B:
            case 0x67:
            case 0x65:
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
            case 0x79:
            case 100:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            }
        }
    };
    class OPCode55 : public Instruction {
    public:
        OPCode55()
            : Instruction(-1, 0x55, nullptr) {}
        OPCode55(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x55", 0x55, Maker) {}
        OPCode55(int addr, Builder* Maker)
            : Instruction(addr, "0x55", 0x55, Maker) {}
        OPCode55(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x55", 0x55, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode56 : public Instruction {
    public:
        OPCode56()
            : Instruction(-1, 0x56, nullptr) {}
        OPCode56(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x56", 0x56, Maker) {}
        OPCode56(int addr, Builder* Maker)
            : Instruction(addr, "0x56", 0x56, Maker) {}
        OPCode56(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x56", 0x56, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 10
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 14
        }
    };
    class OPCode57 : public Instruction {
    public:
        OPCode57()
            : Instruction(-1, 0x57, nullptr) {}
        OPCode57(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x57", 0x57, Maker) {}
        OPCode57(int addr, Builder* Maker)
            : Instruction(addr, "0x57", 0x57, Maker) {}
        OPCode57(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x57", 0x57, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode58 : public Instruction {
    public:
        OPCode58()
            : Instruction(-1, 0x58, nullptr) {}
        OPCode58(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x58", 0x58, Maker) {}
        OPCode58(int addr, Builder* Maker)
            : Instruction(addr, "0x58", 0x58, Maker) {}
        OPCode58(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x58", 0x58, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCode5A : public Instruction {
    public:
        OPCode5A()
            : Instruction(-1, 0x5A, nullptr) {}
        OPCode5A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x5A", 0x5A, Maker) {}
        OPCode5A(int addr, Builder* Maker)
            : Instruction(addr, "0x5A", 0x5A, Maker) {}
        OPCode5A(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x5A", 0x5A, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            if ((unsigned char)control_byte[0] == 0) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            }
        }
    };
    class OPCode5B : public Instruction {
    public:
        OPCode5B()
            : Instruction(-1, 0x5B, nullptr) {}
        OPCode5B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x5B", 0x5B, Maker) {}
        OPCode5B(int addr, Builder* Maker)
            : Instruction(addr, "0x5B", 0x5B, Maker) {}
        OPCode5B(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x5B", 0x5B, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            sub05(addr, content, this);
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 2:
            case 3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
            case 4:
            case 6:

            case 9:
            case 10:
            case 0xF:
            case 0x10:
            case 0x11:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0xB:
            case 2:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x12:
            case 0xD:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 8:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }

        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 2:
            case '\f':
            case '\r':
            case '\033': // \033 is the same as \e, but standard
            case 3:
            case 4:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case '\a':
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\b':
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\x18':
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case '\x0F':
            case '\v':
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x10:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;

            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
            case 4:
            case 7:
            case 8:
            case 9:
            case 0xB:
            case 0x13:
            case 0x15:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 2:
            case 3:
            case 5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 10:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            case 0xC:
            case 0xD:
            case 0x12:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x14:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x18:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x17:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x19:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1A:
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1C:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x1D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;

            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 7:
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0:
            case 3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 2:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 4:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 5:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 6:
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCode6B : public Instruction {
    public:
        OPCode6B()
            : Instruction(-1, 0x6B, nullptr) {}
        OPCode6B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x6B", 0x6B, Maker) {}
        OPCode6B(int addr, Builder* Maker)
            : Instruction(addr, "0x6B", 0x6B, Maker) {}
        OPCode6B(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x6B", 0x6B, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode6F : public Instruction {
    public:
        OPCode6F()
            : Instruction(-1, 0x6F, nullptr) {}
        OPCode6F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x6F", 0x6F, Maker) {}
        OPCode6F(int addr, Builder* Maker)
            : Instruction(addr, "0x6F", 0x6F, Maker) {}
        OPCode6F(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x6F", 0x6F, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            uint8_t variant = control_byte[0];
            switch(variant){
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
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
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x1B:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x1C:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1D:
                fun_1403c90e0(addr, content, this, 0);
                break;
            case 0x1E:
                fun_1403c90e0(addr, content, this, 0);
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1F:
            case 0x21:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x20:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            }


        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
            case 7:
            case 0xB:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 4:
            case 9:
            case 0xC:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 8:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 10:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x0D:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0E:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x0F:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode71
            : public Instruction
    {
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode72
            : public Instruction // not sure at all
    {
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr, "short", control_short));
            uint16_t control_shrt = ReadShortFromByteArray(0, control_short);
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            if ((unsigned char)(control_byte[0] - 1U) < 2) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            } else if (((unsigned char)control_byte[0]) == 0xB) {
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

            } else if (((unsigned char)control_byte[0]) == 0x6) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            }
            else if (!((unsigned char)(control_byte[0] - 3U) < 2)) {
                if (((unsigned char)(control_byte[0] - 8) & 0xFD) == 0) {
                    fun_1403c90e0(addr, content, this, 0);
                }
            } else {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1))); // skipped??
            }

            if (control_shrt != 0xFFFF) {
                if (control_shrt < 0x100) {
                    switch ((unsigned char)control_byte[0]) {
                    /*case '\x06':
                            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                            break;*/
                    }
                }
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));


            switch ((unsigned char)control_byte[0]) {
            case 1:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            default:
                if ((((char)control_byte[0] - 2U) & 0xfd) == 0) {
                    fun_1403c90e0(addr, content, this, 0);
                }
                if (((char)control_byte[0] & 0xFD) == 0){
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }



                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 3:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            }
        }
    };
    class OPCode75 : public Instruction {
    public:
        OPCode75()
            : Instruction(-1, 0x75, nullptr) {}
        OPCode75(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x75, Maker) {}
        OPCode75(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x75, Maker) {}
        OPCode75(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x75, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", control_byte2));
            uint8_t variant = control_byte[0];
            uint8_t variant2 = control_byte2[0];
            if (variant2 < 4) {
                switch (variant) {
                case 0:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 1:
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 2:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 4:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                    break;
                case 5:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                case 6:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 7:
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0xC:
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xD:
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    fun_1403c90e0(addr, content, this, 0);
                    break;
                case 0xE:
                case 0xF:
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x10:
                    this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                    break;
                default:
                    if ((uint8_t)(variant - 9U) < 3){
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                    }
                    break;

                }
            }
        }
    };
    class OPCode76 : public Instruction {
    public:
        OPCode76()
            : Instruction(-1, 0x76, nullptr) {}
        OPCode76(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x76, Maker) {}
        OPCode76(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x76, Maker) {}
        OPCode76(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x76, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            switch ((unsigned char)control_byte[0]) {

            case 0x1:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0]) {

            case 0x7:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 0:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 1:
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 3:

                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 1:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCode7C : public Instruction {
    public:
        OPCode7C()
            : Instruction(-1, 0x7C, nullptr) {}
        OPCode7C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x7C, Maker) {}
        OPCode7C(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x7C, Maker) {}
        OPCode7C(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x7C, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 0:

                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 3
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 5
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 7
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 9
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // B
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // D
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // f
                break;
            }
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 2:
            case 6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 4:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 5:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 7:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
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
            (void)content;
            addr++;
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
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
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode84 : public Instruction {
    public:
        OPCode84()
            : Instruction(-1, 0x84, nullptr) {}
        OPCode84(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "???", 0x84, Maker) {}
        OPCode84(int addr, Builder* Maker)
            : Instruction(addr, "???", 0x84, Maker) {}
        OPCode84(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "???", 0x84, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 3: { // not sure
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr, "byte", control_byte2));
                break;
            }
            default: {
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            }
            }
        }
    };
    class OPCode86 : public Instruction {
    public:
        OPCode86()
            : Instruction(-1, 0x86, nullptr) {}
        OPCode86(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x86", 0x86, Maker) {}
        OPCode86(int addr, Builder* Maker)
            : Instruction(addr, "0x86", 0x86, Maker) {}
        OPCode86(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x86", 0x86, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 2
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 4
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 6
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // 8
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // A
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2))); // c
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode87 : public Instruction {
    public:
        OPCode87()
            : Instruction(-1, 0x87, nullptr) {}
        OPCode87(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x87", 0x87, Maker) {}
        OPCode87(int addr, Builder* Maker)
            : Instruction(addr, "0x87", 0x87, Maker) {}
        OPCode87(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x87", 0x87, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode88 : public Instruction {
    public:
        OPCode88()
            : Instruction(-1, 0x88, nullptr) {}
        OPCode88(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x88", 0x88, Maker) {}
        OPCode88(int addr, Builder* Maker)
            : Instruction(addr, "0x88", 0x88, Maker) {}
        OPCode88(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x88", 0x88, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode89 : public Instruction {
    public:
        OPCode89()
            : Instruction(-1, 0x89, nullptr) {}
        OPCode89(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x89", 0x89, Maker) {}
        OPCode89(int addr, Builder* Maker)
            : Instruction(addr, "0x89", 0x89, Maker) {}
        OPCode89(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x89", 0x89, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode8A : public Instruction {
    public:
        OPCode8A()
            : Instruction(-1, 0x8A, nullptr) {}
        OPCode8A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8A", 0x8A, Maker) {}
        OPCode8A(int addr, Builder* Maker)
            : Instruction(addr, "0x8A", 0x8A, Maker) {}
        OPCode8A(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x8A", 0x8A, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xA:
            case 0xC:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xB:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0xD:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;

            case 0xFE:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xFF:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            default:
                if (((unsigned char)control_byte[0] - 0x32U) < 2) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                }
                break;
            }
        }
    };
    class OPCode8B : public Instruction {
    public:
        OPCode8B()
            : Instruction(-1, 0x8B, nullptr) {}
        OPCode8B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8B", 0x8B, Maker) {}
        OPCode8B(int addr, Builder* Maker)
            : Instruction(addr, "0x8B", 0x8B, Maker) {}
        OPCode8B(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x8B", 0x8B, Maker) {
            addr++;
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode8C : public Instruction {
    public:
        OPCode8C()
            : Instruction(-1, 0x8C, nullptr) {}
        OPCode8C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8C", 0x8C, Maker) {}
        OPCode8C(int addr, Builder* Maker)
            : Instruction(addr, "0x8C", 0x8C, Maker) {}
        OPCode8C(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x8C", 0x8C, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 3:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 4:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case '\n':
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                break;
            case '\f':
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case '\r':
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x0F:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x10:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x11:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case '\b':
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x14:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCode8D : public Instruction {
    public:
        OPCode8D()
            : Instruction(-1, 0x8D, nullptr) {}
        OPCode8D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8D", 0x8D, Maker) {}
        OPCode8D(int addr, Builder* Maker)
            : Instruction(addr, "0x8D", 0x8D, Maker) {}
        OPCode8D(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x8D", 0x8D, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode8E : public Instruction {
    public:
        OPCode8E()
            : Instruction(-1, 0x8E, nullptr) {}
        OPCode8E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8E", 0x8E, Maker) {}
        OPCode8E(int addr, Builder* Maker)
            : Instruction(addr, "0x8E", 0x8E, Maker) {}
        OPCode8E(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x8E", 0x8E, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode8F : public Instruction {
    public:
        OPCode8F()
            : Instruction(-1, 0x8F, nullptr) {}
        OPCode8F(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x8F", 0x8F, Maker) {}
        OPCode8F(int addr, Builder* Maker)
            : Instruction(addr, "0x8F", 0x8F, Maker) {}
        OPCode8F(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
        : Instruction(addr, "0x8F", 0x8F, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode98 : public Instruction {
    public:
        OPCode98()
            : Instruction(-1, 0x98, nullptr) {}
        OPCode98(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x98", 0x98, Maker) {}
        OPCode98(int addr, Builder* Maker)
            : Instruction(addr, "0x98", 0x98, Maker) {}
        OPCode98(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x98", 0x98, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr, "short", control_short));
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            switch (control_shrt) {
            case 1:
            case 2:
            case 6:
            case 7:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3E8:
            case 0x3E9:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 2000:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 3000:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 4000:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 0xFA1:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 0xFA2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;

            case 5000:
            case 0x1389:
            case 0x138A:
            case 0x1771:
            case 0x1964:
            case 6000:
            case 9000:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 7000:
            case 7001:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 8000:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 10000:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            }
        }
    };
    class OPCode90 : public Instruction {
    public:
        OPCode90()
            : Instruction(-1, 0x90, nullptr) {}
        OPCode90(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x90", 0x90, Maker) {}
        OPCode90(int addr, Builder* Maker)
            : Instruction(addr, "0x90", 0x90, Maker) {}
        OPCode90(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x90", 0x90, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCode91 : public Instruction {
    public:
        OPCode91()
            : Instruction(-1, 0x91, nullptr) {}
        OPCode91(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x91", 0x91, Maker) {}
        OPCode91(int addr, Builder* Maker)
            : Instruction(addr, "0x91", 0x91, Maker) {}
        OPCode91(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x91", 0x91, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
            case 5:
            case 0xA:
            case 0xB:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 2:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode92 : public Instruction {
    public:
        OPCode92()
            : Instruction(-1, 0x92, nullptr) {}
        OPCode92(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x92", 0x92, Maker) {}
        OPCode92(int addr, Builder* Maker)
            : Instruction(addr, "0x92", 0x92, Maker) {}
        OPCode92(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x92", 0x92, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode93 : public Instruction {
    public:
        OPCode93()
            : Instruction(-1, 0x93, nullptr) {}
        OPCode93(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x93", 0x93, Maker) {}
        OPCode93(int addr, Builder* Maker)
            : Instruction(addr, "0x93", 0x93, Maker) {}
        OPCode93(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x93", 0x93, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case -0x2e:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode94 : public Instruction {
    public:
        OPCode94()
            : Instruction(-1, 0x94, nullptr) {}
        OPCode94(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x94", 0x94, Maker) {}
        OPCode94(int addr, Builder* Maker)
            : Instruction(addr, "0x94", 0x94, Maker) {}
        OPCode94(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x94", 0x94, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCode95 : public Instruction {
    public:
        OPCode95()
            : Instruction(-1, 0x95, nullptr) {}
        OPCode95(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x95", 0x95, Maker) {}
        OPCode95(int addr, Builder* Maker)
            : Instruction(addr, "0x95", 0x95, Maker) {}
        OPCode95(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x95", 0x95, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode96 : public Instruction {
    public:
        OPCode96()
            : Instruction(-1, 0x96, nullptr) {}
        OPCode96(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x96", 0x96, Maker) {}
        OPCode96(int addr, Builder* Maker)
            : Instruction(addr, "0x96", 0x96, Maker) {}
        OPCode96(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x96", 0x96, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCode97 : public Instruction {
    public:
        OPCode97()
            : Instruction(-1, 0x97, nullptr) {}
        OPCode97(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x97", 0x97, Maker) {}
        OPCode97(int addr, Builder* Maker)
            : Instruction(addr, "0x97", 0x97, Maker) {}
        OPCode97(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x97", 0x97, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };

    class OPCode99 : public Instruction {
    public:
        OPCode99()
            : Instruction(-1, 0x99, nullptr) {}
        OPCode99(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x99", 0x99, Maker) {}
        OPCode99(int addr, Builder* Maker)
            : Instruction(addr, "0x99", 0x99, Maker) {}
        OPCode99(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x99", 0x99, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode9A : public Instruction {
    public:
        OPCode9A()
            : Instruction(-1, 0x9A, nullptr) {}
        OPCode9A(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x9A", 0x9A, Maker) {}
        OPCode9A(int addr, Builder* Maker)
            : Instruction(addr, "0x9A", 0x9A, Maker) {}
        OPCode9A(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x9A", 0x9A, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCode9B : public Instruction {
    public:
        OPCode9B()
            : Instruction(-1, 0x9B, nullptr) {}
        OPCode9B(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x9B", 0x9B, Maker) {}
        OPCode9B(int addr, Builder* Maker)
            : Instruction(addr, "0x9B", 0x9B, Maker) {}
        OPCode9B(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x9B", 0x9B, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x00:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode9C : public Instruction {
    public:
        OPCode9C()
            : Instruction(-1, 0x9C, nullptr) {}
        OPCode9C(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x9C", 0x9C, Maker) {}
        OPCode9C(int addr, Builder* Maker)
            : Instruction(addr, "0x9C", 0x9C, Maker) {}
        OPCode9C(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x9C", 0x9C, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0x00:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x02:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCode9D : public Instruction {
    public:
        OPCode9D()
            : Instruction(-1, 0x9D, nullptr) {}
        OPCode9D(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x9D", 0x9D, Maker) {}
        OPCode9D(int addr, Builder* Maker)
            : Instruction(addr, "0x9D", 0x9D, Maker) {}
        OPCode9D(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x9D", 0x9D, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCode9E : public Instruction {
    public:
        OPCode9E()
            : Instruction(-1, 0x9E, nullptr) {}
        OPCode9E(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0x9E", 0x9E, Maker) {}
        OPCode9E(int addr, Builder* Maker)
            : Instruction(addr, "0x9E", 0x9E, Maker) {}
        OPCode9E(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0x9E", 0x9E, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            uint8_t control = control_byte[0];
            switch (control) {
            case 0x1:
            case 0x00:
            case 0x02:
            case 0x10:
            case 0x13:
                break;
            case 0xF:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x11:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x12:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
                //CONTINUER!!!
            default:{
                if (0x1d < control - 0x14) {
                    switch(control){
                    case 0x32:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                        break;
                    case 0x33:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));

                        break;
                    case 0x34:{
                        ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
                        this->AddOperande(operande(addr, "byte", control_byte2));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        switch((unsigned char)control_byte2[0]){
                        case 5:
                            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                            break;
                        }
                        break;}
                    case 0x35:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x36:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x37:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x38:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        break;
                    case 0x3A:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x3B:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x3D:
                    case 0x3C:
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x3E:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x3F:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x41:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x42:
                    case 0x43:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x44:
                    case 0x45:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x46:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x48:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x49:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x4A:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x4B:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x4C:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x50:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x51:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x53:
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x54:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x56:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x59:
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    }
                }
                else {
                    ssd::Buffer control_byte3 = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr, "byte", control_byte3));
                    switch((unsigned char) control_byte3[0]){
                    case 0x1:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        break;
                    case 0x2:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x3:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x4:
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x5:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x6:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x7:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case '\t':
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x0A:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                        break;
                    case '\v':
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case '\r':
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x0E:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x0F:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x10:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x11:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x12:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x13:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x14:
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x15:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        break;
                    case 0x16:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                        break;
                    case 0x18:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x19:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                        fun_1403c90e0(addr, content, this, 0);
                        break;
                    case 0x1A:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    case 0x1B:
                        this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                        break;
                    }

                }
            }


            }
        }
    };
    class OPCodeA0 : public Instruction {
    public:
        OPCodeA0()
            : Instruction(-1, 0xA0, nullptr) {}
        OPCodeA0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA0", 0xA0, Maker) {}
        OPCodeA0(int addr, Builder* Maker)
            : Instruction(addr, "0xA0", 0xA0, Maker) {}
        OPCodeA0(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
        : Instruction(addr, "0xA0", 0xA0, Maker) {

            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA1 : public Instruction {
    public:
        OPCodeA1()
            : Instruction(-1, 0xA1, nullptr) {}
        OPCodeA1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA1", 0xA1, Maker) {}
        OPCodeA1(int addr, Builder* Maker)
            : Instruction(addr, "0xA1", 0xA1, Maker) {}
        OPCodeA1(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA1", 0xA1, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA2 : public Instruction {
    public:
        OPCodeA2()
            : Instruction(-1, 0xA2, nullptr) {}
        OPCodeA2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA2", 0xA2, Maker) {}
        OPCodeA2(int addr, Builder* Maker)
            : Instruction(addr, "0xA2", 0xA2, Maker) {}
        OPCodeA2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA2", 0xA2, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA3 : public Instruction {
    public:
        OPCodeA3()
            : Instruction(-1, 0xA3, nullptr) {}
        OPCodeA3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA3", 0xA3, Maker) {}
        OPCodeA3(int addr, Builder* Maker)
            : Instruction(addr, "0xA3", 0xA3, Maker) {}
        OPCodeA3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA3", 0xA3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA4 : public Instruction {
    public:
        OPCodeA4()
            : Instruction(-1, 0xA4, nullptr) {}
        OPCodeA4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA4", 0xA4, Maker) {}
        OPCodeA4(int addr, Builder* Maker)
            : Instruction(addr, "0xA4", 0xA4, Maker) {}
        OPCodeA4(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA4", 0xA4, Maker) {
            addr++;
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeA5 : public Instruction {
    public:
        OPCodeA5()
            : Instruction(-1, 0xA5, nullptr) {}
        OPCodeA5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA5", 0xA5, Maker) {}
        OPCodeA5(int addr, Builder* Maker)
            : Instruction(addr, "0xA5", 0xA5, Maker) {}
        OPCodeA5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA5", 0xA5, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

            switch ((unsigned char)control_byte[0]) {
                case 0x0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            }


        }
    };
    class OPCodeA6 : public Instruction {
    public:
        OPCodeA6()
            : Instruction(-1, 0xA6, nullptr) {}
        OPCodeA6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA6", 0xA6, Maker) {}
        OPCodeA6(int addr, Builder* Maker)
            : Instruction(addr, "0xA6", 0xA6, Maker) {}
        OPCodeA6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA6", 0xA6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA8 : public Instruction {
    public:
        OPCodeA8()
            : Instruction(-1, 0xA8, nullptr) {}
        OPCodeA8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA8", 0xA8, Maker) {}
        OPCodeA8(int addr, Builder* Maker)
            : Instruction(addr, "0xA8", 0xA8, Maker) {}
        OPCodeA8(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA8", 0xA8, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeA9 : public Instruction {
    public:
        OPCodeA9()
            : Instruction(-1, 0xA9, nullptr) {}
        OPCodeA9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xA9", 0xA9, Maker) {}
        OPCodeA9(int addr, Builder* Maker)
            : Instruction(addr, "0xA9", 0xA9, Maker) {}
        OPCodeA9(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xA9", 0xA9, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeAA : public Instruction {
    public:
        OPCodeAA()
            : Instruction(-1, 0xAA, nullptr) {}
        OPCodeAA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAA", 0xAA, Maker) {}
        OPCodeAA(int addr, Builder* Maker)
            : Instruction(addr, "0xAA", 0xAA, Maker) {}
        OPCodeAA(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAA", 0xAA, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeAB : public Instruction {
    public:
        OPCodeAB()
            : Instruction(-1, 0xAB, nullptr) {}
        OPCodeAB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAB", 0xAB, Maker) {}
        OPCodeAB(int addr, Builder* Maker)
            : Instruction(addr, "0xAB", 0xAB, Maker) {}
        OPCodeAB(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAB", 0xAB, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                for (int i = 4; i <= 0x36; i++) {
                    this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                }
                break;

            case 0x2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                for (int i = 3; i <= 0x65; i += 2) {
                    this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                }
                break;
            case 0x3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeAC : public Instruction {
    public:
        OPCodeAC()
            : Instruction(-1, 0xAC, nullptr) {}
        OPCodeAC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAC", 0xAC, Maker) {}
        OPCodeAC(int addr, Builder* Maker)
            : Instruction(addr, "0xAC", 0xAC, Maker) {}
        OPCodeAC(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAC", 0xAC, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            case 0x1:
                for (size_t idx = 0; idx < 4; ++idx) {
                    fun_1403c90e0(addr, content, this, 1);
                }
                break;
            case 0x2:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));

                break;
            case '\r':
            case 0x5:
            case 0x3:
            case 0x4:
            case 0x12:
            case 0x14:
            case 0x16:
                fun_1403c90e0(addr, content, this, 1);

                break;
            case 0xF:
            case '\v':
            case '\a':
            case '\t':
            case 0x13:
            case 0x15:
            case 0x17:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCodeAD : public Instruction {
    public:
        OPCodeAD()
            : Instruction(-1, 0xAD, nullptr) {}
        OPCodeAD(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAD", 0xAD, Maker) {}
        OPCodeAD(int addr, Builder* Maker)
            : Instruction(addr, "0xAD", 0xAD, Maker) {}
        OPCodeAD(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAD", 0xAD, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x00:
            case 0x01:
            case 0x03:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x02:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeAE : public Instruction {
    public:
        OPCodeAE()
            : Instruction(-1, 0xAE, nullptr) {}
        OPCodeAE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAE", 0xAE, Maker) {}
        OPCodeAE(int addr, Builder* Maker)
            : Instruction(addr, "0xAE", 0xAE, Maker) {}
        OPCodeAE(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAE", 0xAE, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeAF : public Instruction {
    public:
        OPCodeAF()
            : Instruction(-1, 0xAF, nullptr) {}
        OPCodeAF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xAF", 0xAF, Maker) {}
        OPCodeAF(int addr, Builder* Maker)
            : Instruction(addr, "0xAF", 0xAF, Maker) {}
        OPCodeAF(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xAF", 0xAF, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCodeB0 : public Instruction {
    public:
        OPCodeB0()
            : Instruction(-1, 0xB0, nullptr) {}
        OPCodeB0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB0", 0xB0, Maker) {}
        OPCodeB0(int addr, Builder* Maker)
            : Instruction(addr, "0xB0", 0xB0, Maker) {}
        OPCodeB0(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB0", 0xB0, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0x0A:
            case 0x04:
            case 0x12:
            case 0x19:
            case 0x1D:
            case 0x1E:
            case 0x21:
            case 0x25:
            case 0x28:
            case 0x2E:
            case 0x2F:
            case 0x0C:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x0B:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x10:
                fun_1403c90e0(addr, content, this, 1);
                break;
            case 0x31:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x36:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3C:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x3D:
            case 0x3E:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;

            }
        }
    };
    class OPCodeB1 : public Instruction {
    public:
        OPCodeB1()
            : Instruction(-1, 0xB1, nullptr) {}
        OPCodeB1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB1", 0xB1, Maker) {}
        OPCodeB1(int addr, Builder* Maker)
            : Instruction(addr, "0xB1", 0xB1, Maker) {}
        OPCodeB1(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB1", 0xB1, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeB2 : public Instruction {
    public:
        OPCodeB2()
            : Instruction(-1, 0xB2, nullptr) {}
        OPCodeB2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB2", 0xB2, Maker) {}
        OPCodeB2(int addr, Builder* Maker)
            : Instruction(addr, "0xB2", 0xB2, Maker) {}
        OPCodeB2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB2", 0xB2, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch((unsigned char) control_byte[0]){
            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 1:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }

        }
    };
    class OPCodeB3 : public Instruction {
    public:
        OPCodeB3()
            : Instruction(-1, 0xB3, nullptr) {}
        OPCodeB3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB3", 0xB3, Maker) {}
        OPCodeB3(int addr, Builder* Maker)
            : Instruction(addr, "0xB3", 0xB3, Maker) {}
        OPCodeB3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB3", 0xB3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeB4 : public Instruction {
    public:
        OPCodeB4()
            : Instruction(-1, 0xB4, nullptr) {}
        OPCodeB4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB4", 0xB4, Maker) {}
        OPCodeB4(int addr, Builder* Maker)
            : Instruction(addr, "0xB4", 0xB4, Maker) {}
        OPCodeB4(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
        : Instruction(addr, "0xB4", 0xB4, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeB5 : public Instruction {
    public:
        OPCodeB5()
            : Instruction(-1, 0xB5, nullptr) {}
        OPCodeB5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB5", 0xB5, Maker) {}
        OPCodeB5(int addr, Builder* Maker)
            : Instruction(addr, "0xB5", 0xB5, Maker) {}
        OPCodeB5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB5", 0xB5, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeB6 : public Instruction {
    public:
        OPCodeB6()
            : Instruction(-1, 0xB6, nullptr) {}
        OPCodeB6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB6", 0xB6, Maker) {}
        OPCodeB6(int addr, Builder* Maker)
            : Instruction(addr, "0xB6", 0xB6, Maker) {}
        OPCodeB6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB6", 0xB6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
        }
    };
    class OPCodeB7 : public Instruction {
    public:
        OPCodeB7()
            : Instruction(-1, 0xB7, nullptr) {}
        OPCodeB7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB7", 0xB7, Maker) {}
        OPCodeB7(int addr, Builder* Maker)
            : Instruction(addr, "0xB7", 0xB7, Maker) {}
        OPCodeB7(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB7", 0xB7, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeBA : public Instruction {
    public:
        OPCodeBA()
            : Instruction(-1, 0xBA, nullptr) {}
        OPCodeBA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xBA", 0xBA, Maker) {}
        OPCodeBA(int addr, Builder* Maker)
            : Instruction(addr, "0xBA", 0xBA, Maker) {}
        OPCodeBA(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xBA", 0xBA, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x14:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCodeB8 : public Instruction {
    public:
        OPCodeB8()
            : Instruction(-1, 0xB8, nullptr) {}
        OPCodeB8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB8", 0xB8, Maker) {}
        OPCodeB8(int addr, Builder* Maker)
            : Instruction(addr, "0xB8", 0xB8, Maker) {}
        OPCodeB8(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB8", 0xB8, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            fun_1403c90e0(addr, content, this, 1);
            fun_1403c90e0(addr, content, this, 1);
        }
    };
    class OPCodeB9 : public Instruction {
    public:
        OPCodeB9()
            : Instruction(-1, 0xB9, nullptr) {}
        OPCodeB9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xB9", 0xB9, Maker) {}
        OPCodeB9(int addr, Builder* Maker)
            : Instruction(addr, "0xB9", 0xB9, Maker) {}
        OPCodeB9(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xB9", 0xB9, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 1:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 2:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };

    class OPCodeBB : public Instruction {
    public:
        OPCodeBB()
            : Instruction(-1, 0xBB, nullptr) {}
        OPCodeBB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xBB", 0xBB, Maker) {}
        OPCodeBB(int addr, Builder* Maker)
            : Instruction(addr, "0xBB", 0xBB, Maker) {}
        OPCodeBB(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xBB", 0xBB, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
        }
    };
    class OPCodeBC : public Instruction {
    public:
        OPCodeBC()
            : Instruction(-1, 0xBC, nullptr) {}
        OPCodeBC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xBC", 0xBC, Maker) {}
        OPCodeBC(int addr, Builder* Maker)
            : Instruction(addr, "0xBC", 0xBC, Maker) {}
        OPCodeBC(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xBC", 0xBC, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            fun_1403c90e0(addr, content, this, 1);
            switch ((unsigned char)control_byte[0]) {
            case 0:
            case 1:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // C
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 0
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 4
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4))); // 8
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 4:
            case 5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 7:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 8:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 9:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 10:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xb:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xc:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xd:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xe:
                fun_1403c90e0(addr, content, this, 1);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0xf:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x10:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x11:
            case 0x12:
            case 0x13:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x18:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x17:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x19:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
    class OPCodeBD : public Instruction {
    public:
        OPCodeBD()
            : Instruction(-1, 0xBD, nullptr) {}
        OPCodeBD(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xBD", 0xBD, Maker) {}
        OPCodeBD(int addr, Builder* Maker)
            : Instruction(addr, "0xBD", 0xBD, Maker) {}
        OPCodeBD(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xBD", 0xBD, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x3:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCodeBE : public Instruction {
    public:
        OPCodeBE()
            : Instruction(-1, 0xBE, nullptr) {}
        OPCodeBE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xBE", 0xBE, Maker) {}
        OPCodeBE(int addr, Builder* Maker)
            : Instruction(addr, "0xBE", 0xBE, Maker) {}
        OPCodeBE(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xBE", 0xBE, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeC0 : public Instruction {
    public:
        OPCodeC0()
            : Instruction(-1, 0xC0, nullptr) {}
        OPCodeC0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC0", 0xC0, Maker) {}
        OPCodeC0(int addr, Builder* Maker)
            : Instruction(addr, "0xC0", 0xC0, Maker) {}
        OPCodeC0(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC0", 0xC0, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr, "short", control_short));
            switch (control_shrt) {
            case 0x1:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;

            case 0x3:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1000:
            case 0x3E9:
            case 0x3EB:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCodeC1 : public Instruction {
    public:
        OPCodeC1()
            : Instruction(-1, 0xC1, nullptr) {}
        OPCodeC1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC1", 0xC1, Maker) {}
        OPCodeC1(int addr, Builder* Maker)
            : Instruction(addr, "0xC1", 0xC1, Maker) {}
        OPCodeC1(int& addr, [[maybe_unused]] ssd::Buffer& content, Builder* Maker)
        : Instruction(addr, "0xC1", 0xC1, Maker) {
            addr++;
        }
    };
    class OPCodeC2 : public Instruction {
    public:
        OPCodeC2()
            : Instruction(-1, 0xC2, nullptr) {}
        OPCodeC2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC2", 0xC2, Maker) {}
        OPCodeC2(int addr, Builder* Maker)
            : Instruction(addr, "0xC2", 0xC2, Maker) {}
        OPCodeC2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC2", 0xC2, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCodeC3 : public Instruction {
    public:
        OPCodeC3()
            : Instruction(-1, 0xC3, nullptr) {}
        OPCodeC3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC3", 0xC3, Maker) {}
        OPCodeC3(int addr, Builder* Maker)
            : Instruction(addr, "0xC3", 0xC3, Maker) {}
        OPCodeC3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC3", 0xC3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x1:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCodeC4 : public Instruction {
    public:
        OPCodeC4()
            : Instruction(-1, 0xC4, nullptr) {}
        OPCodeC4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC4", 0xC4, Maker) {}
        OPCodeC4(int addr, Builder* Maker)
            : Instruction(addr, "0xC4", 0xC4, Maker) {}
        OPCodeC4(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC4", 0xC4, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x2:
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                fun_1403c90e0(addr, content, this, 0);

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 0x5:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x6:
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x8:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x9:
                fun_1403c90e0(addr, content, this, 0);

                break;
            case 0xA:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 0xB:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xD:
            case 0xC:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0xE:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xF:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x10:
                fun_1403c90e0(addr, content, this, 0);
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            }
        }
    };
    class OPCodeC5 : public Instruction {
    public:
        OPCodeC5()
            : Instruction(-1, 0xC5, nullptr) {}
        OPCodeC5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC5", 0xC5, Maker) {}
        OPCodeC5(int addr, Builder* Maker)
            : Instruction(addr, "0xC5", 0xC5, Maker) {}
        OPCodeC5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC5", 0xC5, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x4:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x6:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x7:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0xA:
            case 0x9:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xC:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;

            }
        }
    };
    class OPCodeC6 : public Instruction {
    public:
        OPCodeC6()
            : Instruction(-1, 0xC6, nullptr) {}
        OPCodeC6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC6", 0xC6, Maker) {}
        OPCodeC6(int addr, Builder* Maker)
            : Instruction(addr, "0xC6", 0xC6, Maker) {}
        OPCodeC6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC6", 0xC6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x2:
            case 0x3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x4:
            case 0x5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 0x6:
            case 0x7:
            case 0x9:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x8:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCodeC7 : public Instruction {
    public:
        OPCodeC7()
            : Instruction(-1, 0xC7, nullptr) {}
        OPCodeC7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC7", 0xC7, Maker) {}
        OPCodeC7(int addr, Builder* Maker)
            : Instruction(addr, "0xC7", 0xC7, Maker) {}
        OPCodeC7(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC7", 0xC7, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0x1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeC8 : public Instruction {
    public:
        OPCodeC8()
            : Instruction(-1, 0xC8, nullptr) {}
        OPCodeC8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC8", 0xC8, Maker) {}
        OPCodeC8(int addr, Builder* Maker)
            : Instruction(addr, "0xC8", 0xC8, Maker) {}
        OPCodeC8(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC8", 0xC8, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            switch ((unsigned char)control_byte[0]) {
            case 0x2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x3:
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeC9 : public Instruction {
    public:
        OPCodeC9()
            : Instruction(-1, 0xC9, nullptr) {}
        OPCodeC9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xC9", 0xC9, Maker) {}
        OPCodeC9(int addr, Builder* Maker)
            : Instruction(addr, "0xC9", 0xC9, Maker) {}
        OPCodeC9(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xC9", 0xC9, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
            case 0x1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0xA:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
    class OPCodeCA : public Instruction {
    public:
        OPCodeCA()
            : Instruction(-1, 0xCA, nullptr) {}
        OPCodeCA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCA", 0xCA, Maker) {}
        OPCodeCA(int addr, Builder* Maker)
            : Instruction(addr, "0xCA", 0xCA, Maker) {}
        OPCodeCA(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCA", 0xCA, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x0:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeCB : public Instruction {
    public:
        OPCodeCB()
            : Instruction(-1, 0xCB, nullptr) {}
        OPCodeCB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCB", 0xCB, Maker) {}
        OPCodeCB(int addr, Builder* Maker)
            : Instruction(addr, "0xCB", 0xCB, Maker) {}
        OPCodeCB(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCB", 0xCB, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCodeCC : public Instruction {
    public:
        OPCodeCC()
            : Instruction(-1, 0xCC, nullptr) {}
        OPCodeCC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCC", 0xCC, Maker) {}
        OPCodeCC(int addr, Builder* Maker)
            : Instruction(addr, "0xCC", 0xCC, Maker) {}
        OPCodeCC(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCC", 0xCC, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCodeCD : public Instruction {
    public:
        OPCodeCD()
            : Instruction(-1, 0xCD, nullptr) {}
        OPCodeCD(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCD", 0xCD, Maker) {}
        OPCodeCD(int addr, Builder* Maker)
            : Instruction(addr, "0xCD", 0xCD, Maker) {}
        OPCodeCD(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCD", 0xCD, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 0x2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            case 0x6:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
        }
    };
    class OPCodeCE : public Instruction {
    public:
        OPCodeCE()
            : Instruction(-1, 0xCE, nullptr) {}
        OPCodeCE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCE", 0xCE, Maker) {}
        OPCodeCE(int addr, Builder* Maker)
            : Instruction(addr, "0xCE", 0xCE, Maker) {}
        OPCodeCE(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCE", 0xCE, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr, "short", control_short));
            switch (control_shrt) {
            case 0:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 2:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 8:
            case 7:
            case 6:
            case 3:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 5:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 4:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 9:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 10:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 11:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x14:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x15:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1E:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x1F:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x28:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x32:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0x33:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
        }
    };
    class OPCodeCF : public Instruction {
    public:
        OPCodeCF()
            : Instruction(-1, 0xCF, nullptr) {}
        OPCodeCF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xCF", 0xCF, Maker) {}
        OPCodeCF(int addr, Builder* Maker)
            : Instruction(addr, "0xCF", 0xCF, Maker) {}
        OPCodeCF(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xCF", 0xCF, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
        }
    };
    class OPCodeD0 : public Instruction {
    public:
        OPCodeD0()
            : Instruction(-1, 0xD0, nullptr) {}
        OPCodeD0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD0", 0xD0, Maker) {}
        OPCodeD0(int addr, Builder* Maker)
            : Instruction(addr, "0xD0", 0xD0, Maker) {}
        OPCodeD0(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD0", 0xD0, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0x1E:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            default:

                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            }
        }
    };
    class OPCodeD1 : public Instruction {
    public:
        OPCodeD1()
            : Instruction(-1, 0xD1, nullptr) {}
        OPCodeD1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD1", 0xD1, Maker) {}
        OPCodeD1(int addr, Builder* Maker)
            : Instruction(addr, "0xD1", 0xD1, Maker) {}
        OPCodeD1(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD1", 0xD1, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));

                break;
            case 1:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
        }
    };
    class OPCodeD2 : public Instruction {
    public:
        OPCodeD2()
            : Instruction(-1, 0xD2, nullptr) {}
        OPCodeD2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD2", 0xD2, Maker) {}
        OPCodeD2(int addr, Builder* Maker)
            : Instruction(addr, "0xD2", 0xD2, Maker) {}
        OPCodeD2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD2", 0xD2, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_short = ReadSubByteArray(content, addr, 2);
            int16_t control_shrt = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr, "short", control_short));
            switch (control_shrt) {
            case 0x0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case -2:
            case -1:
                fun_1403c90e0(addr, content, this, 0);
                break;
            case 3:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeD3 : public Instruction {
    public:
        OPCodeD3()
            : Instruction(-1, 0xD3, nullptr) {}
        OPCodeD3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD3", 0xD3, Maker) {}
        OPCodeD3(int addr, Builder* Maker)
            : Instruction(addr, "0xD3", 0xD3, Maker) {}
        OPCodeD3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD3", 0xD3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
        }
    };
    class OPCodeD4 : public Instruction {
    public:
        OPCodeD4()
            : Instruction(-1, 0xD4, nullptr) {}
        OPCodeD4(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD4", 0xD4, Maker) {}
        OPCodeD4(int addr, Builder* Maker)
            : Instruction(addr, "0xD4", 0xD4, Maker) {}
        OPCodeD4(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD4", 0xD4, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            ssd::Buffer control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte2));
        }
    };
    class OPCodeD5 : public Instruction {
    public:
        OPCodeD5()
            : Instruction(-1, 0xD5, nullptr) {}
        OPCodeD5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD5", 0xD5, Maker) {}
        OPCodeD5(int addr, Builder* Maker)
            : Instruction(addr, "0xD5", 0xD5, Maker) {}
        OPCodeD5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD5", 0xD5, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 1:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));

                break;
            }
        }
    };
    class OPCodeD6 : public Instruction {
    public:
        OPCodeD6()
            : Instruction(-1, 0xD6, nullptr) {}
        OPCodeD6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD6", 0xD6, Maker) {}
        OPCodeD6(int addr, Builder* Maker)
            : Instruction(addr, "0xD6", 0xD6, Maker) {}
        OPCodeD6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD6", 0xD6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
        }
    };
    class OPCodeD7 : public Instruction {
    public:
        OPCodeD7()
            : Instruction(-1, 0xD7, nullptr) {}
        OPCodeD7(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD7", 0xD7, Maker) {}
        OPCodeD7(int addr, Builder* Maker)
            : Instruction(addr, "0xD7", 0xD7, Maker) {}
        OPCodeD7(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD7", 0xD7, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
        }
    };
    class OPCodeD8 : public Instruction {
    public:
        OPCodeD8()
            : Instruction(-1, 0xD8, nullptr) {}
        OPCodeD8(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD8", 0xD8, Maker) {}
        OPCodeD8(int addr, Builder* Maker)
            : Instruction(addr, "0xD8", 0xD8, Maker) {}
        OPCodeD8(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD8", 0xD8, Maker) {
            addr++;
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
        }
    };
    class OPCodeD9 : public Instruction {
    public:
        OPCodeD9()
            : Instruction(-1, 0xD9, nullptr) {}
        OPCodeD9(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xD9", 0xD9, Maker) {}
        OPCodeD9(int addr, Builder* Maker)
            : Instruction(addr, "0xD9", 0xD9, Maker) {}
        OPCodeD9(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xD9", 0xD9, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 3:
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                break;
            case 4:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 6:

                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeDA : public Instruction {
    public:
        OPCodeDA()
            : Instruction(-1, 0xDA, nullptr) {}
        OPCodeDA(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xDA", 0xDA, Maker) {}
        OPCodeDA(int addr, Builder* Maker)
            : Instruction(addr, "0xDA", 0xDA, Maker) {}
        OPCodeDA(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xDA", 0xDA, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
        }
    };
    class OPCodeDB : public Instruction {
    public:
        OPCodeDB()
            : Instruction(-1, 0xDB, nullptr) {}
        OPCodeDB(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xDB", 0xDB, Maker) {}
        OPCodeDB(int addr, Builder* Maker)
            : Instruction(addr, "0xDB", 0xDB, Maker) {}
        OPCodeDB(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xDB", 0xDB, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

        }
    };
    class OPCodeDC : public Instruction {
    public:
        OPCodeDC()
            : Instruction(-1, 0xDC, nullptr) {}
        OPCodeDC(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xDC", 0xDC, Maker) {}
        OPCodeDC(int addr, Builder* Maker)
            : Instruction(addr, "0xDC", 0xDC, Maker) {}
        OPCodeDC(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xDC", 0xDC, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            if ((unsigned char)control_byte[0] == 0) {
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }



        }
    };
    class OPCodeDE : public Instruction {
    public:
        OPCodeDE()
            : Instruction(-1, 0xDE, nullptr) {}
        OPCodeDE(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xDE", 0xDE, Maker) {}
        OPCodeDE(int addr, Builder* Maker)
            : Instruction(addr, "0xDE", 0xDE, Maker) {}
        OPCodeDE(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xDE", 0xDE, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            if ((unsigned char)control_byte[0] == 1) {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            }
            else if ((unsigned char)control_byte[0] == 3) {
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
            }

        }
    };
    class OPCodeDF : public Instruction {
    public:
        OPCodeDF()
            : Instruction(-1, 0xDF, nullptr) {}
        OPCodeDF(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xDF", 0xDF, Maker) {}
        OPCodeDF(int addr, Builder* Maker)
            : Instruction(addr, "0xDF", 0xDF, Maker) {}
        OPCodeDF(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xDF", 0xDF, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

        }
    };
    class OPCodeE0 : public Instruction {
    public:
        OPCodeE0()
            : Instruction(-1, 0xE0, nullptr) {}
        OPCodeE0(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE0", 0xE0, Maker) {}
        OPCodeE0(int addr, Builder* Maker)
            : Instruction(addr, "0xE0", 0xE0, Maker) {}
        OPCodeE0(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE0", 0xE0, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            case 5:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 6:
                this->AddOperande(operande(addr, "string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
    };
    class OPCodeE1 : public Instruction {
    public:
        OPCodeE1()
            : Instruction(-1, 0xE1, nullptr) {}
        OPCodeE1(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE1", 0xE1, Maker) {}
        OPCodeE1(int addr, Builder* Maker)
            : Instruction(addr, "0xE1", 0xE1, Maker) {}
        OPCodeE1(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE1", 0xE1, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {
            case 0:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr, "float", ReadSubByteArray(content, addr, 4)));
                break;
            }
        }
    };
    class OPCodeE2 : public Instruction {
    public:
        OPCodeE2()
            : Instruction(-1, 0xE2, nullptr) {}
        OPCodeE2(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE2", 0xE2, Maker) {}
        OPCodeE2(int addr, Builder* Maker)
            : Instruction(addr, "0xE2", 0xE2, Maker) {}
        OPCodeE2(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE2", 0xE2, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
            case 4:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));

                break;
            case 3:
            case 2:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 5:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;


            }
        }
    };
    class OPCodeE3 : public Instruction {
    public:
        OPCodeE3()
            : Instruction(-1, 0xE3, nullptr) {}
        OPCodeE3(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE3", 0xE3, Maker) {}
        OPCodeE3(int addr, Builder* Maker)
            : Instruction(addr, "0xE3", 0xE3, Maker) {}
        OPCodeE3(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE3", 0xE3, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));
            switch ((unsigned char)control_byte[0]) {

            case 2:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;
            case 3:
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));
                break;



            }
        }
    };
    class OPCodeE5 : public Instruction {
    public:
        OPCodeE5()
            : Instruction(-1, 0xE5, nullptr) {}
        OPCodeE5(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE5", 0xE5, Maker) {}
        OPCodeE5(int addr, Builder* Maker)
            : Instruction(addr, "0xE5", 0xE5, Maker) {}
        OPCodeE5(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE5", 0xE5, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            ssd::Buffer control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr, "byte", control_byte));

            switch ((unsigned char)control_byte[0]) {

            case 0:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 1:
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr, "byte", ReadSubByteArray(content, addr, 1)));
                break;



            }
        }
    };
    class OPCodeE6 : public Instruction {
    public:
        OPCodeE6()
            : Instruction(-1, 0xE6, nullptr) {}
        OPCodeE6(int& addr, int idx_row, QXlsx::Document& doc, Builder* Maker)
            : Instruction(addr, idx_row, doc, "0xE6", 0xE6, Maker) {}
        OPCodeE6(int addr, Builder* Maker)
            : Instruction(addr, "0xE6", 0xE6, Maker) {}
        OPCodeE6(int& addr, ssd::Buffer& content, Builder* Maker)
            : Instruction(addr, "0xE6", 0xE6, Maker) {
            addr++;
            this->AddOperande(operande(addr, "header", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr, "short", ReadSubByteArray(content, addr, 2)));



        }
    };

    std::shared_ptr<Instruction> CreateInstructionFromDAT(int& addr, ssd::Buffer& dat_content, int function_type) override {
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
            case 0x09:
                return std::make_shared<OPCode9>(addr, dat_content, this);
            case 0x0A:
                return std::make_shared<OPCodeA>(addr, dat_content, this);
                //            case 0x0B:
                //                return std::make_shared<OPCodeB>(addr, dat_content, this);
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
            case 0x19:
                return std::make_shared<OPCode19>(addr, dat_content, this);
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
                return std::make_shared<OPCode5B>(addr,dat_content,this);
            case 0x5C:
                return std::make_shared<OPCode5C>(addr, dat_content, this);
            case 0x5D:
                return std::make_shared<OPCode5D>(addr, dat_content, this);
            case 0x5E:
                return std::make_shared<OPCode5E>(addr, dat_content, this);
            case 0x5F:
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
            case 0x71:
                return std::make_shared<OPCode71>(addr, dat_content, this);
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
            case 0x7F:
                return std::make_shared<OPCode7F>(addr, dat_content, this);
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
                //            case 0x87:
                //                return std::make_shared<OPCode87>(addr, dat_content, this);
            case 0x88:
                return std::make_shared<OPCode88>(addr, dat_content, this);
            case 0x89:
                return std::make_shared<OPCode89>(addr, dat_content, this);
            case 0x8A:
                return std::make_shared<OPCode8A>(addr, dat_content, this);
                //            case 0x8B:
                //                return std::make_shared<OPCode8B>(addr, dat_content, this);
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
            case 0xA2:
                return std::make_shared<OPCodeA2>(addr, dat_content, this);
            case 0xA3:
                return std::make_shared<OPCodeA3>(addr, dat_content, this);
            case 0xA5:
                return std::make_shared<OPCodeA5>(addr, dat_content, this);
                //            case 0xA4:
                //                return std::make_shared<OPCodeA4>(addr, dat_content, this);
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
            case 0xB0:
                return std::make_shared<OPCodeB0>(addr, dat_content, this);
            case 0xB1:
                return std::make_shared<OPCodeB1>(addr, dat_content, this);
            case 0xB2:
                return std::make_shared<OPCodeB2>(addr, dat_content, this);
                //            case 0xB3:
                //                return std::make_shared<OPCodeB3>(addr, dat_content, this);
            case 0xB4:
                return std::make_shared<OPCodeB4>(addr, dat_content, this);
            case 0xB5:
                return std::make_shared<OPCodeB5>(addr, dat_content, this);
                            case 0xB6:
                                return std::make_shared<OPCodeB6>(addr, dat_content, this);
            case 0xB7:
                return std::make_shared<OPCodeB7>(addr, dat_content, this);
                //            case 0xB8:
                //                return std::make_shared<OPCodeB8>(addr, dat_content, this);
            case 0xB9:
                return std::make_shared<OPCodeB9>(addr, dat_content, this);
            case 0xBA:
                return std::make_shared<OPCodeBA>(addr, dat_content, this);
            case 0xBB:
                return std::make_shared<OPCodeBB>(addr, dat_content, this);
            case 0xBC:
                return std::make_shared<OPCodeBC>(addr, dat_content, this);
                            case 0xBD:
                                return std::make_shared<OPCodeBD>(addr, dat_content, this);
                            case 0xBE:
                                return std::make_shared<OPCodeBE>(addr, dat_content, this);
            case 0xC0:
                return std::make_shared<OPCodeC0>(addr, dat_content, this);
                //            case 0xC1:
                //                return std::make_shared<OPCodeC1>(addr, dat_content, this);
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
                //                //                case 0xCA: return std::make_shared<OPCodeCA>(addr,dat_content,this);
                //                //                case 0xCB: return std::make_shared<OPCodeCB>(addr,dat_content,this);
                //                //                case 0xCC: return std::make_shared<OPCodeCC>(addr,dat_content,this);
            case 0xCD:
                return std::make_shared<OPCodeCD>(addr, dat_content, this);
            case 0xCE:
                return std::make_shared<OPCodeCE>(addr, dat_content, this);
            case 0xCF:
                return std::make_shared<OPCodeCF>(addr, dat_content, this);
            case 0xD0:
                return std::make_shared<OPCodeD0>(addr, dat_content, this);
            case 0xD1:
                return std::make_shared<OPCodeD1>(addr, dat_content, this);
            case 0xD2:
                return std::make_shared<OPCodeD2>(addr, dat_content, this);
            case 0xD3:
                return std::make_shared<OPCodeD3>(addr, dat_content, this);
            case 0xD4:
                return std::make_shared<OPCodeD4>(addr, dat_content, this);
                //            case 0xD5:
                //                return std::make_shared<OPCodeD5>(addr, dat_content, this);
            case 0xD6:
                return std::make_shared<OPCodeD6>(addr, dat_content, this);
                //            case 0xD7:
                //                return std::make_shared<OPCodeD7>(addr, dat_content, this);
                //            case 0xD8:
                //                return std::make_shared<OPCodeD8>(addr, dat_content, this);

            case 0xD9:
                return std::make_shared<OPCodeD9>(addr, dat_content, this);
            case 0xDA:
                return std::make_shared<OPCodeDA>(addr, dat_content, this);
            case 0xDB:
                return std::make_shared<OPCodeDB>(addr, dat_content, this);
            case 0xDE:
                return std::make_shared<OPCodeDE>(addr, dat_content, this);
            case 0xDC:
                return std::make_shared<OPCodeDC>(addr, dat_content, this);
            case 0xDF:
                return std::make_shared<OPCodeDF>(addr, dat_content, this);
            case 0xE0:
                return std::make_shared<OPCodeE0>(addr, dat_content, this);
            case 0xE1:
                return std::make_shared<OPCodeE1>(addr, dat_content, this);
            case 0xE2:
                return std::make_shared<OPCodeE2>(addr, dat_content, this);
            case 0xE3:
                return std::make_shared<OPCodeE3>(addr, dat_content, this);
            case 0xE5:
                return std::make_shared<OPCodeE5>(addr, dat_content, this);
            case 0xE6:
                return std::make_shared<OPCodeE6>(addr, dat_content, this);
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
                return std::make_shared<ActionTable>(addr, dat_content, this);
            } else if (function_type == 4) {
                return std::make_shared<AlgoTable>(addr, dat_content, this);
            } else if (function_type == 5) {
                return std::make_shared<WeaponAttTable>(addr, dat_content, this);
            } else if (function_type == 6) {
                return std::make_shared<BreakTable>(addr, dat_content, this);
            } else if (function_type == 7) {
                return std::make_shared<SummonTable>(addr, dat_content, this);
            } else if (function_type == 8) {
                return std::make_shared<ReactionTable>(addr, dat_content, this);
            } else if (function_type == 9) {
                return std::make_shared<PartTable>(addr, dat_content, this);
            } else if (function_type == 10) {
                res = std::make_shared<AnimeClipTable>(addr, dat_content, this);
            } else if (function_type == 11) {
                res = std::make_shared<FieldMonsterData>(addr, dat_content, this);
            } else if (function_type == 12) {
                res = std::make_shared<FieldFollowData>(addr, dat_content, this);
            } else if (function_type == 13) {
                return std::make_shared<FC_autoX>(addr, dat_content, this);
            } else if (function_type == 14) {
                return std::make_shared<BookData99>(addr, dat_content, this);
            } else if (function_type == 15) {
                return std::make_shared<BookDataX>(addr, dat_content, this);
            } else if (function_type == 16) {
                return std::make_shared<AddCollision>(addr, dat_content, this);
            } else if (function_type == 18) {
                return std::make_shared<StyleName>(addr, dat_content, this);
            }
            if ((uint8_t)dat_content[addr] != 1) throw ssd::exceptions::past_the_end_addr();
            if (this->goal < addr) throw ssd::exceptions::past_the_end_addr();
            return res;
        }

        if (this->goal < addr) throw ssd::exceptions::past_the_end_addr();
        return {};
    }
    bool CreateHeaderFromDAT(ssd::Buffer& dat_content) override {

        display_text("Reading header...");
        int nb_functions = ReadIntegerFromByteArray(0x14, dat_content);
        int position_filename = ReadIntegerFromByteArray(0x4, dat_content);
        int position = position_filename;
        int next_position = 0;
        std::string filename = ReadStringFromByteArray(position, dat_content);
        SceneName = filename;
        int start_offset_area = ReadIntegerFromByteArray(0x8, dat_content);
        for (int idx_fun = 0; idx_fun < nb_functions; idx_fun++) {
            position = start_offset_area + 4 * idx_fun;
            next_position = start_offset_area + 4 * (idx_fun + 1);
            int addr = ReadIntegerFromByteArray(position, dat_content);
            position = start_offset_area + 4 * nb_functions + 2 * idx_fun;
            uint16_t name_pos = ReadShortFromByteArray(position, dat_content);
            int name_pos_int = name_pos;
            std::string function_name = ReadStringFromByteArray(name_pos_int, dat_content);
            size_t end_addr = 0;
            if (idx_fun == nb_functions - 1) {
                end_addr = dat_content.size();
            } else {
                end_addr = ReadIntegerFromByteArray(next_position, dat_content);
            }
            FunctionsToParse.emplace_back(idx_fun, function_name, name_pos, addr, end_addr);
        }
        display_text("Header parsed.");
        return true;
    }
    std::shared_ptr<Instruction> CreateInstructionFromXLSX(int& addr, int row, QXlsx::Document& xls_content) override {

        int OP = xls_content.read(row + 1, 2).toInt();

        switch (OP) {
        case 0x00:
            return std::make_shared<OPCode0>(addr, row, xls_content, this);
        case 0x01:
            return std::make_shared<OPCode1>(addr, row, xls_content, this);
        case 0x02:
            return std::make_shared<OPCode2>(addr, row, xls_content, this);
        case 0x03:
            return std::make_shared<OPCode3>(addr, row, xls_content, this);
        case 0x04:
            return std::make_shared<OPCode4>(addr, row, xls_content, this);
        case 0x05:
            return std::make_shared<OPCode5>(addr, row, xls_content, this);
        case 0x06:
            return std::make_shared<OPCode6>(addr, row, xls_content, this);
        case 0x07:
            return std::make_shared<OPCode7>(addr, row, xls_content, this);
        case 0x08:
            return std::make_shared<OPCode8>(addr, row, xls_content, this);
        case 0x09:
            return std::make_shared<OPCode9>(addr, row, xls_content, this);
        case 0x0A:
            return std::make_shared<OPCodeA>(addr, row, xls_content, this);
            //            case 0x0B:
            //                return std::make_shared<OPCodeB>(addr, row, xls_content, this);
        case 0x0C:
            return std::make_shared<OPCodeC>(addr, row, xls_content, this);
        case 0x0D:
            return std::make_shared<OPCode0D>(addr, row, xls_content, this);
        case 0x0E:
            return std::make_shared<OPCodeE>(addr, row, xls_content, this);
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
        case 0x4E:
            return std::make_shared<OPCode4E>(addr, row, xls_content, this);
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
        case 0x54:
            return std::make_shared<OPCode54>(addr, row, xls_content, this);
        case 0x55:
            return std::make_shared<OPCode55>(addr, row, xls_content, this);
        case 0x56:
            return std::make_shared<OPCode56>(addr, row, xls_content, this);
        case 0x57:
            return std::make_shared<OPCode57>(addr, row, xls_content, this);
        case 0x58:
            return std::make_shared<OPCode58>(addr, row, xls_content, this);
        case 0x5A:
            return std::make_shared<OPCode5A>(addr, row, xls_content, this);
        case 0x5B:
            return std::make_shared<OPCode5B>(addr,row, xls_content,this);
        case 0x5C:
            return std::make_shared<OPCode5C>(addr, row, xls_content, this);
        case 0x5D:
            return std::make_shared<OPCode5D>(addr, row, xls_content, this);
        case 0x5E:
            return std::make_shared<OPCode5E>(addr, row, xls_content, this);
        case 0x5F:
            return std::make_shared<OPCode5E>(addr, row, xls_content, this);
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
        case 0x75:
            return std::make_shared<OPCode75>(addr, row, xls_content, this);
        case 0x76:
            return std::make_shared<OPCode76>(addr, row, xls_content, this);
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
        case 0x7C:
            return std::make_shared<OPCode7C>(addr, row, xls_content, this);
        case 0x7D:
            return std::make_shared<OPCode7D>(addr, row, xls_content, this);
        case 0x7E:
            return std::make_shared<OPCode7E>(addr, row, xls_content, this);
        case 0x7F:
            return std::make_shared<OPCode7F>(addr, row, xls_content, this);
        case 0x80:
            return std::make_shared<OPCode80>(addr, row, xls_content, this);
        case 0x82:
            return std::make_shared<OPCode82>(addr, row, xls_content, this);
        case 0x83:
            return std::make_shared<OPCode83>(addr, row, xls_content, this);
        case 0x84:
            return std::make_shared<OPCode84>(addr, row, xls_content, this);
        case 0x86:
            return std::make_shared<OPCode86>(addr, row, xls_content, this);
            //            case 0x87:
            //                return std::make_shared<OPCode87>(addr, row, xls_content, this);
        case 0x88:
            return std::make_shared<OPCode88>(addr, row, xls_content, this);
        case 0x89:
            return std::make_shared<OPCode89>(addr, row, xls_content, this);
        case 0x8A:
            return std::make_shared<OPCode8A>(addr, row, xls_content, this);
            //            case 0x8B:
            //                return std::make_shared<OPCode8B>(addr, row, xls_content, this);
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
        case 0x9C:
            return std::make_shared<OPCode9C>(addr, row, xls_content, this);
        case 0x9D:
            return std::make_shared<OPCode9D>(addr, row, xls_content, this);
        case 0x9E:
            return std::make_shared<OPCode9E>(addr, row, xls_content, this);
        case 0xA0:
            return std::make_shared<OPCodeA0>(addr, row, xls_content, this);
        case 0xA1:
            return std::make_shared<OPCodeA1>(addr, row, xls_content, this);
        case 0xA2:
            return std::make_shared<OPCodeA2>(addr, row, xls_content, this);
        case 0xA3:
            return std::make_shared<OPCodeA3>(addr, row, xls_content, this);
            //            case 0xA4:
            //                return std::make_shared<OPCodeA4>(addr, row, xls_content, this);

        case 0xA5:
            return std::make_shared<OPCodeA5>(addr, row, xls_content, this);
        case 0xA6:
            return std::make_shared<OPCodeA6>(addr, row, xls_content, this);
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
        case 0xAE:
            return std::make_shared<OPCodeAE>(addr, row, xls_content, this);
        case 0xAF:
            return std::make_shared<OPCodeAF>(addr, row, xls_content, this);
        case 0xB0:
            return std::make_shared<OPCodeB0>(addr, row, xls_content, this);
        case 0xB1:
            return std::make_shared<OPCodeB1>(addr, row, xls_content, this);
        case 0xB2:
            return std::make_shared<OPCodeB2>(addr, row, xls_content, this);
            //            case 0xB3:
            //                return std::make_shared<OPCodeB3>(addr, row, xls_content, this);
        case 0xB4:
            return std::make_shared<OPCodeB4>(addr, row, xls_content, this);
        case 0xB5:
            return std::make_shared<OPCodeB5>(addr, row, xls_content, this);
                        case 0xB6:
                            return std::make_shared<OPCodeB6>(addr, row, xls_content, this);
        case 0xB7:
            return std::make_shared<OPCodeB7>(addr, row, xls_content, this);
            //            case 0xB8:
            //                return std::make_shared<OPCodeB8>(addr, row, xls_content, this);
        case 0xB9:
            return std::make_shared<OPCodeB9>(addr, row, xls_content, this);
        case 0xBA:
            return std::make_shared<OPCodeBA>(addr, row, xls_content, this);
        case 0xBB:
            return std::make_shared<OPCodeBB>(addr, row, xls_content, this);
        case 0xBC:
            return std::make_shared<OPCodeBC>(addr, row, xls_content, this);
                        case 0xBD:
                            return std::make_shared<OPCodeBD>(addr, row, xls_content, this);
                        case 0xBE:
                            return std::make_shared<OPCodeBE>(addr, row, xls_content, this);
        case 0xC0:
            return std::make_shared<OPCodeC0>(addr, row, xls_content, this);
            //            case 0xC1:
            //                return std::make_shared<OPCodeC1>(addr, row, xls_content, this);
        case 0xC2:
            return std::make_shared<OPCodeC2>(addr, row, xls_content, this);
        case 0xC3:
            return std::make_shared<OPCodeC3>(addr, row, xls_content, this);
        case 0xC4:
            return std::make_shared<OPCodeC4>(addr, row, xls_content, this);
        case 0xC5:
            return std::make_shared<OPCodeC5>(addr, row, xls_content, this);
        case 0xC6:
            return std::make_shared<OPCodeC6>(addr, row, xls_content, this);
        case 0xC7:
            return std::make_shared<OPCodeC7>(addr, row, xls_content, this);
        case 0xC8:
            return std::make_shared<OPCodeC8>(addr, row, xls_content, this);
        case 0xC9:
            return std::make_shared<OPCodeC9>(addr, row, xls_content, this);
            //                //                case 0xCA: return std::make_shared<OPCodeCA>(addr,row, xls_content,this);
            //                //                case 0xCB: return std::make_shared<OPCodeCB>(addr,row, xls_content,this);
            //                //                case 0xCC: return std::make_shared<OPCodeCC>(addr,row, xls_content,this);
        case 0xCD:
            return std::make_shared<OPCodeCD>(addr, row, xls_content, this);
        case 0xCE:
            return std::make_shared<OPCodeCE>(addr, row, xls_content, this);
        case 0xCF:
            return std::make_shared<OPCodeCF>(addr, row, xls_content, this);
        case 0xD0:
            return std::make_shared<OPCodeD0>(addr, row, xls_content, this);
        case 0xD1:
            return std::make_shared<OPCodeD1>(addr, row, xls_content, this);
        case 0xD2:
            return std::make_shared<OPCodeD2>(addr, row, xls_content, this);
        case 0xD3:
            return std::make_shared<OPCodeD3>(addr, row, xls_content, this);
        case 0xD4:
            return std::make_shared<OPCodeD4>(addr, row, xls_content, this);
            //            case 0xD5:
            //                return std::make_shared<OPCodeD5>(addr, row, xls_content, this);
        case 0xD6:
            return std::make_shared<OPCodeD6>(addr, row, xls_content, this);
            //            case 0xD7:
            //                return std::make_shared<OPCodeD7>(addr, row, xls_content, this);
            //            case 0xD8:
            //                return std::make_shared<OPCodeD8>(addr, row, xls_content, this);
        case 0xD9:
            return std::make_shared<OPCodeD9>(addr, row, xls_content, this);
        case 0xDA:
            return std::make_shared<OPCodeDA>(addr, row, xls_content, this);
        case 0xDB:
            return std::make_shared<OPCodeDB>(addr, row, xls_content, this);
        case 0xDC:
            return std::make_shared<OPCodeDC>(addr, row, xls_content, this);
        case 0xDE:
            return std::make_shared<OPCodeDE>(addr, row, xls_content, this);
        case 0xDF:
            return std::make_shared<OPCodeDF>(addr, row, xls_content, this);
        case 0xE0:
            return std::make_shared<OPCodeE0>(addr, row, xls_content, this);
        case 0xE1:
            return std::make_shared<OPCodeE1>(addr, row, xls_content, this);
        case 0xE2:
            return std::make_shared<OPCodeE2>(addr, row, xls_content, this);
        case 0xE3:
            return std::make_shared<OPCodeE3>(addr, row, xls_content, this);
        case 0xE5:
            return std::make_shared<OPCodeE5>(addr, row, xls_content, this);
        case 0xE6:
            return std::make_shared<OPCodeE6>(addr, row, xls_content, this);
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
        case 273:
            return std::make_shared<AnimeClipData>(addr, row, xls_content, this);
        case 274:
            return std::make_shared<StyleName>(addr, row, xls_content, this);
        default:
            error = true;
            throw ssd::exceptions::bad_opcode(OP, addr);
        }
    }

    ssd::Buffer CreateHeaderBytes() override {

        ssd::Buffer header;

        ssd::Buffer scene_name_bytes = ssd::Buffer::fromStdString(SceneName);
        scene_name_bytes.push_back('\x0');
        size_t size_of_scene_name = scene_name_bytes.size();

        size_t nb_byte_to_add_scene_name = (((size_t)std::ceil((float)(size_of_scene_name) / 4))) * 4 - size_of_scene_name;
        size_of_scene_name += nb_byte_to_add_scene_name;
        header.push_back(GetBytesFromInt(0x20));
        header.push_back(GetBytesFromInt(0x20));
        header.push_back(GetBytesFromInt(0x20 + size_of_scene_name + 4));
        header.push_back(GetBytesFromInt(FunctionsParsed.size() * 4));
        header.push_back(GetBytesFromInt(0x20 + size_of_scene_name + 4 + FunctionsParsed.size() * 4));
        header.push_back(GetBytesFromInt(FunctionsParsed.size()));
        size_t length_of_names_section = 0;
        for (auto& fun : FunctionsParsed) {
            length_of_names_section = length_of_names_section + fun.name.size() + 1;
        }
        header.push_back(GetBytesFromInt(0x20 + size_of_scene_name + 4 + FunctionsParsed.size() * 4 + FunctionsParsed.size() * 2 +
                                         length_of_names_section));
        header.push_back(GetBytesFromInt(0xABCDEF00));
        header.push_back(scene_name_bytes);
        for (size_t i = 0; i < nb_byte_to_add_scene_name; i++) {
            header.push_back('\x0');
        }
        header.push_back(GetBytesFromInt(0xFF000000)); // specific to haji, never read
        if (!FunctionsParsed.empty()) {
            ssd::Buffer position_names;
            ssd::Buffer actual_names;
            size_t offset_names = 0;
            for (auto& fun : FunctionsParsed) {
                header.push_back(GetBytesFromInt(fun.actual_addr));
                ssd::Buffer name = ssd::Buffer::fromStdString(fun.name);
                name.push_back('\x0');
                position_names.push_back(GetBytesFromShort(0x20 + size_of_scene_name + 4 + FunctionsParsed.size() * 4 +
                                                           FunctionsParsed.size() * 2 + offset_names));
                actual_names.push_back(name);
                offset_names = offset_names + name.size();
            }
            header.push_back(position_names);
            header.push_back(actual_names);
            int multiple = 4;
            if (FunctionsParsed[0].name.starts_with("_")) multiple = 0x10;
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

#endif // REVERIEINSTRUCTIONSSET_H
