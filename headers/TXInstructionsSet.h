#ifndef TXINSTRUCTIONSSET_H
#define TXINSTRUCTIONSSET_H
#include "headers/utilities.h"
#include "headers/functions.h"
#include "headers/translationfile.h"

#include <QString>



class TXTranslationFile : public TranslationFile
{
    public:
    TXTranslationFile():TranslationFile(){}

};
class TXBuilder : public Builder
{
    public:
    TXBuilder(){

    }

    QVector<QString> TXUIFiles = {"battle_menu","camp_menu", "camp_menu_v","note_menu","note_menu_v","shop_menu","shop_menu_v","title_menu","title_menu_v"};
    static void reading_dialog(int &addr, QByteArray &content, Instruction * instr){

        QByteArray current_op_value;
        int addr_ = addr;

        bool start_text = false;
        int cnt = 0;
        do{
            unsigned char current_byte = content[addr];

            switch(current_byte){
                case 0x00:
                    current_op_value.clear();
                    current_op_value[0] = 0;
                    instr->AddOperande(operande(addr,"byte", current_op_value));
                    addr++;
                    return;
                case 0x01:

                    if (start_text)
                        current_op_value.push_back(current_byte);
                    else{

                        current_op_value.clear();
                        current_op_value.push_back(current_byte);
                        instr->AddOperande(operande(addr,"byte", current_op_value));
                        current_op_value.clear();
                    }
                    addr++;
                    break;
                case 0x02:
                    start_text = false;
                    if (current_op_value.size()>0) {
                        instr->AddOperande(operande(addr_,"dialog", current_op_value));

                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr,"byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    break;
                case 0x10:
                    if (current_op_value.size()>0) {
                        instr->AddOperande(operande(addr_,"dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr,"byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x17:
                case 0x19:
                    start_text = false;
                    if (current_op_value.size()>0) {
                        instr->AddOperande(operande(addr_,"dialog", current_op_value));

                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr,"byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x11:
                case 0x12:
                    if (current_op_value.size()>0) {
                        instr->AddOperande(operande(addr_,"dialog", current_op_value));
                    }
                    current_op_value.clear();
                    current_op_value.push_back(current_byte);
                    instr->AddOperande(operande(addr,"byte", current_op_value));
                    current_op_value.clear();
                    addr++;
                    instr->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x23:
                    current_op_value.push_back(0x23);
                    addr_ = addr;
                    addr++;
                    current_byte = content[addr];
                    if ((((current_byte == 0x45) || (current_byte == 0x65)) || (current_byte == 0x4d)) ||
                           (current_byte == 0x42)||(current_byte == 0x48)||(current_byte == 0x56)||(current_byte == 0x4b) ||
                            (current_byte == 0x6b) || (current_byte == 0x46)) {
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];

                              if (current_byte != 0x5f) {
                                  if (current_byte == 0x5b) {
                                      do{
                                          current_op_value.push_back(current_byte);
                                          addr++;
                                          current_byte = content[addr];
                                      }
                                      while(current_byte!=0x5D);
                                      current_op_value.push_back(current_byte);
                                      addr++;
                                      current_byte = content[addr];
                                  }
                                  break;
                              }
                              else{
                                  current_op_value.push_back(current_byte);
                                  addr++;
                                  current_byte = content[addr];
                                  current_op_value.push_back(current_byte);
                                  addr++;
                                  current_byte = content[addr];
                                  break;
                              }

                           }
                        else if(((current_byte + 0xb7 & 0xdf) == 0)||(current_byte == 0x50)||(current_byte == 0x54)||(current_byte == 0x57)||
                                 (current_byte == 0x53)||(current_byte == 0x73)||(current_byte == 0x43)||(current_byte == 99)||(current_byte == 0x78)||
                                 (current_byte == 0x79)||(current_byte == 0x47)||(current_byte == 0x44)||(current_byte == 0x55)||(current_byte == 0x52)) {
                                 current_op_value.push_back(current_byte);
                                 addr++;
                                 current_byte = content[addr];
                                 break;
                        }
                        break;
                default:
                    if (current_byte < 0x20){
                        if (current_op_value.size()>0) {
                            instr->AddOperande(operande(addr_,"dialog", current_op_value));
                        }
                        start_text = false;
                        current_op_value.clear();
                        current_op_value.push_back(current_byte);
                        instr->AddOperande(operande(addr,"byte", current_op_value));
                        addr++;
                        current_op_value.clear();
                    }
                    else {
                        if (!(start_text)) start_text = true;

                        if ((current_byte < 0xE0)&&(current_byte >= 0xC0)){
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                        }
                        else if ((current_byte >= 0xE0)&&(current_byte <= 0xF7)){
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                            current_op_value.push_back(current_byte);
                            addr++;
                        }
                        else if ((current_byte > 0xF7)){
                            current_op_value.push_back(current_byte);
                            addr++;
                        }
                        else{
                            current_op_value.push_back(current_byte);
                            addr++;
                        }
                    }
                    break;
            }


            cnt++;
        }
        while(cnt<9999);

    }
    static void fun_1403c90e0(int &addr, QByteArray &content, Instruction * instr, int param){
        QByteArray control_byte3_arr = ReadSubByteArray(content, addr, 1);
        instr->AddOperande(operande(addr,"byte", control_byte3_arr));
        unsigned char control_byte3 = (unsigned char)control_byte3_arr[0];

        if (control_byte3 == '\x11'){
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

        }
        else if (control_byte3 != '3'){

            if (control_byte3 == '\"'){
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

            }
            else if (control_byte3 != 0x44){

                if (control_byte3 == 0xDD){


                    if (param!=0)instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                }
                else if (control_byte3 == 0xFF){
                    instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                }
                else{
                    if (control_byte3 != 0xEE){

                    }
                    else{
                        instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    }
                }


            }
            else{
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                if (param!=0)instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }

        }
        else{
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

        }
    }
    static void sub05(int &addr, QByteArray &content, Instruction * instr){
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        instr->AddOperande(operande(addr,"byte", control_byte));

        while ((int)control_byte[0]!=1){

                switch ((unsigned char)control_byte[0])  {
                    case 0x0:
                    case 0x24:
                        instr->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                        break;

                    case 0x1c:{
                        //the next byte is the OP code for a new instruction

                        std::shared_ptr<Instruction> instr2 = instr->Maker->CreateInstructionFromDAT(addr, content,0);

                        operande op = operande(addr,"instruction", instr2->getBytes());
                        instr->AddOperande(op);


                        break;
                    }
                    case 0x1e:
                        instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    case 0x1f:
                    case 0x20:
                    case 0x23:

                        instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 0x21:
                        instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    case 0x25:
                        instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    default:
                        ;
                }

                control_byte = ReadSubByteArray(content, addr, 1);

                instr->AddOperande(operande(addr,"byte", control_byte));

            }

    }
    class CreateMonsters : public Instruction
    {
        public:
        CreateMonsters():Instruction(-1,256,nullptr){}
        CreateMonsters(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"CreateMonsters", 256,Maker){}
        CreateMonsters(int addr, Builder *Maker):Instruction(addr,"CreateMonsters", 256, Maker){}
        CreateMonsters(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"CreateMonsters", 256,Maker){
            int initial_addr = addr;
            int max_nb_monsters = 8;
            if (Maker->goal<addr+0x20){

                addr = initial_addr;
                Maker->flag_monsters = false;
                return;
            }
            int first = ReadIntegerFromByteArray(addr,content);

            if ((first == -1)&&(addr + 0x20 == Maker->goal)){

                this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 0x1C))); //?? we don't forget to take the int as well
                Maker->flag_monsters = true;
                return;
            }
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            int check1 = ReadIntegerFromByteArray(addr,content);
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

            int check2 = ReadIntegerFromByteArray(addr,content);
            if (check1 && check2 != 0){ //bad

                addr = initial_addr;
                Maker->flag_monsters = false;
                return;
            }
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2))); //0x10
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2))); //0x1A
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4))); //0x1C
            //0x20


            int cnt = 0;
            do{


                QByteArray array = ReadSubByteArray(content, addr, 4);
                first = ReadIntegerFromByteArray(0,array);
                this->AddOperande(operande(addr,"int", array));

                int counter = 0;

                if (first == -2){

                    QByteArray array = ReadSubByteArray(content, addr, 4);
                    this->AddOperande(operande(addr,"int", array));
                    max_nb_monsters = 4;
                }
                else if (first == -1){
                    this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 0x18))); //?? it's empty
                    Maker->flag_monsters = true;
                    return;
                }
                else max_nb_monsters = 8;

                if (Maker->goal<addr + (0x10)*max_nb_monsters + max_nb_monsters*2){

                    addr = initial_addr;
                    Maker->flag_monsters = false;
                    return;
                }

                do{
                    counter++;
                    QByteArray monsters_name = ReadStringSubByteArray(content, addr);
                    this->AddOperande(operande(addr,"string", monsters_name));

                    QByteArray remaining = ReadSubByteArray(content, addr, 0x10-monsters_name.size()-1);
                    operande fill = operande(addr,"fill", remaining);
                    fill.setBytesToFill(0x10);
                    this->AddOperande(fill);

                }
                while(counter < max_nb_monsters);

                for (int idx_byte = 0; idx_byte < max_nb_monsters; idx_byte++) this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                if ((unsigned char) content[addr] == 0)this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, max_nb_monsters))); //??
                else {
                    QByteArray monsters_name = ReadStringSubByteArray(content, addr);
                    this->AddOperande(operande(addr,"string", monsters_name));

                    QByteArray remaining = ReadSubByteArray(content, addr, 12-monsters_name.size()-1);
                    operande fill = operande(addr,"bytearray", remaining);
                    fill.setBytesToFill(12);
                    this->AddOperande(fill);


                }

                first = ReadIntegerFromByteArray(addr,content);

                cnt++;

            }
            while((first != -1)&&(addr != Maker->goal - 4));
            if (addr == Maker->goal - 4) {
                first = ReadIntegerFromByteArray(addr,content);
                if (first != 1){
                    addr = initial_addr;
                    Maker->flag_monsters = false;
                    return;
                }
                else{
                    Maker->flag_monsters = true;
                    return;
                }

            }
            this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 0x1C))); //?? we don't forget to take the int as well
            Maker->flag_monsters = true;
        }


    };
    class EffectsInstr : public Instruction
    {
        public:
        EffectsInstr():Instruction(-1,257,nullptr){}
        EffectsInstr(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"EffectsInstr", 257,Maker){}
        EffectsInstr(int addr, Builder *Maker):Instruction(addr,"EffectsInstr", 257, Maker){}
        EffectsInstr(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"EffectsInstr", 257,Maker){
            bool bytes_blocks_remain = Maker->goal >= addr+0x28;

            while (bytes_blocks_remain){

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                QByteArray str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));

                QByteArray remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);


                operande fill = operande(addr,"fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                bytes_blocks_remain = Maker->goal >= addr+0x28;

            }

        }


    };
    class ActionTable : public Instruction
    {
        public:
        ActionTable():Instruction(-1,258,nullptr){}
        ActionTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"ActionTable", 258,Maker){}
        ActionTable(int addr, Builder *Maker):Instruction(addr,"ActionTable", 258, Maker){}
        ActionTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"ActionTable", 258,Maker){

            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            int cnt = 0;
            while(cnt < current_byte){



                QByteArray short_bytes = ReadSubByteArray(content, addr,2);
                this->AddOperande(operande(addr,"short", short_bytes));//2
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-46
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-45
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-44
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-43
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-42
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-41
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-40
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-3F
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-3E
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//ebp-3D
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-3C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-38
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-34
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-30
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-2C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-28
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//ebp-24
                QByteArray str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                QByteArray remaining = ReadSubByteArray(content, addr, 0x10-str.size()-1);
                operande fill = operande(addr,"fill", remaining);
                fill.setBytesToFill(0x10);
                this->AddOperande(fill);

                QByteArray str_ = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str_));
                QByteArray remaining_ = ReadSubByteArray(content, addr, 0x20-str_.size()-1);
                operande fill_ = operande(addr,"fill", remaining_);
                fill_.setBytesToFill(0x20);
                this->AddOperande(fill_);


                QByteArray str__ = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str__));
                QByteArray remaining__ = ReadSubByteArray(content, addr, 0x30-str__.size()-1);
                operande fill__ = operande(addr,"fill", remaining__);
                fill__.setBytesToFill(0x30);
                this->AddOperande(fill__);

                cnt++;
          }


        }


    };
    class AddCollision : public Instruction
    {
        public:
        AddCollision():Instruction(-1,271,nullptr){}
        AddCollision(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"AddCollision", 271,Maker){}
        AddCollision(int addr, Builder *Maker):Instruction(addr,"AddCollision", 271, Maker){}
        AddCollision(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"AddCollision", 271,Maker){


            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            int cnt = 0;
            while(cnt < current_byte){
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                cnt++;
          }


        }


    };
    class ConditionTable : public Instruction
    {
        public:
        ConditionTable():Instruction(-1,272,nullptr){}
        ConditionTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"ConditionTable", 272,Maker){}
        ConditionTable(int addr, Builder *Maker):Instruction(addr,"ConditionTable", 272, Maker){}
        ConditionTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"ConditionTable", 272,Maker){

            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            int cnt = 0;
            while(cnt < current_byte){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//EBP-52
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-50
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-4C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-48
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-44
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-40
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-3C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-38
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-34
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-30
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-2C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-28
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-24
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-20
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-1C
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-18
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4))); //EBP-14
                cnt++;
          }


        }


    };
    class AlgoTable : public Instruction //42d177
    {
        public:
        AlgoTable():Instruction(-1,259,nullptr){}
        AlgoTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"AlgoTable", 259,Maker){}
        AlgoTable(int addr, Builder *Maker):Instruction(addr,"AlgoTable", 259, Maker){}
        AlgoTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"AlgoTable", 259,Maker){
            int cnt = 0;
            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//3
            while(cnt < current_byte){
                QByteArray short_bytes = ReadSubByteArray(content, addr,2);

                this->AddOperande(operande(addr,"short", short_bytes));


                this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0x1E)));
                if (addr+0x20 > Maker->goal) return;
                cnt++;
            }

            //this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0x1E)));

        }
    };
    class WeaponAttTable : public Instruction
    {
        public:
        WeaponAttTable():Instruction(-1,260,nullptr){}
        WeaponAttTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"WeaponAttTable", 260,Maker){}
        WeaponAttTable(int addr, Builder *Maker):Instruction(addr,"WeaponAttTable", 260, Maker){}
        WeaponAttTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"WeaponAttTable", 260,Maker){

            this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,4)));
        }
    };
    class BreakTable : public Instruction
    {
        public:
        BreakTable():Instruction(-1,261,nullptr){}
        BreakTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"BreakTable", 261,Maker){}
        BreakTable(int addr, Builder *Maker):Instruction(addr,"BreakTable", 261, Maker){}
        BreakTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"BreakTable", 261,Maker){
            int cnt = 0;
            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//3
            while(cnt < current_byte){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                cnt++;
            }
        }
    };
    class SummonTable : public Instruction //140142002
    {
        public:
        SummonTable():Instruction(-1,262,nullptr){}
        SummonTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"SummonTable", 262,Maker){}
        SummonTable(int addr, Builder *Maker):Instruction(addr,"SummonTable", 262, Maker){}
        SummonTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"SummonTable", 262,Maker){

            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            int cnt = 0;
            while(cnt < current_byte){
                QByteArray short_bytes = ReadSubByteArray(content, addr,2);
                ushort shrt = ReadShortFromByteArray(0, short_bytes);
                this->AddOperande(operande(addr,"short", short_bytes));
                if (shrt == 0xFFFF) break;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                QByteArray str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                QByteArray remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);
                operande fill = operande(addr,"fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);
                cnt++;
            }

        }
    };
    class ReactionTable : public Instruction //140142002
    {
        public:
        ReactionTable():Instruction(-1,263,nullptr){}
        ReactionTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"ReactionTable", 263,Maker){}
        ReactionTable(int addr, Builder *Maker):Instruction(addr,"ReactionTable", 263, Maker){}
        ReactionTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"ReactionTable", 263,Maker){
            int cnt = 0;
            ushort current_shrt = ReadShortFromByteArray(addr,content);
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

            while(cnt < current_shrt){
                QByteArray short_bytes = ReadSubByteArray(content, addr,2);

                this->AddOperande(operande(addr,"short", short_bytes));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                cnt++;
            }
        }
    };
    class PartTable : public Instruction //14019797c
    {
        public:
        PartTable():Instruction(-1,264,nullptr){}
        PartTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"PartTable", 264,Maker){}
        PartTable(int addr, Builder *Maker):Instruction(addr,"PartTable", 264, Maker){}
        PartTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"PartTable", 264,Maker){
            unsigned char current_byte = content[addr];
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            int cnt = 0;
            while(cnt < current_byte){

                QByteArray int_bytes = ReadSubByteArray(content, addr,4);
                this->AddOperande(operande(addr,"int", int_bytes));

                QByteArray str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                QByteArray remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);
                operande fill = operande(addr,"fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);
                fill = operande(addr,"fill", remaining);
                fill.setBytesToFill(0x20);
                this->AddOperande(fill);

                cnt++;
            }

        }
    };

    class AnimeClipTable : public Instruction // from CS3
    {
        public:
        AnimeClipTable():Instruction(-1,265,nullptr){}
        AnimeClipTable(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"AnimeClipTable", 265,Maker){}
        AnimeClipTable(int addr, Builder *Maker):Instruction(addr,"AnimeClipTable", 265, Maker){}
        AnimeClipTable(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"AnimeClipTable", 265,Maker){

            int first_integer;
            first_integer = ReadIntegerFromByteArray(addr, content);
            std::vector<function>::iterator itt_current_fun = find_function_by_ID(Maker->FunctionsToParse, Maker->idx_current_fun);
            while(first_integer != 0){
                itt_current_fun->AddInstruction(std::make_shared<AnimeClipData>(addr, content,Maker));
                first_integer = ReadIntegerFromByteArray(addr, content);

            }
            QByteArray first_integer_bytes = ReadSubByteArray(content, addr,4);
            this->AddOperande(operande(addr,"int", first_integer_bytes));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };
    class AnimeClipData : public Instruction
    {
        public:
        AnimeClipData():Instruction(-1,273,nullptr){}
        AnimeClipData(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"AnimeClipData", 273,Maker){}
        AnimeClipData(int addr, Builder *Maker):Instruction(addr,"AnimeClipData", 273, Maker){}
        AnimeClipData(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"AnimeClipData", 273,Maker){


                QByteArray first_integer_bytes = ReadSubByteArray(content, addr,4);
                this->AddOperande(operande(addr,"int", first_integer_bytes));
                QByteArray str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                QByteArray remaining = ReadSubByteArray(content, addr, (0x20)-str.size()-1);
                operande fill = operande(addr,"fill", remaining);
                fill.setBytesToFill((0x20));
                this->AddOperande(fill);
                str = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", str));
                remaining = ReadSubByteArray(content, addr, (0x20)-str.size()-1);
                fill = operande(addr,"fill", remaining);
                fill.setBytesToFill((0x20));
                this->AddOperande(fill);



        }
    };
    class FieldMonsterData : public Instruction // 00000001402613C2 probably trigger only for monsters on the field
    {
        public:
        FieldMonsterData():Instruction(-1,266,nullptr){}
        FieldMonsterData(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"FieldMonsterData", 266,Maker){}
        FieldMonsterData(int addr, Builder *Maker):Instruction(addr,"FieldMonsterData", 266, Maker){}
        FieldMonsterData(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"FieldMonsterData", 266,Maker){

            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));


        }
    };
    class FieldFollowData : public Instruction //
    {
        public:
        FieldFollowData():Instruction(-1,267,nullptr){}
        FieldFollowData(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"FieldMonsterData", 267,Maker){}
        FieldFollowData(int addr, Builder *Maker):Instruction(addr,"FieldMonsterData", 267, Maker){}
        FieldFollowData(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"FieldMonsterData", 267,Maker){

            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }
    };
    class FC_autoX : public Instruction //
    {
        public:
        FC_autoX():Instruction(-1,268,nullptr){}
        FC_autoX(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"FC_autoX", 268,Maker){}
        FC_autoX(int addr, Builder *Maker):Instruction(addr,"FC_autoX", 268, Maker){}
        FC_autoX(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"FC_autoX", 268,Maker){

            QByteArray things = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr,"string", things));
        }
    };
    class BookData99 : public Instruction //
    {
        public:
        BookData99():Instruction(-1,269,nullptr){}
        BookData99(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"BookData99", 269,Maker){}
        BookData99(int addr, Builder *Maker):Instruction(addr,"BookData99", 269, Maker){}
        BookData99(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"BookData99", 269,Maker){
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };
    class BookDataX: public Instruction
    {
        public:
        BookDataX():Instruction(-1,270,nullptr){}
        BookDataX(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"BookDataX", 270,Maker){}
        BookDataX(int addr, Builder *Maker):Instruction(addr,"BookDataX", 270, Maker){}
        BookDataX(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"BookDataX", 270,Maker){
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            short control = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"short", control_short));//3
            if (control>0){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                QByteArray title = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", title));
                QByteArray remaining = ReadSubByteArray(content, addr, 0x10-title.size()-1);
                operande fill = operande(addr,"bytearray", remaining);
                fill.setBytesToFill(0x10);
                this->AddOperande(fill);
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x12
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x14
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x16
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x18
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x1A
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x1C
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x1E
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x20
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x22
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//RCX+0x24
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }
            else
            {
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }
        }
    };


    class OPCode0 : public Instruction
    {
        public:
        OPCode0():Instruction(-1,0,nullptr){}
        OPCode0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"Instruction 0", 0,Maker){}
        OPCode0(int addr, Builder *Maker):Instruction(addr,"Instruction 0", 0, Maker){}
        OPCode0(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"Instruction 0", 0,Maker){
            addr++;

        }


    };
    class OPCode1 : public Instruction
    {
        public:
        OPCode1():Instruction(-1,1,nullptr){}
        OPCode1(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"Return", 1,Maker){}
        OPCode1(int addr, Builder *Maker):Instruction(addr,"Return",1,Maker){}
        OPCode1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"Return", 1,Maker){
            addr++;
        }


    };

    class UI_OP13: public Instruction{
    public:
        UI_OP13():Instruction(-1,0x13,nullptr){}
        UI_OP13(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x13,Maker){}
        UI_OP13(int addr, Builder *Maker):Instruction(addr,"???",0x13,Maker){}
        UI_OP13(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x13,Maker){
        addr++;

        QByteArray control_byte = ReadSubByteArray(content, addr, 1);

        this->AddOperande(operande(addr,"byte", control_byte));
        switch ((unsigned char) control_byte[0])  {

            case 0x00:{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x01:{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 10:{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0xb:{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xc:{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }

            case 0x26:
            case 0xd:{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0xe:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0xf:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x10:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x11:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x12:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x13:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x14:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x15:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x16:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x17:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x18:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x19:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x1B:
            case 0x1A:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x1C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x1D:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x20:
            case 0x1F:
            case 0x1E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x21:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x22:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }

            case 0x23:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x24:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x25:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x27:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x28:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x29:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x2A:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x2B:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x2C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x2D:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x2E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x30:
            case 0x2F:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x33:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x34:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x35:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x36:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x37:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x3B:
            case 0x38:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x3A:
            case 0x39:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x3C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x41:
            case 0x3D:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x3E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x3F:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x40:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x44:
            case 0x43:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x65:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                break;
             }
            case 0x68:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                break;
             }
            case 0x76:
            case 0x69:
            case 0x75:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                break;
             }
            case 0x6B:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
             }
            case 0x6A:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
             }
            case 0x6C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
             }
            case 0x70:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                break;
             }
            case 0x71:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
             }
            case 0x72:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
             }
            case 0x73:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
             }

            case 0x32:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
             }
            case 0x31:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
             }
            default:{
                qFatal("not analyzed yet");
            }
            }
        }

    };
    //SCENARIO FILES INSTRUCTIONS
    //0x05
    class OPCode05: public Instruction{
    public:
        OPCode05():Instruction(-1,0x05,nullptr){}
        OPCode05(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x05,Maker){}
        OPCode05(int addr, Builder *Maker):Instruction(addr,"???",0x05,Maker){}
        OPCode05(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x05,Maker){
            addr++;
            sub05(addr, content, this);

            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4)));
        }
    };

    //0x02
    class OPCode02: public Instruction{
    public:
        OPCode02():Instruction(-1,0x02,nullptr){}
        OPCode02(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x02,Maker){}
        OPCode02(int addr, Builder *Maker):Instruction(addr,"???",0x02,Maker){}
        OPCode02(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x02,Maker){
        addr++;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }
    };
    //0x03
    class OPCode03: public Instruction{
    public:
        OPCode03():Instruction(-1,0x03,nullptr){}
        OPCode03(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x03,Maker){}
        OPCode03(int addr, Builder *Maker):Instruction(addr,"???",0x03,Maker){}
        OPCode03(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x03,Maker){
            addr++;
            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr,4)));

        }
    };
    //0x04
    class OPCode04: public Instruction{
        public:
        OPCode04():Instruction(-1,0x04,nullptr){}
        OPCode04(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x04,Maker){}
        OPCode04(int addr, Builder *Maker):Instruction(addr,"???",0x04,Maker){}
        OPCode04(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x04,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }
    };
    //0x06
    class OPCode06: public Instruction{
        public:
            OPCode06():Instruction(-1,0x06,nullptr){}
            OPCode06(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x06,Maker){}
            OPCode06(int addr, Builder *Maker):Instruction(addr,"???",0x06,Maker){}
            OPCode06(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x06,Maker){
            addr++;
            sub05(addr, content, this);
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            if ((unsigned char) control_byte[0] != 0){


                for (unsigned char i = 0; i<(unsigned char)control_byte[0]; i++){

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr,4)));

                }
            }
            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr,4)));
           }

    };
    //0x07
    class OPCode07: public Instruction{
    public:
        OPCode07():Instruction(-1,0x07,nullptr){}
        OPCode07(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x07,Maker){}
        OPCode07(int addr, Builder *Maker):Instruction(addr,"???",0x07,Maker){}
        OPCode07(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x07,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x08
    class OPCode08: public Instruction{
    public:
        OPCode08():Instruction(-1,0x08,nullptr){}
        OPCode08(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x08,Maker){}
        OPCode08(int addr, Builder *Maker):Instruction(addr,"???",0x08,Maker){}
        OPCode08(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x08,Maker){
            addr++;
            sub05(addr, content, this);
        }

    };
    //0x0A
    class OPCode0A: public Instruction{
    public:
        OPCode0A():Instruction(-1,0x0A,nullptr){}
        OPCode0A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x0A,Maker){}
        OPCode0A(int addr, Builder *Maker):Instruction(addr,"???",0x0A,Maker){}
        OPCode0A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x0A,Maker){
        addr++;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
        sub05(addr, content, this);

        }
    };
    //0x0C
    class OPCode0C: public Instruction{
    public:
        OPCode0C():Instruction(-1,0x0C,nullptr){}
        OPCode0C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x0C,Maker){}
        OPCode0C(int addr, Builder *Maker):Instruction(addr,"???",0x0C,Maker){}
        OPCode0C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x0C,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x0D
    class OPCode0D: public Instruction{
    public:
        OPCode0D():Instruction(-1,0x0D,nullptr){}
        OPCode0D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x0D,Maker){}
        OPCode0D(int addr, Builder *Maker):Instruction(addr,"???",0x0D,Maker){}
        OPCode0D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x0D,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x0E
    class OPCode0E: public Instruction{
    public:
        OPCode0E():Instruction(-1,0x0E,nullptr){}
        OPCode0E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x0E,Maker){}
        OPCode0E(int addr, Builder *Maker):Instruction(addr,"???",0x0E,Maker){}
        OPCode0E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x0E,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
       }
    };
    //0x0F
    class OPCode0F: public Instruction{
    public:
        OPCode0F():Instruction(-1,0x0F,nullptr){}
        OPCode0F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x0F,Maker){}
        OPCode0F(int addr, Builder *Maker):Instruction(addr,"???",0x0F,Maker){}
        OPCode0F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x0F,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x10
    class OPCode10: public Instruction{
    public:
        OPCode10():Instruction(-1,0x10,nullptr){}
        OPCode10(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x10,Maker){}
        OPCode10(int addr, Builder *Maker):Instruction(addr,"???",0x10,Maker){}
        OPCode10(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x10,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x11
    class OPCode11: public Instruction{
    public:
        OPCode11():Instruction(-1,0x11,nullptr){}
        OPCode11(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x11,Maker){}
        OPCode11(int addr, Builder *Maker):Instruction(addr,"???",0x11,Maker){}
        OPCode11(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x11,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x12
    class OPCode12: public Instruction{
    public:
        OPCode12():Instruction(-1,0x12,nullptr){}
        OPCode12(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x12,Maker){}
        OPCode12(int addr, Builder *Maker):Instruction(addr,"???",0x12,Maker){}
        OPCode12(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x12,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            sub05(addr, content, this);
        }
    };
    //0x13
    class OPCode13: public Instruction{
    public:
        OPCode13():Instruction(-1,0x13,nullptr){}
        OPCode13(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x13,Maker){}
        OPCode13(int addr, Builder *Maker):Instruction(addr,"???",0x13,Maker){}
        OPCode13(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x13,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            QString fun_name = ReadStringFromByteArray(addr, content);
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));



            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//i think this one is the id of the battle function it triggers
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };

    //0x14
    class OPCode14: public Instruction{
    public:
        OPCode14():Instruction(-1,0x14,nullptr){}
        OPCode14(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x14,Maker){}
        OPCode14(int addr, Builder *Maker):Instruction(addr,"???",0x14,Maker){}
        OPCode14(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x14,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }

    };
    //0x15
    class OPCode15: public Instruction{
    public:
        OPCode15():Instruction(-1,0x15,nullptr){}
        OPCode15(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x15,Maker){}
        OPCode15(int addr, Builder *Maker):Instruction(addr,"???",0x15,Maker){}
        OPCode15(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x15,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

        }
    };
    //0x16
    class OPCode16: public Instruction{
    public:
        OPCode16():Instruction(-1,0x16,nullptr){}
        OPCode16(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x16,Maker){}
        OPCode16(int addr, Builder *Maker):Instruction(addr,"???",0x16,Maker){}
        OPCode16(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x16,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
          }

    };
    //0x17
    class OPCode17: public Instruction{
    public:
        OPCode17():Instruction(-1,0x17,nullptr){}
        OPCode17(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x17,Maker){}
        OPCode17(int addr, Builder *Maker):Instruction(addr,"???",0x17,Maker){}
        OPCode17(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x17,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }
    };
    //0x18
    class OPCode18: public Instruction{
    public:
        OPCode18():Instruction(-1,0x18,nullptr){}
        OPCode18(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x18,Maker){}
        OPCode18(int addr, Builder *Maker):Instruction(addr,"???",0x18,Maker){}
        OPCode18(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x18,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            reading_dialog(addr, content, this);
        }

    };
    //0x19
    class OPCode19: public Instruction{
    public:
        OPCode19():Instruction(-1,0x19,nullptr){}
        OPCode19(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x19,Maker){}
        OPCode19(int addr, Builder *Maker):Instruction(addr,"???",0x19,Maker){}
        OPCode19(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x19,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x05:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

            }
        }

    };
    //0x1A
    class OPCode1A: public Instruction{
    public:
        OPCode1A():Instruction(-1,0x1A,nullptr){}
        OPCode1A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1A,Maker){}
        OPCode1A(int addr, Builder *Maker):Instruction(addr,"???",0x1A,Maker){}
        OPCode1A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1A,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            reading_dialog(addr, content, this);
        }

    };
    //0x1B
    class OPCode1B: public Instruction{
    public:
        OPCode1B():Instruction(-1,0x1B,nullptr){}
        OPCode1B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1B,Maker){}
        OPCode1B(int addr, Builder *Maker):Instruction(addr,"???",0x1B,Maker){}
        OPCode1B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1B,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
        }

    };
    //0x1C
    class OPCode1C: public Instruction{
    public:
        OPCode1C():Instruction(-1,0x1C,nullptr){}
        OPCode1C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1C,Maker){}
        OPCode1C(int addr, Builder *Maker):Instruction(addr,"???",0x1C,Maker){}
        OPCode1C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1C,Maker){
            addr++;
        }

    };
    //0x1D
    class OPCode1D: public Instruction{
    public:
        OPCode1D():Instruction(-1,0x1D,nullptr){}
        OPCode1D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1D,Maker){}
        OPCode1D(int addr, Builder *Maker):Instruction(addr,"???",0x1D,Maker){}
        OPCode1D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1D,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x1E
    class OPCode1E: public Instruction{
    public:
        OPCode1E():Instruction(-1,0x1E,nullptr){}
        OPCode1E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1E,Maker){}
        OPCode1E(int addr, Builder *Maker):Instruction(addr,"???",0x1E,Maker){}
        OPCode1E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1E,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        }

    };
    //0x1F
    class OPCode1F: public Instruction{
    public:
        OPCode1F():Instruction(-1,0x1F,nullptr){}
        OPCode1F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x1F,Maker){}
        OPCode1F(int addr, Builder *Maker):Instruction(addr,"???",0x1F,Maker){}
        OPCode1F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte2));
        if ((unsigned char)control_byte2[0]<0x4){
            switch((unsigned char)control_byte[0]) {

                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }

                case 0x01:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x05:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x07:
                case 0x06:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x08:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x09:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0B:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

                case 0x0E:
                case 0x0D:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }


            }
        }

        }

    };
    //0x20
    class OPCode20: public Instruction{
        public:
        OPCode20():Instruction(-1,0x20,nullptr){}
        OPCode20(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x20,Maker){}
        OPCode20(int addr, Builder *Maker):Instruction(addr,"???",0x20,Maker){}
        OPCode20(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x20,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch ((unsigned char)control_byte[0])  {
                case 0x02:
                case 0x01:
                case 0x04:
                case 0x03:
                case 0x00:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

            }
        }


    };
    //0x21
    class OPCode21: public Instruction{
        public:
        OPCode21():Instruction(-1,0x21,nullptr){}
        OPCode21(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x21,Maker){}
        OPCode21(int addr, Builder *Maker):Instruction(addr,"???",0x21,Maker){}
        OPCode21(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x21,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            switch ((unsigned char)control_byte[0])  {
                case 0x00:
                {
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                    break;
                }
                case 0x01:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }

            }
        }

    };
    //0x22
    class OPCode22: public Instruction{
        public:
        OPCode22():Instruction(-1,0x22,nullptr){}
        OPCode22(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x22,Maker){}
        OPCode22(int addr, Builder *Maker):Instruction(addr,"???",0x22,Maker){}
        OPCode22(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x22,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        }

    };
    //0x23
    class OPCode23: public Instruction{
        public:
        OPCode23():Instruction(-1,0x23,nullptr){}
        OPCode23(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x23,Maker){}
        OPCode23(int addr, Builder *Maker):Instruction(addr,"???",0x23,Maker){}
        OPCode23(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x23,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        }

    };
    //0x24
    class OPCode24: public Instruction{
        public:
        OPCode24():Instruction(-1,0x24,nullptr){}
        OPCode24(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x24,Maker){}
        OPCode24(int addr, Builder *Maker):Instruction(addr,"???",0x24,Maker){}
        OPCode24(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x24,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch((unsigned char)control_byte[0]){
                case 0:
                case 1:
                case 2:
                case 4:
                case 10:
                case 0xB:
                case 5:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 7:
                case 6:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 9:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0xE:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 8:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }

            }


        }

    };
    //0x25
    class OPCode25: public Instruction{
        public:
        OPCode25():Instruction(-1,0x25,nullptr){}
        OPCode25(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x25,Maker){}
        OPCode25(int addr, Builder *Maker):Instruction(addr,"???",0x25,Maker){}
        OPCode25(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x25,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            switch((unsigned char)control_byte[0]){
                /*case 1:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }*/
                case 7:
                case 8:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

            }

        }

    };
    //0x26
    class OPCode26: public Instruction{
        public:
        OPCode26():Instruction(-1,0x26,nullptr){}
        OPCode26(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x26,Maker){}
        OPCode26(int addr, Builder *Maker):Instruction(addr,"???",0x26,Maker){}
        OPCode26(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x26,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

        }

    };
    //0x27
    class OPCode27: public Instruction{
        public:
        OPCode27():Instruction(-1,0x27,nullptr){}
        OPCode27(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x27,Maker){}
        OPCode27(int addr, Builder *Maker):Instruction(addr,"???",0x27,Maker){}
        OPCode27(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x27,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){

                case 0x0A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0x0B:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x0C:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                }


                case 0x17:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

                case 0x12:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

                case 0x14:{
                    qFatal("not entirely sure");
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x13:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x15:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;
                }
                case 0xD:
                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                    break;
                }
                case 0xE:
                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x0F:
                case 0x11:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x10:
                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x16:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x18:{
                    qFatal("not sureeeeee");
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x19:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x1A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

            }
        }

    };
    //0x28
    /*class OPCode28: public Instruction{
    public:
        OPCode28():Instruction(-1,0x28,nullptr){}
        OPCode28(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x28,Maker){}
        OPCode28(int addr, Builder *Maker):Instruction(addr,"???",0x28,Maker){}
        OPCode28(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x28,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
            case 0x00:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x01:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '\b':
            case 0x06:
            case 0x04:
            {
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '\a':
            case 0x05:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

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
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '\x0f':
            case '\n':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '\f':
            case '4':
            case 'Z':
            case '\v':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\x0e':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }

            case '\x11':
            case 'z':{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '\x12':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                for (int i = 0; i<8; i++) this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '\x13':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\x15':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '2':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case '5':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '6':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '7':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case '<':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'F':{

                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'P':{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case '[':{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\\':{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 'a':{

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 'd':{

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 'n':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '~':
            case 'q':{

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 'r':{

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case '{':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 'i':{



                break;
            }
            case 'j':{


                break;
            }
            case 0x51:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x91:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x1D:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x27:{


                break;
            }
            case 0x26:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
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
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x19:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'b':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x97:{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
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
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x68:{

                break;
            }
            case 0x92:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x93:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x96:
            case 0x95:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x9D:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0xA0:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0xA1:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0xA2:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA3:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA4:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xA5:{
                 this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                 this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                 this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
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
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x67:{

                break;
            }
            case 0x03:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x6F:{

                break;
            }
            case 0x70:{

                break;
            }
            case 0x83:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
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
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x9E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x16:{

                break;
            }
            case 0x2E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x7F:{

                break;
            }
            case 0x89:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

            case 0x8A:{
                break;
            }
            case 0x52:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x1F:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x18:{

                break;
            }
            case 0x1E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x2C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x2D:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x20:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

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
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x8F:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x8E:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x99:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x9B:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x9C:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x7D:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x49:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x88:{

                break;
            }
            case 0x80:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x9F:{

                break;
            }
            case 0x81:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x82:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            default:{
                qFatal("not analyzed yet");
            }

        }


        }

    };*/
    //0x28
    class OPCode28: public Instruction{
    public:
        OPCode28():Instruction(-1,0x28,nullptr){}
        OPCode28(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x28,Maker){}
        OPCode28(int addr, Builder *Maker):Instruction(addr,"???",0x28,Maker){}
        OPCode28(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x28,Maker){
            addr++;

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));


        }

    };
    //0x29
    class OPCode29: public Instruction{
    public:
        OPCode29():Instruction(-1,0x29,nullptr){}
        OPCode29(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x29,Maker){}
        OPCode29(int addr, Builder *Maker):Instruction(addr,"???",0x29,Maker){}
        OPCode29(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x29,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x2A
    class OPCode2A: public Instruction{
    public:
        OPCode2A():Instruction(-1,0x2A,nullptr){}
        OPCode2A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2A,Maker){}
        OPCode2A(int addr, Builder *Maker):Instruction(addr,"???",0x2A,Maker){}
        OPCode2A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2A,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x2B
    class OPCode2B: public Instruction{
    public:
        OPCode2B():Instruction(-1,0x2B,nullptr){}
        OPCode2B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2B,Maker){}
        OPCode2B(int addr, Builder *Maker):Instruction(addr,"???",0x2B,Maker){}
        OPCode2B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2B,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };

    //0x2C
    /*class OPCode2C: public Instruction{
    public:
        OPCode2C():Instruction(-1,0x2C,nullptr){}
        OPCode2C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2C,Maker){}
        OPCode2C(int addr, Builder *Maker):Instruction(addr,"???",0x2C,Maker){}
        OPCode2C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2C,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

        }

    };*/
    //0x2C
    class OPCode2C: public Instruction{
    public:
        OPCode2C():Instruction(-1,0x2C,nullptr){}
        OPCode2C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2C,Maker){}
        OPCode2C(int addr, Builder *Maker):Instruction(addr,"???",0x2C,Maker){}
        OPCode2C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2C,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){

            case 0x02:{

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x14:
            case 0x03:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x04:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x05:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x07:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x08:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x09:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x0B:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x0D:
            case 0x0C:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x0E:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x0F:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x11:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x12:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x13:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;


                break;
            }
            case 0x15:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x16:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x17:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x18:
            case 0x19:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x1A:{

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            }
            case 0x1B:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x1E:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
            case 0x22:
            case 0x1F:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x21:
            case 0x20:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x24:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x25:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x26:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                break;
            }

        }
        }

    };
    //0x2D
    class OPCode2D: public Instruction{
    public:
        OPCode2D():Instruction(-1,0x2D,nullptr){}
        OPCode2D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2D,Maker){}
        OPCode2D(int addr, Builder *Maker):Instruction(addr,"???",0x2D,Maker){}
        OPCode2D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2D,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }

    };
    //0x2E
    class OPCode2E: public Instruction{
    public:
        OPCode2E():Instruction(-1,0x2E,nullptr){}
        OPCode2E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2E,Maker){}
        OPCode2E(int addr, Builder *Maker):Instruction(addr,"???",0x2E,Maker){}
        OPCode2E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            switch((unsigned char)control_byte[0]){
                case 0x03:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
            }
        }

    };
    //0x2F
    class OPCode2F: public Instruction{
    public:
        OPCode2F():Instruction(-1,0x2F,nullptr){}
        OPCode2F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x2F,Maker){}
        OPCode2F(int addr, Builder *Maker):Instruction(addr,"???",0x2F,Maker){}
        OPCode2F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x30
    class OPCode30: public Instruction{
    public:
        OPCode30():Instruction(-1,0x30,nullptr){}
        OPCode30(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x30,Maker){}
        OPCode30(int addr, Builder *Maker):Instruction(addr,"???",0x30,Maker){}
        OPCode30(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x30,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 1:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 2:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                default:{
                    break;
                }
                case 3:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 4:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 5:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 6:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
            }
        }
    };
    //0x31
    class OPCode31: public Instruction{
    public:
        OPCode31():Instruction(-1,0x31,nullptr){}
        OPCode31(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x31,Maker){}
        OPCode31(int addr, Builder *Maker):Instruction(addr,"???",0x31,Maker){}
        OPCode31(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x31,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));


        if ((unsigned char)control_byte[0] > 0xFD){
            if ((unsigned char)control_byte[0] == 0xFE){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
            else if ((unsigned char)control_byte[0] == 0xFF){
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            }

            return;
        }



        switch ((unsigned char)control_byte[0])  {
            case 0x00:
            case 0x14:
            case 0x32:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


                break;
            }
            case 0x33:
            case 0x01:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x34:
            case 0x02:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x35:
            case 0x03:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x39:
            case 0x37:
            case 0x3B:
            case 0x05:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

            case 0x3A:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 100:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x65:
            {
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x96:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x97:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
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
            default:{this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));}


        }
        }

    };
    //0x33
    class OPCode32: public Instruction{
    public:
        OPCode32():Instruction(-1,0x32,nullptr){}
        OPCode32(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x32,Maker){}
        OPCode32(int addr, Builder *Maker):Instruction(addr,"???",0x32,Maker){}
        OPCode32(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x32,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        switch((unsigned char)control_byte[0]){
            case 0x01:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
            case 0x02:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x03:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x04:
            {
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
        }
        }

    };
    //0x33
    class OPCode33: public Instruction{
    public:
        OPCode33():Instruction(-1,0x33,nullptr){}
        OPCode33(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x33,Maker){}
        OPCode33(int addr, Builder *Maker):Instruction(addr,"???",0x33,Maker){}
        OPCode33(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x33,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }
    };
    //0x34
    class OPCode34: public Instruction{
    public:
        OPCode34():Instruction(-1,0x34,nullptr){}
        OPCode34(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x34,Maker){}
        OPCode34(int addr, Builder *Maker):Instruction(addr,"???",0x34,Maker){}
        OPCode34(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x34,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

       }

    };
    //0x35
    class OPCode35: public Instruction{
    public:
        OPCode35():Instruction(-1,0x35,nullptr){}
        OPCode35(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x35,Maker){}
        OPCode35(int addr, Builder *Maker):Instruction(addr,"???",0x35,Maker){}
        OPCode35(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x35,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x36
    class OPCode36: public Instruction{
    public:
        OPCode36():Instruction(-1,0x36,nullptr){}
        OPCode36(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x36,Maker){}
        OPCode36(int addr, Builder *Maker):Instruction(addr,"???",0x36,Maker){}
        OPCode36(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x36,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));
            short second_arg = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            if ((second_arg == -0x1fe)||(second_arg == -0x1fd)){
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            }
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            if (second_arg == -0x1fc){
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }
        }

    };
    //0x37
    class OPCode37: public Instruction{
    public:
        OPCode37():Instruction(-1,0x37,nullptr){}
        OPCode37(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x37,Maker){}
        OPCode37(int addr, Builder *Maker):Instruction(addr,"???",0x37,Maker){}
        OPCode37(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x37,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

        }

    };
    //0x38
    class OPCode38: public Instruction{
    public:
        OPCode38():Instruction(-1,0x38,nullptr){}
        OPCode38(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x38,Maker){}
        OPCode38(int addr, Builder *Maker):Instruction(addr,"???",0x38,Maker){}
        OPCode38(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x38,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
       }

    };
    //0x39
    class OPCode39: public Instruction{
    public:
        OPCode39():Instruction(-1,0x39,nullptr){}
        OPCode39(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x39,Maker){}
        OPCode39(int addr, Builder *Maker):Instruction(addr,"???",0x39,Maker){}
        OPCode39(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x39,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch(code){
                case 0xFF:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x0C:
                case 0x0B:
                case 0x0A:
                case 0x69:
                case 0x05:
                case 0xFE:{
                    break;
                }
                default:{
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
            }

       }

    };
    //0x3A
    class OPCode3A: public Instruction{
    public:
        OPCode3A():Instruction(-1,0x3A,nullptr){}
        OPCode3A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3A,Maker){}
        OPCode3A(int addr, Builder *Maker):Instruction(addr,"???",0x3A,Maker){}
        OPCode3A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3A,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x3B
    class OPCode3B: public Instruction{
    public:
        OPCode3B():Instruction(-1,0x3B,nullptr){}
        OPCode3B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3B,Maker){}
        OPCode3B(int addr, Builder *Maker):Instruction(addr,"???",0x3B,Maker){}
        OPCode3B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3B,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x3C


    class OPCode3C: public Instruction{
        public:
        OPCode3C():Instruction(-1,0x3C,nullptr){}
        OPCode3C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3C,Maker){}
        OPCode3C(int addr, Builder *Maker):Instruction(addr,"???",0x3C,Maker){}
        OPCode3C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3C,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));
            QByteArray control_short2 = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short2));
            QByteArray control_short3 = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short3));
            ushort short1 = ReadShortFromByteArray(0, control_short);
            QByteArray control_short4 = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short4));

            if (short1 == 1){
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            }


        }

    };
    //0x3D
    class OPCode3D: public Instruction{
    public:
        OPCode3D():Instruction(-1,0x3D,nullptr){}
        OPCode3D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3D,Maker){}
        OPCode3D(int addr, Builder *Maker):Instruction(addr,"???",0x3D,Maker){}
        OPCode3D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3D,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x3E
    class OPCode3E: public Instruction{
    public:
        OPCode3E():Instruction(-1,0x3E,nullptr){}
        OPCode3E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3E,Maker){}
        OPCode3E(int addr, Builder *Maker):Instruction(addr,"???",0x3E,Maker){}
        OPCode3E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3E,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };
    //0x3F
    class OPCode3F: public Instruction{
    public:
        OPCode3F():Instruction(-1,0x3F,nullptr){}
        OPCode3F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x3F,Maker){}
        OPCode3F(int addr, Builder *Maker):Instruction(addr,"???",0x3F,Maker){}
        OPCode3F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x09:
                case 0x0A:
                case 0x0B:
                case 0x01:
                case 0x06:
                case 0x05:
                case 0x04:
                case 0x10:
                case 0x13:
                case 0x00:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x08:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x0D:{
                    for (int i = 0; i < 0x18; i++) this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x1A:
                case 0x0E:
                case 0x16:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x19:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }

        }
        }

    };
    //0x40
   class OPCode40: public Instruction{
    public:
        OPCode40():Instruction(-1,0x40,nullptr){}
        OPCode40(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x40,Maker){}
        OPCode40(int addr, Builder *Maker):Instruction(addr,"???",0x40,Maker){}
        OPCode40(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x40,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };
    //0x41
    class OPCode41: public Instruction{
    public:
        OPCode41():Instruction(-1,0x41,nullptr){}
        OPCode41(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x41,Maker){}
        OPCode41(int addr, Builder *Maker):Instruction(addr,"???",0x41,Maker){}
        OPCode41(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x41,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };

    //0x42
    class OPCode42: public Instruction{
    public:
        OPCode42():Instruction(-1,0x42,nullptr){}
        OPCode42(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x42,Maker){}
        OPCode42(int addr, Builder *Maker):Instruction(addr,"???",0x42,Maker){}
        OPCode42(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x42,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }
    };

    //0x43
    class OPCode43: public Instruction{
    public:
        OPCode43():Instruction(-1,0x43,nullptr){}
        OPCode43(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x43,Maker){}
        OPCode43(int addr, Builder *Maker):Instruction(addr,"???",0x43,Maker){}
        OPCode43(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x43,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }
    };
    //0x44
    class OPCode44: public Instruction{
    public:
        OPCode44():Instruction(-1,0x44,nullptr){}
        OPCode44(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x44,Maker){}
        OPCode44(int addr, Builder *Maker):Instruction(addr,"???",0x44,Maker){}
        OPCode44(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x44,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch((unsigned char)control_byte[0]){

                case 0x00:{

                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x03:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x04:
                case 0x05:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;}
                case 0x06:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x07:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x08:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }


            }
        }

    };

    //0x45
    class OPCode45: public Instruction{
    public:
        OPCode45():Instruction(-1,0x45,nullptr){}
        OPCode45(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x45,Maker){}
        OPCode45(int addr, Builder *Maker):Instruction(addr,"???",0x45,Maker){}
        OPCode45(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x45,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }
    };
    class OPCode46: public Instruction{
        public:
            OPCode46():Instruction(-1,0x46,nullptr){}
            OPCode46(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x46,Maker){}
            OPCode46(int addr, Builder *Maker):Instruction(addr,"???",0x46,Maker){}
            OPCode46(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x46,Maker){
                addr++;
                QByteArray control_byte = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr,"byte", control_byte));
                switch((unsigned char)control_byte[0]){
                    case 0:{
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 3:
                    case 1:{
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 2:{
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                }
            }
        };



    //0x47
    /*class OPCode47: public Instruction{
    public:
        OPCode47():Instruction(-1,0x47,nullptr){}
        OPCode47(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x47,Maker){}
        OPCode47(int addr, Builder *Maker):Instruction(addr,"???",0x47,Maker){}
        OPCode47(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x47,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };*/
    //0x47
    class OPCode47: public Instruction{
    public:
        OPCode47():Instruction(-1,0x47,nullptr){}
        OPCode47(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x47,Maker){}
        OPCode47(int addr, Builder *Maker):Instruction(addr,"???",0x47,Maker){}
        OPCode47(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x47,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x48
    class OPCode48: public Instruction{
    public:
        OPCode48():Instruction(-1,0x48,nullptr){}
        OPCode48(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x48,Maker){}
        OPCode48(int addr, Builder *Maker):Instruction(addr,"???",0x48,Maker){}
        OPCode48(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x48,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

           }

    };
    //0x49
    /*class OPCode49: public Instruction{
    public:
        OPCode49():Instruction(-1,0x49,nullptr){}
        OPCode49(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x49,Maker){}
        OPCode49(int addr, Builder *Maker):Instruction(addr,"???",0x49,Maker){}
        OPCode49(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x49,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){

            case 0x0A:
            case 0x05:
            case 0x06:
            case 0x04:
            case 0x03:
            case 0x02:
            case 0x01:
            case 0x00:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '\r':{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x14:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x15:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


                break;
            }
            case 0xE:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x17:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0x18:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x19:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x1C:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '!':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '(':
            case '$':
            case '#':
            case '\v':
            case '3':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '&':{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case ')':{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '+':{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case '4':
            case '0':
            case ',':{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '5':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case '7':{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case '8':{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case '9':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case ':':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case ';':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '<':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '=':{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case '>':{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 'D':
            case 'B':
            case '@':{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

        }
        }

    };*/
    class OPCode49: public Instruction{
        public:
            OPCode49():Instruction(-1,0x49,nullptr){}
            OPCode49(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x49,Maker){}
            OPCode49(int addr, Builder *Maker):Instruction(addr,"???",0x49,Maker){}
            OPCode49(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x49,Maker){
                addr++;
                QByteArray control_byte = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr,"byte", control_byte));
                switch((unsigned char)control_byte[0]){

                    case 0x00:{
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x01:{

                        QByteArray str = ReadStringSubByteArray(content, addr);
                        this->AddOperande(operande(addr,"string", str));

                        QByteArray remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);
                        operande fill = operande(addr,"fill", remaining);
                        fill.setBytesToFill(0x20);
                        this->AddOperande(fill);
                        break;
                    }
                    case 0x0A:
                    case 0x04:
                    case 0x02:
                    case 0x03:{ //Here, not entirely sure, doesn't seem to be used tho. Example I found has 0x20 zeros.
                        QByteArray str = ReadStringSubByteArray(content, addr);
                        this->AddOperande(operande(addr,"string", str));

                        QByteArray remaining = ReadSubByteArray(content, addr, 0x20-str.size()-1);
                        operande fill = operande(addr,"fill", remaining);
                        fill.setBytesToFill(0x20);
                        this->AddOperande(fill);
                        break;
                    }
                    case 0x0B:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x0D:
                    {
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x0E:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    }
                    case 0x14:
                    {
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));


                        break;
                    }
                    case 0x15:
                    {
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                        break;
                    }
                    case 0x17:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x18:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    }
                    case 0x19:
                    {
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x1A:
                    case 0x1B:
                    case 0x3D:
                    {
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x1C:
                    {
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x21:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x28:
                    case 0x24:
                    case 0x23:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x29:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        qFatal("not sure here");
                        break;
                    }
                    case 0x2B:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        qFatal("not sure here");
                        break;
                    }
                    case 0x34:
                    case 0x30:
                    {
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x33:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x35:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x38:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                        break;
                    }
                    case 0x40:
                    case 0x39:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                        break;
                    }
                    case 0x3A:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                        break;
                    }
                    case 0x3B:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,1)));


                        break;
                    }
                    case 0x3C:
                    {

                        qFatal("not sure here");

                        break;
                    }
                    case 0x3E:
                    {

                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,1)));

                        break;
                    }
                    case 0x44:
                    {

                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                        break;
                    }
                    case 0x45:
                    case 0x46:
                    {

                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                        break;
                    }
                    case 0x47:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x48:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                        break;
                    }



                }

            }

        };

    //0x4A
    class OPCode4A: public Instruction{
    public:
        OPCode4A():Instruction(-1,0x4A,nullptr){}
        OPCode4A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4A,Maker){}
        OPCode4A(int addr, Builder *Maker):Instruction(addr,"???",0x4A,Maker){}
        OPCode4A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4A,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

        }

    };
    //0x4B
    class OPCode4B: public Instruction{
    public:
        OPCode4B():Instruction(-1,0x4B,nullptr){}
        OPCode4B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4B,Maker){}
        OPCode4B(int addr, Builder *Maker):Instruction(addr,"???",0x4B,Maker){}
        OPCode4B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4B,Maker){
        addr++;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x4C
    class OPCode4C: public Instruction{
    public:
        OPCode4C():Instruction(-1,0x4C,nullptr){}
        OPCode4C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4C,Maker){}
        OPCode4C(int addr, Builder *Maker):Instruction(addr,"???",0x4C,Maker){}
        OPCode4C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4C,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

        }

    };
    //0x4D
    class OPCode4D: public Instruction{
    public:
        OPCode4D():Instruction(-1,0x4D,nullptr){}
        OPCode4D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4D,Maker){}
        OPCode4D(int addr, Builder *Maker):Instruction(addr,"???",0x4D,Maker){}
        OPCode4D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4D,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        }

    };
    //0x4F
    /*class OPCode4F: public Instruction{
    public:
        OPCode4F():Instruction(-1,0x4F,nullptr){}
        OPCode4F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4F,Maker){}
        OPCode4F(int addr, Builder *Maker):Instruction(addr,"???",0x4F,Maker){}
        OPCode4F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4F,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
        }

    };*/
    //0x4F
    class OPCode4F: public Instruction{
    public:
        OPCode4F():Instruction(-1,0x4F,nullptr){}
        OPCode4F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x4F,Maker){}
        OPCode4F(int addr, Builder *Maker):Instruction(addr,"???",0x4F,Maker){}
        OPCode4F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x4F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x50
    class OPCode50: public Instruction{
    public:
        OPCode50():Instruction(-1,0x50,nullptr){}
        OPCode50(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x50,Maker){}
        OPCode50(int addr, Builder *Maker):Instruction(addr,"???",0x50,Maker){}
        OPCode50(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x50,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            sub05(addr, content, this);
       }

    };
    //0x51
    class OPCode51: public Instruction{
        public:
            OPCode51():Instruction(-1,0x51,nullptr){}
            OPCode51(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x51,Maker){}
            OPCode51(int addr, Builder *Maker):Instruction(addr,"???",0x51,Maker){}
            OPCode51(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x51,Maker){
                addr++;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            }

    };
    //0x52
    class OPCode52: public Instruction{
    public:
        OPCode52():Instruction(-1,0x52,nullptr){}
        OPCode52(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x52,Maker){}
        OPCode52(int addr, Builder *Maker):Instruction(addr,"???",0x52,Maker){}
        OPCode52(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x52,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x53
    class OPCode53: public Instruction{
    public:
        OPCode53():Instruction(-1,0x53,nullptr){}
        OPCode53(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x53,Maker){}
        OPCode53(int addr, Builder *Maker):Instruction(addr,"???",0x53,Maker){}
        OPCode53(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x53,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            if ((unsigned char) control_byte[0] == 0){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
        }

    };
    //0x55
    class OPCode55: public Instruction{
    public:
        OPCode55():Instruction(-1,0x55,nullptr){}
        OPCode55(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x55,Maker){}
        OPCode55(int addr, Builder *Maker):Instruction(addr,"???",0x55,Maker){}
        OPCode55(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x55,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


       }

    };
    //0x56
    class OPCode56: public Instruction{
    public:
        OPCode56():Instruction(-1,0x56,nullptr){}
        OPCode56(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x56,Maker){}
        OPCode56(int addr, Builder *Maker):Instruction(addr,"???",0x56,Maker){}
        OPCode56(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x56,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x57
    class OPCode57: public Instruction{
    public:
        OPCode57():Instruction(-1,0x57,nullptr){}
        OPCode57(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x57,Maker){}
        OPCode57(int addr, Builder *Maker):Instruction(addr,"???",0x57,Maker){}
        OPCode57(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x57,Maker){
            addr++;
        }

    };
    //0x58
    class OPCode58: public Instruction{
    public:
        OPCode58():Instruction(-1,0x58,nullptr){}
        OPCode58(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x58,Maker){}
        OPCode58(int addr, Builder *Maker):Instruction(addr,"???",0x58,Maker){}
        OPCode58(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x58,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };
    //0x59
    /*class OPCode59: public Instruction{
    public:
        OPCode59():Instruction(-1,0x59,nullptr){}
        OPCode59(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x59,Maker){}
        OPCode59(int addr, Builder *Maker):Instruction(addr,"???",0x59,Maker){}
        OPCode59(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x59,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };*/
    //0x5A
    class OPCode5A: public Instruction{
    public:
        OPCode5A():Instruction(-1,0x5A,nullptr){}
        OPCode5A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5A,Maker){}
        OPCode5A(int addr, Builder *Maker):Instruction(addr,"???",0x5A,Maker){}
        OPCode5A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            switch((unsigned char)control_byte[0]){
                case 0x01:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
            }
        }

    };
    //0x5B
    class OPCode5B: public Instruction{
    public:
        OPCode5B():Instruction(-1,0x5B,nullptr){}
        OPCode5B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5B,Maker){}
        OPCode5B(int addr, Builder *Maker):Instruction(addr,"???",0x5B,Maker){}
        OPCode5B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5B,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch((unsigned char)control_byte[0]){
                case 0x06:
                case 0x04:
                case 0x01:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
            }
        }

    };
    //0x5D
   /* class OPCode5D: public Instruction{
    public:
        OPCode5D():Instruction(-1,0x5D,nullptr){}
        OPCode5D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5D,Maker){}
        OPCode5D(int addr, Builder *Maker):Instruction(addr,"???",0x5D,Maker){}
        OPCode5D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5D,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        switch((unsigned char)control_byte[0]){
            case 0x01:
            case 0x00:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
        }
        }

    };*/
    //0x5E
    class OPCode5D: public Instruction{
    public:
        OPCode5D():Instruction(-1,0x5D,nullptr){}
        OPCode5D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5D,Maker){}
        OPCode5D(int addr, Builder *Maker):Instruction(addr,"???",0x5D,Maker){}
        OPCode5D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5D,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            switch((unsigned char)control_byte[0]){
                case 0x01:
                case 0x00:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x04:
                case 0x03:
                case 0x02:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }



                case '\a':{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case '\b':{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case '\v':{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x0D:
                case 0x0C:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                    break;
                }
                case 0x0E:
               {
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x0F:
               {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x10:
               {
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x11:
               {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x12:
               {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x14:
               {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x15:
               {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x16:
               {
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x17:
               {
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                    break;
                }
                case 0x18:
               {
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }


            }
        }

    };
    //0x5E
    class OPCode5E: public Instruction{
    public:
        OPCode5E():Instruction(-1,0x5E,nullptr){}
        OPCode5E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5E,Maker){}
        OPCode5E(int addr, Builder *Maker):Instruction(addr,"???",0x5E,Maker){}
        OPCode5E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5E,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x03:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                    break;
                }

                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x0C:
                case 0x0B:
                case 0x0A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x04:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));;
                    break;
                }

                case 0x05:
                case 0x06:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
            }
        }

    };

    //0x5F
    /*class OPCode5F: public Instruction{
    public:
        OPCode5F():Instruction(-1,0x5F,nullptr){}
        OPCode5F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5F,Maker){}
        OPCode5F(int addr, Builder *Maker):Instruction(addr,"???",0x5F,Maker){}
        OPCode5F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5F,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0B:
                case 0x09:
                case 0x07:
                case 0x08:
                case 0x04:
                case 0x13:
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x05:
                case 0x03:
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x06:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
                case 0x0E:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));;
                    break;
                }
                case 0x0D:
                case 0x0C:
                case 0x0F:
                case 0x12:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
                case 0x10:
                case 0x11:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }


            }
        }

    };*/
    //0x5F
    class OPCode5F: public Instruction{
    public:
        OPCode5F():Instruction(-1,0x5F,nullptr){}
        OPCode5F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x5F,Maker){}
        OPCode5F(int addr, Builder *Maker):Instruction(addr,"???",0x5F,Maker){}
        OPCode5F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:
                case 0x03:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x04:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                 }
                case 0x05:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                 }

            }
        }

    };
    //0x61
    /*class OPCode61: public Instruction{
    public:
        OPCode61():Instruction(-1,0x61,nullptr){}
        OPCode61(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x61,Maker){}
        OPCode61(int addr, Builder *Maker):Instruction(addr,"???",0x61,Maker){}
        OPCode61(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x61,Maker){
            addr++;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };*/
    //0x60
    class OPCode60: public Instruction{
    public:
        OPCode60():Instruction(-1,0x60,nullptr){}
        OPCode60(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x60,Maker){}
        OPCode60(int addr, Builder *Maker):Instruction(addr,"???",0x60,Maker){}
        OPCode60(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x60,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }
    };
    //0x61
    class OPCode61: public Instruction{
    public:
        OPCode61():Instruction(-1,0x61,nullptr){}
        OPCode61(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x61,Maker){}
        OPCode61(int addr, Builder *Maker):Instruction(addr,"???",0x61,Maker){}
        OPCode61(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x61,Maker){
            addr++;


            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x63
    class OPCode63: public Instruction{
    public:
        OPCode63():Instruction(-1,0x63,nullptr){}
        OPCode63(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x63,Maker){}
        OPCode63(int addr, Builder *Maker):Instruction(addr,"???",0x63,Maker){}
        OPCode63(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x63,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };
    //0x64
    class OPCode64: public Instruction{
    public:
        OPCode64():Instruction(-1,0x64,nullptr){}
        OPCode64(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x64,Maker){}
        OPCode64(int addr, Builder *Maker):Instruction(addr,"???",0x64,Maker){}
        OPCode64(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x64,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            switch((unsigned char)control_byte[0]){
                case 0x11:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x14:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x16:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
            }

        }

    };
    //0x65
    class OPCode65: public Instruction{
    public:
        OPCode65():Instruction(-1,0x65,nullptr){}
        OPCode65(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x65,Maker){}
        OPCode65(int addr, Builder *Maker):Instruction(addr,"???",0x65,Maker){}
        OPCode65(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x65,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){


            case 0x00:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x09:
            case 0x04:
            case 0x0C:
            {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                break;
            }
            case 0x08:
            case 0x07:
            case 0x0B:
            case 0x01:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                break;
            }
            case 0x02:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }

            case 0x05:
            case 0x03:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            case 0x06:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            }

        }

        }

    };

    //0x66
    class OPCode66: public Instruction{
    public:
        OPCode66():Instruction(-1,0x66,nullptr){}
        OPCode66(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x66,Maker){}
        OPCode66(int addr, Builder *Maker):Instruction(addr,"???",0x66,Maker){}
        OPCode66(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x66,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };
    //0x68
    class OPCode67: public Instruction{
    public:
        OPCode67():Instruction(-1,0x67,nullptr){}
        OPCode67(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x67,Maker){}
        OPCode67(int addr, Builder *Maker):Instruction(addr,"???",0x67,Maker){}
        OPCode67(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x67,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            switch(code){
                case 0x02:
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                    break;
                }
                case 0x03:
                case 0x04:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                    break;
                }
            }


        }

    };
    //0x68
    class OPCode68: public Instruction{
    public:
        OPCode68():Instruction(-1,0x68,nullptr){}
        OPCode68(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x68,Maker){}
        OPCode68(int addr, Builder *Maker):Instruction(addr,"???",0x68,Maker){}
        OPCode68(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x68,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
            case 0x01:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                break;
            }
            case 0x00:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));


                break;
            }
        }
        }

    };
    //0x69
    class OPCode69: public Instruction{
    public:
        OPCode69():Instruction(-1,0x69,nullptr){}
        OPCode69(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x69,Maker){}
        OPCode69(int addr, Builder *Maker):Instruction(addr,"???",0x69,Maker){}
        OPCode69(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x69,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
            case 0x00:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }

            case 0x02:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x03:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x14:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }



        }
        }

    };
    //0x6A
   class OPCode6A: public Instruction{
        public:
            OPCode6A():Instruction(-1,0x6A,nullptr){}
            OPCode6A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6A,Maker){}
            OPCode6A(int addr, Builder *Maker):Instruction(addr,"???",0x6A,Maker){}
            OPCode6A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte2));

            if ((unsigned char)control_byte2[0] < 4){
                switch ((unsigned char)control_byte[0])  {
                    case 0x00:{
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                        break;
                    }

                    case 0x01:{
                        //reading_dialog(addr, content, this);
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x02:{

                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
                        break;
                    }
                    case 0x04:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

                        break;
                    }
                    case 0x05:{
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                        break;
                    }
                    case 0x09:
                    case 0x07:
                    case 0x06:{
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }



                }
            }

        }

    };
    //0x6B
    class OPCode6B: public Instruction{
    public:
        OPCode6B():Instruction(-1,0x6B,nullptr){}
        OPCode6B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6B,Maker){}
        OPCode6B(int addr, Builder *Maker):Instruction(addr,"???",0x6B,Maker){}
        OPCode6B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6B,Maker){
        addr++;
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));;
        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };
    //0x6C
    class OPCode6C: public Instruction{
    public:
        OPCode6C():Instruction(-1,0x6C,nullptr){}
        OPCode6C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6C,Maker){}
        OPCode6C(int addr, Builder *Maker):Instruction(addr,"???",0x6C,Maker){}
        OPCode6C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6C,Maker){
        addr++;

        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };
    //0x6D
    class OPCode6D: public Instruction{
    public:
        OPCode6D():Instruction(-1,0x6D,nullptr){}
        OPCode6D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6D,Maker){}
        OPCode6D(int addr, Builder *Maker):Instruction(addr,"???",0x6D,Maker){}
        OPCode6D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6D,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        switch((unsigned char)control_byte[0]){
            case 0x01:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x02:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x03:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
        }
        }

    };
    //0x6E
    class OPCode6E: public Instruction{
    public:
        OPCode6E():Instruction(-1,0x6E,nullptr){}
        OPCode6E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6E,Maker){}
        OPCode6E(int addr, Builder *Maker):Instruction(addr,"???",0x6E,Maker){}
        OPCode6E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6E,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        switch((unsigned char)control_byte[0]){
            case 0x05:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x0C:{
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            }
        }
        }

    };
    //0x6F
    class OPCode6F: public Instruction{
        public:
        OPCode6F():Instruction(-1,0x6F,nullptr){}
        OPCode6F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x6F,Maker){}
        OPCode6F(int addr, Builder *Maker):Instruction(addr,"???",0x6F,Maker){}
        OPCode6F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6F,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        switch((unsigned char)control_byte[0]){
            case 0x00:{

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x02:
            case 0x01:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
        }
        }

    };
    //0x70
    class OPCode70: public Instruction{
        public:
        OPCode70():Instruction(-1,0x70,nullptr){}
        OPCode70(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x70,Maker){}
        OPCode70(int addr, Builder *Maker):Instruction(addr,"???",0x70,Maker){}
        OPCode70(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x70,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        unsigned char code = control_byte[0];
        if (code == 0x01){
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }
        }

    };
    //0x71
    /*class OPCode72: public Instruction{
    public:
        OPCode72():Instruction(-1,0x72,nullptr){}
        OPCode72(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x72,Maker){}
        OPCode72(int addr, Builder *Maker):Instruction(addr,"???",0x72,Maker){}
        OPCode72(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x72,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        unsigned char code = control_byte[0];
        if (code == 0x00){
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
        }

    };*/
    //0x72
    class OPCode72: public Instruction{
    public:
        OPCode72():Instruction(-1,0x72,nullptr){}
        OPCode72(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x72,Maker){}
        OPCode72(int addr, Builder *Maker):Instruction(addr,"???",0x72,Maker){}
        OPCode72(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x72,Maker){
        addr++;
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }

    };
    //0x73

   class OPCode73: public Instruction{
    public:
        OPCode73():Instruction(-1,0x73,nullptr){}
        OPCode73(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x73,Maker){}
        OPCode73(int addr, Builder *Maker):Instruction(addr,"???",0x73,Maker){}
        OPCode73(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x73,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        switch((unsigned char)control_byte[0]){
            case 0x03:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case 0xFF:
            case 0x06:
            case 0x02:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            }
            case 0x04:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x05:{
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x07:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0xFD:
            case 0xFE:
            case 0xFC:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }
            case 0x00:
            case 0x01:
            case 0x08:
            case 0x09:{
                break;}
            default:{
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                break;
            }



        }
        }

    };
    //0x74
    class OPCode74: public Instruction{
    public:
        OPCode74():Instruction(-1,0x74,nullptr){}
        OPCode74(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x74,Maker){}
        OPCode74(int addr, Builder *Maker):Instruction(addr,"???",0x74,Maker){}
        OPCode74(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x74,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        switch((unsigned char)control_byte[0]){
            case 0x00:{
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
        }
        }

    };
    //0x75
    class OPCode75: public Instruction{
    public:
        OPCode75():Instruction(-1,0x75,nullptr){}
        OPCode75(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x75,Maker){}
        OPCode75(int addr, Builder *Maker):Instruction(addr,"???",0x75,Maker){}
        OPCode75(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x75,Maker){
        addr++;
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }

    };
    //0x78
    class OPCode78: public Instruction{
        public:
            OPCode78():Instruction(-1,0x78,nullptr){}
            OPCode78(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x78,Maker){}
            OPCode78(int addr, Builder *Maker):Instruction(addr,"???",0x78,Maker){}
            OPCode78(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x78,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

            }

    };
    //0x79

    class OPCode79: public Instruction{
    public:
        OPCode79():Instruction(-1,0x79,nullptr){}
        OPCode79(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x79,Maker){}
        OPCode79(int addr, Builder *Maker):Instruction(addr,"???",0x79,Maker){}
        OPCode79(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x79,Maker){
        addr++;
        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
        switch((unsigned char)control_byte[0]){
            case 0x03:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                return;
                break;
            }
        }
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }

    };
    //0x7A
    class OPCode7A: public Instruction{
    public:
        OPCode7A():Instruction(-1,0x7A,nullptr){}
        OPCode7A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7A,Maker){}
        OPCode7A(int addr, Builder *Maker):Instruction(addr,"???",0x7A,Maker){}
        OPCode7A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7A,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }

    };
    //0x7B
    /*class OPCode7B: public Instruction{
    public:
        OPCode7B():Instruction(-1,0x7B,nullptr){}
        OPCode7B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7B,Maker){}
        OPCode7B(int addr, Builder *Maker):Instruction(addr,"???",0x7B,Maker){}
        OPCode7B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7B,Maker){
            addr++;

            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }

    };*/

    //0x7B
    class OPCode7B: public Instruction{
    public:
        OPCode7B():Instruction(-1,0x7B,nullptr){}
        OPCode7B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7B,Maker){}
        OPCode7B(int addr, Builder *Maker):Instruction(addr,"???",0x7B,Maker){}
        OPCode7B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7B,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


        }

    };
    //0x7D
    class OPCode7D: public Instruction{
    public:
        OPCode7D():Instruction(-1,0x7D,nullptr){}
        OPCode7D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7D,Maker){}
        OPCode7D(int addr, Builder *Maker):Instruction(addr,"???",0x7D,Maker){}
        OPCode7D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7D,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));


        }

    };
    //0x7E
    class OPCode7E: public Instruction{
    public:
        OPCode7E():Instruction(-1,0x7E,nullptr){}
        OPCode7E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7E,Maker){}
        OPCode7E(int addr, Builder *Maker):Instruction(addr,"???",0x7E,Maker){}
        OPCode7E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }

    };
    //0x7F
    class OPCode7F: public Instruction{
    public:
        OPCode7F():Instruction(-1,0x7F,nullptr){}
        OPCode7F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x7F,Maker){}
        OPCode7F(int addr, Builder *Maker):Instruction(addr,"???",0x7F,Maker){}
        OPCode7F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch(control_byte[0]){
            case 0x01   :
            case 0x00:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;
            }
            case 0x02:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            }
            case 0x03:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case -2:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            }
            case -1:{

                break;
            }
            default:{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            }


        }
        }
    };
    //0x80
    class OPCode80: public Instruction{
    public:
        OPCode80():Instruction(-1,0x80,nullptr){}
        OPCode80(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x80,Maker){}
        OPCode80(int addr, Builder *Maker):Instruction(addr,"???",0x80,Maker){}
        OPCode80(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x80,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
        }
    };
    //0x81
    class OPCode81: public Instruction{
    public:
        OPCode81():Instruction(-1,0x81,nullptr){}
        OPCode81(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x81,Maker){}
        OPCode81(int addr, Builder *Maker):Instruction(addr,"???",0x81,Maker){}
        OPCode81(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x81,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00) this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            else if (code == 0x03) this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

        }
    };
    //0x82
    /*class OPCode82: public Instruction{
    public:
        OPCode82():Instruction(-1,0x82,nullptr){}
        OPCode82(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x82,Maker){}
        OPCode82(int addr, Builder *Maker):Instruction(addr,"???",0x82,Maker){}
        OPCode82(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x82,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };*/
    //0x82
    class OPCode82: public Instruction{
    public:
        OPCode82():Instruction(-1,0x82,nullptr){}
        OPCode82(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x82,Maker){}
        OPCode82(int addr, Builder *Maker):Instruction(addr,"???",0x82,Maker){}
        OPCode82(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x82,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }
    };
    //0x83
    class OPCode83: public Instruction{
    public:
        OPCode83():Instruction(-1,0x83,nullptr){}
        OPCode83(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x83,Maker){}
        OPCode83(int addr, Builder *Maker):Instruction(addr,"???",0x83,Maker){}
        OPCode83(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x83,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));


       }
    };
    //0x84
    class OPCode84: public Instruction{
    public:
        OPCode84():Instruction(-1,0x84,nullptr){}
        OPCode84(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x84,Maker){}
        OPCode84(int addr, Builder *Maker):Instruction(addr,"???",0x84,Maker){}
        OPCode84(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x84,Maker){
            addr++;


       }
    };
    //0x85
    class OPCode85: public Instruction{
    public:
        OPCode85():Instruction(-1,0x85,nullptr){}
        OPCode85(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x85,Maker){}
        OPCode85(int addr, Builder *Maker):Instruction(addr,"???",0x85,Maker){}
        OPCode85(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x85,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }
    };
    //0x86
    class OPCode86: public Instruction{
    public:
        OPCode86():Instruction(-1,0x86,nullptr){}
        OPCode86(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x86,Maker){}
        OPCode86(int addr, Builder *Maker):Instruction(addr,"???",0x86,Maker){}
        OPCode86(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x86,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((unsigned char)control_byte[0])  {
                case 0x01:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x04:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x0A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x0B:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x0C:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
                case 0x63:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x64:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x3:{

                    break;
                }

            }
        }
    };
    //0x87
    class OPCode87: public Instruction{
    public:
        OPCode87():Instruction(-1,0x87,nullptr){}
        OPCode87(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x87,Maker){}
        OPCode87(int addr, Builder *Maker):Instruction(addr,"???",0x87,Maker){}
        OPCode87(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x87,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x88
    class OPCode88: public Instruction{
    public:
        OPCode88():Instruction(-1,0x88,nullptr){}
        OPCode88(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x88,Maker){}
        OPCode88(int addr, Builder *Maker):Instruction(addr,"???",0x88,Maker){}
        OPCode88(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x88,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00){
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            }


        }
    };

    //0x89
    class OPCode89: public Instruction{
    public:
        OPCode89():Instruction(-1,0x89,nullptr){}
        OPCode89(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x89,Maker){}
        OPCode89(int addr, Builder *Maker):Instruction(addr,"???",0x89,Maker){}
        OPCode89(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x89,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            if ((code == 0x00)||(code == 0x14)){
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            }
            else if (code == 0x0A){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
        }
    };
    //0x8A
    class OPCode8A: public Instruction{
    public:
        OPCode8A():Instruction(-1,0x8A,nullptr){}
        OPCode8A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8A,Maker){}
        OPCode8A(int addr, Builder *Maker):Instruction(addr,"???",0x8A,Maker){}
        OPCode8A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            unsigned char code = control_byte[0];
            if (code == 0x00){
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
        }
    };
    //0x8C
    class OPCode8C: public Instruction{
    public:
        OPCode8C():Instruction(-1,0x8C,nullptr){}
        OPCode8C(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8C,Maker){}
        OPCode8C(int addr, Builder *Maker):Instruction(addr,"???",0x8C,Maker){}
        OPCode8C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8C,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
        }
    };
    //0x8D
    /*class OPCode8D: public Instruction{
    public:
        OPCode8D():Instruction(-1,0x8D,nullptr){}
        OPCode8D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8D,Maker){}
        OPCode8D(int addr, Builder *Maker):Instruction(addr,"???",0x8D,Maker){}
        OPCode8D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8D,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };*/
    //0x8D
    class OPCode8D: public Instruction{
    public:
        OPCode8D():Instruction(-1,0x8D,nullptr){}
        OPCode8D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8D,Maker){}
        OPCode8D(int addr, Builder *Maker):Instruction(addr,"???",0x8D,Maker){}
        OPCode8D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8D,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            short control = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"short", control_short));
            switch (control){
                case 0x02:
                case 0x01:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x03:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
            }
        }
    };
    //0x8E
    class OPCode8E: public Instruction{
    public:
        OPCode8E():Instruction(-1,0x8E,nullptr){}
        OPCode8E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8E,Maker){}
        OPCode8E(int addr, Builder *Maker):Instruction(addr,"???",0x8E,Maker){}
        OPCode8E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x8F
    class OPCode8F: public Instruction{
    public:
        OPCode8F():Instruction(-1,0x8F,nullptr){}
        OPCode8F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x8F,Maker){}
        OPCode8F(int addr, Builder *Maker):Instruction(addr,"???",0x8F,Maker){}
        OPCode8F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x8F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }
    };
    //0x90
    class OPCode90: public Instruction{
    public:
        OPCode90():Instruction(-1,0x90,nullptr){}
        OPCode90(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x90,Maker){}
        OPCode90(int addr, Builder *Maker):Instruction(addr,"???",0x90,Maker){}
        OPCode90(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x90,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                }
            }
        }
    };
    //0x91
    class OPCode91: public Instruction{
    public:
        OPCode91():Instruction(-1,0x91,nullptr){}
        OPCode91(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x91,Maker){}
        OPCode91(int addr, Builder *Maker):Instruction(addr,"???",0x91,Maker){}
        OPCode91(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x91,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
            }
        }
    };

    //0x92
    class OPCode92: public Instruction{
    public:
        OPCode92():Instruction(-1,0x92,nullptr){}
        OPCode92(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x92,Maker){}
        OPCode92(int addr, Builder *Maker):Instruction(addr,"???",0x92,Maker){}
        OPCode92(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x92,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;

        }
    };

    //0x93
    class OPCode93: public Instruction{
    public:
        OPCode93():Instruction(-1,0x93,nullptr){}
        OPCode93(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x93,Maker){}
        OPCode93(int addr, Builder *Maker):Instruction(addr,"???",0x93,Maker){}
        OPCode93(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x93,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray control_byte2 = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte2));
            if ((unsigned char)control_byte2[0] == 1){
                switch((unsigned char)control_byte[0]){
                    case 0x01:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        break;
                    }
                    case 0x03:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    }
                    case 0x05:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    }
                    case 0x08:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x0D:{
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    }
                    case 0x10:{
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        break;
                    }
                    case 0x14:
                    case 0x12:{
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;
                    }
                    case 0x17:{
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                        break;
                    }
                }
            }

        }
    };
    //0x94
    class OPCode94: public Instruction{
    public:
        OPCode94():Instruction(-1,0x94,nullptr){}
        OPCode94(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x94,Maker){}
        OPCode94(int addr, Builder *Maker):Instruction(addr,"???",0x94,Maker){}
        OPCode94(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x94,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }
    };
    //0x95
    class OPCode95: public Instruction{
    public:
        OPCode95():Instruction(-1,0x95,nullptr){}
        OPCode95(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x95,Maker){}
        OPCode95(int addr, Builder *Maker):Instruction(addr,"???",0x95,Maker){}
        OPCode95(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x95,Maker){
            addr++;

        }
    };
    //0x96
    class OPCode96: public Instruction{
    public:
        OPCode96():Instruction(-1,0x96,nullptr){}
        OPCode96(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x96,Maker){}
        OPCode96(int addr, Builder *Maker):Instruction(addr,"???",0x96,Maker){}
        OPCode96(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x96,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };
    //0x97
    class OPCode97: public Instruction{
    public:
        OPCode97():Instruction(-1,0x97,nullptr){}
        OPCode97(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x97,Maker){}
        OPCode97(int addr, Builder *Maker):Instruction(addr,"???",0x97,Maker){}
        OPCode97(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x97,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0x98
    /*class OPCode98: public Instruction{
    public:
        OPCode98():Instruction(-1,0x98,nullptr){}
        OPCode98(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x98,Maker){}
        OPCode98(int addr, Builder *Maker):Instruction(addr,"???",0x98,Maker){}
        OPCode98(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x98,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };*/
    //0x99
    class OPCode99: public Instruction{
    public:
        OPCode99():Instruction(-1,0x99,nullptr){}
        OPCode99(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x99,Maker){}
        OPCode99(int addr, Builder *Maker):Instruction(addr,"???",0x99,Maker){}
        OPCode99(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x99,Maker){
            addr++;

        }
    };
    //0x9A
    class OPCode9A: public Instruction{
    public:
        OPCode9A():Instruction(-1,0x9A,nullptr){}
        OPCode9A(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9A,Maker){}
        OPCode9A(int addr, Builder *Maker):Instruction(addr,"???",0x9A,Maker){}
        OPCode9A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9A,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };
    //0x9B
    /*class OPCode9B: public Instruction{
    public:
        OPCode9B():Instruction(-1,0x9B,nullptr){}
        OPCode9B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9B,Maker){}
        OPCode9B(int addr, Builder *Maker):Instruction(addr,"???",0x9B,Maker){}
        OPCode9B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9B,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x01:
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));


                    break;
                }
                case 0x03:
                case 0x04:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
            }
        }
    };*/
    //0x9B
    class OPCode9B: public Instruction{
        public:
            OPCode9B():Instruction(-1,0x9B, nullptr){}
            OPCode9B(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9B,Maker){}
            OPCode9B(int addr, Builder *Maker):Instruction(addr,"???",0x9B,Maker){}
            OPCode9B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9B,Maker){
                addr++;
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }
    };
    //0x9D
    class OPCode9D: public Instruction{
        public:
            OPCode9D():Instruction(-1,0x9D,nullptr){}
            OPCode9D(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9D,Maker){}
            OPCode9D(int addr, Builder *Maker):Instruction(addr,"???",0x9D,Maker){}
            OPCode9D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9D,Maker){
                addr++;
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
        }
    };
    //0x9E
    class OPCode9E: public Instruction{
        public:
            OPCode9E():Instruction(-1,0x9E,nullptr){}
            OPCode9E(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9E,Maker){}
            OPCode9E(int addr, Builder *Maker):Instruction(addr,"???",0x9E,Maker){}
            OPCode9E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9E,Maker){
                addr++;
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

        }
    };
    //0x9F
    /*class OPCode9F: public Instruction{
    public:
        OPCode9F():Instruction(-1,0x9F,nullptr){}
        OPCode9F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9F,Maker){}
        OPCode9F(int addr, Builder *Maker):Instruction(addr,"???",0x9F,Maker){}
        OPCode9F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9F,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

        }
    };*/
    //0x9F
    class OPCode9F: public Instruction{
    public:
        OPCode9F():Instruction(-1,0x9F,nullptr){}
        OPCode9F(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0x9F,Maker){}
        OPCode9F(int addr, Builder *Maker):Instruction(addr,"???",0x9F,Maker){}
        OPCode9F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x9F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }
    };
    //0xA0
    class OPCodeA0: public Instruction{
    public:
        OPCodeA0():Instruction(-1,0xA0,nullptr){}
        OPCodeA0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA0,Maker){}
        OPCodeA0(int addr, Builder *Maker):Instruction(addr,"???",0xA0,Maker){}
        OPCodeA0(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA0,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:
                case 0x04:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x02:
                case 0x05:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x03:
                {

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }

            }
        }
    };
    //0xA1
    class OPCodeA1: public Instruction{
        public:
            OPCodeA1():Instruction(-1,0xA1,nullptr){}
            OPCodeA1(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA1,Maker){}
            OPCodeA1(int addr, Builder *Maker):Instruction(addr,"???",0xA1,Maker){}
            OPCodeA1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA1,Maker){
                addr++;
                QByteArray control_byte = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr,"byte", control_byte));
                switch((unsigned char)control_byte[0]){
                    case 0x00:
                    {
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                        break;
                    }
                    case 0x03:
                    case 0x04:
                    case 0x02:
                    case 0x01:
                    {
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                        break;
                    }


                }
            }
        };

    //0xA2
    class OPCodeA2: public Instruction{
    public:
        OPCodeA2():Instruction(-1,0xA2,nullptr){}
        OPCodeA2(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA2,Maker){}
        OPCodeA2(int addr, Builder *Maker):Instruction(addr,"???",0xA2,Maker){}
        OPCodeA2(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA2,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
        }
    };
    //0xA3
    /*class OPCodeA3: public Instruction{
    public:
        OPCodeA3():Instruction(-1,0xA3,nullptr){}
        OPCodeA3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA3,Maker){}
        OPCodeA3(int addr, Builder *Maker):Instruction(addr,"???",0xA3,Maker){}
        OPCodeA3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA3,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
        }
    };*/
    //0xA3
    class OPCodeA3: public Instruction{
    public:
        OPCodeA3():Instruction(-1,0xA3,nullptr){}
        OPCodeA3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA3,Maker){}
        OPCodeA3(int addr, Builder *Maker):Instruction(addr,"???",0xA3,Maker){}
        OPCodeA3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA3,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

        }
    };
    //0xA4
    class OPCodeA4: public Instruction{
    public:
        OPCodeA4():Instruction(-1,0xA4,nullptr){}
        OPCodeA4(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA4,Maker){}
        OPCodeA4(int addr, Builder *Maker):Instruction(addr,"???",0xA4,Maker){}
        OPCodeA4(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA4,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

        }
    };
    //0xA6
    /*class OPCodeA6: public Instruction{
    public:
        OPCodeA6():Instruction(-1,0xA6,nullptr){}
        OPCodeA6(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA6,Maker){}
        OPCodeA6(int addr, Builder *Maker):Instruction(addr,"???",0xA6,Maker){}
        OPCodeA6(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA6,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){


                case 0x03:
                case 0x01:
                case 0x04:
                case 0x0E:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }

            }
        }
    };*/
    //0xA6
    class OPCodeA6: public Instruction{
    public:
        OPCodeA6():Instruction(-1,0xA6,nullptr){}
        OPCodeA6(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA6,Maker){}
        OPCodeA6(int addr, Builder *Maker):Instruction(addr,"???",0xA6,Maker){}
        OPCodeA6(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA6,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

        }
    };
    //0xA7
    class OPCodeA7: public Instruction{
    public:
        OPCodeA7():Instruction(-1,0xA7,nullptr){}
        OPCodeA7(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA7,Maker){}
        OPCodeA7(int addr, Builder *Maker):Instruction(addr,"???",0xA7,Maker){}
        OPCodeA7(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA7,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

        }
    };
    //0xA8
    class OPCodeA8: public Instruction{
    public:
        OPCodeA8():Instruction(-1,0xA8,nullptr){}
        OPCodeA8(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA8,Maker){}
        OPCodeA8(int addr, Builder *Maker):Instruction(addr,"???",0xA8,Maker){}
        OPCodeA8(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA8,Maker){
            addr++;

            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

        }
    };
    //0xA9
    class OPCodeA9: public Instruction{
    public:
        OPCodeA9():Instruction(-1,0xA9,nullptr){}
        OPCodeA9(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xA9,Maker){}
        OPCodeA9(int addr, Builder *Maker):Instruction(addr,"???",0xA9,Maker){}
        OPCodeA9(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xA9,Maker){
            addr++;
        }
    };
    //0xAA
    class OPCodeAA: public Instruction{
    public:
        OPCodeAA():Instruction(-1,0xAA,nullptr){}
        OPCodeAA(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAA,Maker){}
        OPCodeAA(int addr, Builder *Maker):Instruction(addr,"???",0xAA,Maker){}
        OPCodeAA(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAA,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        }
    };
    //0xAB
    class OPCodeAB: public Instruction{
    public:
        OPCodeAB():Instruction(-1,0xAB,nullptr){}
        OPCodeAB(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAB,Maker){}
        OPCodeAB(int addr, Builder *Maker):Instruction(addr,"???",0xAB,Maker){}
        OPCodeAB(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAB,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

        }
    };
    //0xAC
    class OPCodeAC: public Instruction{
    public:
        OPCodeAC():Instruction(-1,0xAC,nullptr){}
        OPCodeAC(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAC,Maker){}
        OPCodeAC(int addr, Builder *Maker):Instruction(addr,"???",0xAC,Maker){}
        OPCodeAC(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAC,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));




        }
    };
    //0xAD
    class OPCodeAD: public Instruction{
    public:
        OPCodeAD():Instruction(-1,0xAD,nullptr){}
        OPCodeAD(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAD,Maker){}
        OPCodeAD(int addr, Builder *Maker):Instruction(addr,"???",0xAD,Maker){}
        OPCodeAD(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAD,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
        }
    };
    //0xAE
    class OPCodeAE: public Instruction{
    public:
        OPCodeAE():Instruction(-1,0xAE,nullptr){}
        OPCodeAE(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAE,Maker){}
        OPCodeAE(int addr, Builder *Maker):Instruction(addr,"???",0xAE,Maker){}
        OPCodeAE(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAE,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));


        }
    };
    //0xAF
    class OPCodeAF: public Instruction{
    public:
        OPCodeAF():Instruction(-1,0xAF,nullptr){}
        OPCodeAF(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xAF,Maker){}
        OPCodeAF(int addr, Builder *Maker):Instruction(addr,"???",0xAF,Maker){}
        OPCodeAF(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xAF,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){
                case 0x65:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x66:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x67:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x69:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }
            }


        }
    };
    //0xB0
    /*class OPCodeB0: public Instruction{
    public:
        OPCodeB0():Instruction(-1,0xB0,nullptr){}
        OPCodeB0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB0,Maker){}
        OPCodeB0(int addr, Builder *Maker):Instruction(addr,"???",0xB0,Maker){}
        OPCodeB0(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB0,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                }
            }



        }
    };*/
    //0xB0
    class OPCodeB0: public Instruction{
    public:
        OPCodeB0():Instruction(-1,0xB0,nullptr){}
        OPCodeB0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB0,Maker){}
        OPCodeB0(int addr, Builder *Maker):Instruction(addr,"???",0xB0,Maker){}
        OPCodeB0(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB0,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
        }
    };
    //0xB1
    class OPCodeB1: public Instruction{
    public:
        OPCodeB1():Instruction(-1,0xB1,nullptr){}
        OPCodeB1(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB1,Maker){}
        OPCodeB1(int addr, Builder *Maker):Instruction(addr,"???",0xB1,Maker){}
        OPCodeB1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB1,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x04:
                case 0x03:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x06:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0A:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0D:
                case 0x0C:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0E:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0F:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
            }
        }
    };
    //0xB2
    class OPCodeB2: public Instruction{
    public:
        OPCodeB2():Instruction(-1,0xB2,nullptr){}
        OPCodeB2(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB2,Maker){}
        OPCodeB2(int addr, Builder *Maker):Instruction(addr,"???",0xB2,Maker){}
        OPCodeB2(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB2,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){
                case 0x01:{ //this one is REALLY strange. two times I found that they added two missing floats (or int)
                    //in the files, the executable doesn't parse them right, not sure what to do
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    //these might need to be removed
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x02:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x0A:
                case 0x0B:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                }
                case 0x0C:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

                    break;
                }
                case 0x0D:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x0F:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
                }
                case 0x12:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }



        }
    };

    //0xB3
    /*class OPCodeB3: public Instruction{
    public:
        OPCodeB3():Instruction(-1,0xB3,nullptr){}
        OPCodeB3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB3,Maker){}
        OPCodeB3(int addr, Builder *Maker):Instruction(addr,"???",0xB3,Maker){}
        OPCodeB3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB3,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x03:
                {
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x0A:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }



        }
    };*/
    //0xB3
    class OPCodeB3: public Instruction{
    public:
        OPCodeB3():Instruction(-1,0xB3,nullptr){}
        OPCodeB3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB3,Maker){}
        OPCodeB3(int addr, Builder *Maker):Instruction(addr,"???",0xB3,Maker){}
        OPCodeB3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB3,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));



        }
    };
    //0xB5
    /*class OPCodeB5: public Instruction{
    public:
        OPCodeB5():Instruction(-1,0xB5,nullptr){}
        OPCodeB5(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB5,Maker){}
        OPCodeB5(int addr, Builder *Maker):Instruction(addr,"???",0xB5,Maker){}
        OPCodeB5(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB5,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));



        }
    };*/
    //0xB5
    class OPCodeB5: public Instruction{
    public:
        OPCodeB5():Instruction(-1,0xB5,nullptr){}
        OPCodeB5(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB5,Maker){}
        OPCodeB5(int addr, Builder *Maker):Instruction(addr,"???",0xB5,Maker){}
        OPCodeB5(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB5,Maker){
            addr++;
            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));
            switch(control){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 100:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
            }
        }
    };
    //0xB6
    class OPCodeB6: public Instruction{
    public:
        OPCodeB6():Instruction(-1,0xB6,nullptr){}
        OPCodeB6(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB6,Maker){}
        OPCodeB6(int addr, Builder *Maker):Instruction(addr,"???",0xB6,Maker){}
        OPCodeB6(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB6,Maker){
            addr++;
            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));
            QByteArray control_ba2 = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_ba2));
            switch(control){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x06:
                case 0x05:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
            }
        }
    };
    //0xB7
    class OPCodeB7: public Instruction{
    public:
        OPCodeB7():Instruction(-1,0xB7,nullptr){}
        OPCodeB7(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB7,Maker){}
        OPCodeB7(int addr, Builder *Maker):Instruction(addr,"???",0xB7,Maker){}
        OPCodeB7(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB7,Maker){
            addr++;
            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));
            switch(control){
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                default: {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
            }
        }
    };
    //0xB8
    class OPCodeB8: public Instruction{
    public:
        OPCodeB8():Instruction(-1,0xB8,nullptr){}
        OPCodeB8(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB8,Maker){}
        OPCodeB8(int addr, Builder *Maker):Instruction(addr,"???",0xB8,Maker){}
        OPCodeB8(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB8,Maker){
            addr++;

            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_ba));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));



        }
    };
    //0xB9
    class OPCodeB9: public Instruction{
    public:
        OPCodeB9():Instruction(-1,0xB9,nullptr){}
        OPCodeB9(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xB9,Maker){}
        OPCodeB9(int addr, Builder *Maker):Instruction(addr,"???",0xB9,Maker){}
        OPCodeB9(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xB9,Maker){
            addr++;

            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));

            switch(control){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

                case 0x02:
                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }
                case 0x03:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x04:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x05:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x07:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x09:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x2C:
                case 0x0F:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x14:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));


                    break;
                }
                case 0x15:
                {
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x16:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x18:

                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));


                    break;
                }
                case 0x0A:

                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

                    break;
                }
                case 0x0B:

                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));


                    break;
                }
                case 0x0C:

                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));


                    break;
                }
                case 0x0D:

                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));


                    break;
                }
            case 0x0E:
            {

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                break;
            }

                case 0x2E:
                case 0x1A:
                case 0x1B:
                case 0x17:
                {

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1D:
                case 0x1E:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1F:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x20:
                case 0x24:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x21:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x22:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x23:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x27:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x29:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2A:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x2B:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x2D:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x30:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x31:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x32:
                {

                    break;
                }
                case 0x36:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x34:
                case 0x38:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x39:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x37:
                case 0x3B:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3C:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x3D:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3E:
                {
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x3F:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x19:
                case 0x40:
                case 0x28:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x42:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x41:
                case 0x43:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x44:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x45:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }
                case 0x46:
                case 0x4A:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x3A:
                case 0x48:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x47:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x49:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x54:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                default:{
                    qFatal("not analyzed yet");
                }

            }



        }
    };
    //0xBA
    class OPCodeBA: public Instruction{
    public:
        OPCodeBA():Instruction(-1,0xBA,nullptr){}
        OPCodeBA(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xBA,Maker){}
        OPCodeBA(int addr, Builder *Maker):Instruction(addr,"???",0xBA,Maker){}
        OPCodeBA(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xBA,Maker){
            addr++;

            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));

            switch(control){
                case 0x01:
                case 0x00:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

                case 0x02:

                {
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

                case 0x06:
                {
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x40:
                case 0x41:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }

                case 0x9:
                {
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x0A:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                    break;
                }

            }



        }
    };
    //0xBB
    class OPCodeBB: public Instruction{
    public:
        OPCodeBB():Instruction(-1,0xBB,nullptr){}
        OPCodeBB(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xBB,Maker){}
        OPCodeBB(int addr, Builder *Maker):Instruction(addr,"???",0xBB,Maker){}
        OPCodeBB(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xBB,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

        }
    };
    class OPCodeBC: public Instruction{
    public:
        OPCodeBC():Instruction(-1,0xBC,nullptr){}
        OPCodeBC(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xBC,Maker){}
        OPCodeBC(int addr, Builder *Maker):Instruction(addr,"???",0xBC,Maker){}
        OPCodeBC(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xBC,Maker){
            addr++;

            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            ushort control = ReadShortFromByteArray(0,control_ba);
            this->AddOperande(operande(addr,"short", control_ba));

            switch(control){
                case 0x08:
                case 0x07:
                case 0x2E:
                case 0x3A:
                case 0x3C:
                case 0x3F:
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x01:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x02:
                case 0x3B:
                case 0x3D:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x18:
                case 0x15:
                case 0x14:
                case 0x03:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x40:
                case 0x41:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
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
                case 0x4D:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
                case 0x1E:
                {
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                }
                case 0x2D:
                case 0x44:
                case 0x45:
                case 0x5B:
                case 0x5F:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }
                case 0x35:
                {
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                }

            }



        }
    };
    class OPCodeBE: public Instruction{
    public:
        OPCodeBE():Instruction(-1,0xBE,nullptr){}
        OPCodeBE(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xBE,Maker){}
        OPCodeBE(int addr, Builder *Maker):Instruction(addr,"???",0xBE,Maker){}
        OPCodeBE(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xBE,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            }

    };
    class OPCodeBF: public Instruction{
    public:
        OPCodeBF():Instruction(-1,0xBF,nullptr){}
        OPCodeBF(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xBF,Maker){}
        OPCodeBF(int addr, Builder *Maker):Instruction(addr,"???",0xBF,Maker){}
        OPCodeBF(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xBF,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            }

    };
    class OPCodeC0: public Instruction{
    public:
        OPCodeC0():Instruction(-1,0xC0,nullptr){}
        OPCodeC0(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC0,Maker){}
        OPCodeC0(int addr, Builder *Maker):Instruction(addr,"???",0xC0,Maker){}
        OPCodeC0(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC0,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            }

    };
    class OPCodeC1: public Instruction{
    public:
        OPCodeC1():Instruction(-1,0xC1,nullptr){}
        OPCodeC1(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC1,Maker){}
        OPCodeC1(int addr, Builder *Maker):Instruction(addr,"???",0xC1,Maker){}
        OPCodeC1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC1,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            }

    };
    class OPCodeC2: public Instruction{
    public:
        OPCodeC2():Instruction(-1,0xC2,nullptr){}
        OPCodeC2(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC2,Maker){}
        OPCodeC2(int addr, Builder *Maker):Instruction(addr,"???",0xC2,Maker){}
        OPCodeC2(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC2,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

            }

    };
    class OPCodeC3: public Instruction{
    public:
        OPCodeC3():Instruction(-1,0xC3,nullptr){}
        OPCodeC3(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC3,Maker){}
        OPCodeC3(int addr, Builder *Maker):Instruction(addr,"???",0xC3,Maker){}
        OPCodeC3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC3,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                }
                case 0x02:
                case 0x01:{
                    break;
                }
                default:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                }
            }
            }

    };
    class OPCodeC4: public Instruction{
    public:
        OPCodeC4():Instruction(-1,0xC4,nullptr){}
        OPCodeC4(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC4,Maker){}
        OPCodeC4(int addr, Builder *Maker):Instruction(addr,"???",0xC4,Maker){}
        OPCodeC4(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC4,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

            }

    };
    class OPCodeC5: public Instruction{
    public:
        OPCodeC5():Instruction(-1,0xC5,nullptr){}
        OPCodeC5(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC5,Maker){}
        OPCodeC5(int addr, Builder *Maker):Instruction(addr,"???",0xC5,Maker){}
        OPCodeC5(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC5,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                }

            }
            }

    };
    class OPCodeC7: public Instruction{
    public:
        OPCodeC7():Instruction(-1,0xC7,nullptr){}
        OPCodeC7(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC7,Maker){}
        OPCodeC7(int addr, Builder *Maker):Instruction(addr,"???",0xC7,Maker){}
        OPCodeC7(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC7,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            }

    };
    class OPCodeC8: public Instruction{
    public:
        OPCodeC8():Instruction(-1,0xC8,nullptr){}
        OPCodeC8(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC8,Maker){}
        OPCodeC8(int addr, Builder *Maker):Instruction(addr,"???",0xC8,Maker){}
        OPCodeC8(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC8,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray control_ba = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_ba));

            }

    };
    class OPCodeC9: public Instruction{
    public:
        OPCodeC9():Instruction(-1,0xC9,nullptr){}
        OPCodeC9(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xC9,Maker){}
        OPCodeC9(int addr, Builder *Maker):Instruction(addr,"???",0xC9,Maker){}
        OPCodeC9(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC9,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x00:{
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                }
                case 0x01:{

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                }
            }
            }

    };
    class OPCodeCA: public Instruction{
    public:
        OPCodeCA():Instruction(-1,0xCA,nullptr){}
        OPCodeCA(int &addr, int idx_row, QXlsx::Document &doc,Builder *Maker):Instruction(addr, idx_row, doc,"???", 0xCA,Maker){}
        OPCodeCA(int addr, Builder *Maker):Instruction(addr,"???",0xCA,Maker){}
        OPCodeCA(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xCA,Maker){
            addr++;
            }

    };
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content, int function_type){
        int OP = (dat_content[addr]&0xFF);
        //qDebug() << "OP :" << hex << OP << " at " << addr;
        int i = TXUIFiles.indexOf(SceneName);
        if ((i != -1)&&(OP == 0x13)) return std::make_shared<UI_OP13>(addr,dat_content,this); //UI files have a special 0x13 instruction

        if (function_type == 0){ //the function is read with OPCodes

            switch(OP){
                case 0x00: return std::make_shared<OPCode0>(addr,dat_content,this);
                case 0x01: return std::make_shared<OPCode1>(addr,dat_content,this);
                case 0x02: return std::make_shared<OPCode02>(addr,dat_content,this);
                case 0x03: return std::make_shared<OPCode03>(addr,dat_content,this);
                case 0x04: return std::make_shared<OPCode04>(addr,dat_content,this);
                case 0x05: return std::make_shared<OPCode05>(addr,dat_content,this);
                case 0x06: return std::make_shared<OPCode06>(addr,dat_content,this);
                case 0x07: return std::make_shared<OPCode07>(addr,dat_content,this);
                case 0x08: return std::make_shared<OPCode08>(addr,dat_content,this);
                case 0x0A: return std::make_shared<OPCode0A>(addr,dat_content,this);
                case 0x0C: return std::make_shared<OPCode0C>(addr,dat_content,this);
                case 0x0D: return std::make_shared<OPCode0D>(addr,dat_content,this);
                case 0x0E: return std::make_shared<OPCode0E>(addr,dat_content,this);
                case 0x0F: return std::make_shared<OPCode0F>(addr,dat_content,this);
                case 0x10: return std::make_shared<OPCode10>(addr,dat_content,this);
                case 0x11: return std::make_shared<OPCode11>(addr,dat_content,this);
                case 0x12: return std::make_shared<OPCode12>(addr,dat_content,this);
                case 0x13: return std::make_shared<OPCode13>(addr,dat_content,this);
                case 0x14: return std::make_shared<OPCode14>(addr,dat_content,this);
                case 0x15: return std::make_shared<OPCode15>(addr,dat_content,this);
                case 0x16: return std::make_shared<OPCode16>(addr,dat_content,this);
                case 0x17: return std::make_shared<OPCode17>(addr,dat_content,this);
                case 0x18: return std::make_shared<OPCode18>(addr,dat_content,this);
                case 0x19: return std::make_shared<OPCode19>(addr,dat_content,this);
                case 0x1A: return std::make_shared<OPCode1A>(addr,dat_content,this);
                case 0x1B: return std::make_shared<OPCode1B>(addr,dat_content,this);
                case 0x1C: return std::make_shared<OPCode1C>(addr,dat_content,this);
                case 0x1D: return std::make_shared<OPCode1D>(addr,dat_content,this);
                case 0x1E: return std::make_shared<OPCode1E>(addr,dat_content,this);
                case 0x1F: return std::make_shared<OPCode1F>(addr,dat_content,this);
                case 0x20: return std::make_shared<OPCode20>(addr,dat_content,this);
//                case 0x21: return std::make_shared<OPCode21>(addr,dat_content,this);
                case 0x22: return std::make_shared<OPCode22>(addr,dat_content,this);
                case 0x23: return std::make_shared<OPCode23>(addr,dat_content,this);
                case 0x24: return std::make_shared<OPCode24>(addr,dat_content,this);
                case 0x25: return std::make_shared<OPCode25>(addr,dat_content,this);
                case 0x26: return std::make_shared<OPCode26>(addr,dat_content,this);
                case 0x27: return std::make_shared<OPCode27>(addr,dat_content,this);
                case 0x28: return std::make_shared<OPCode28>(addr,dat_content,this);
                case 0x29: return std::make_shared<OPCode29>(addr,dat_content,this);
                case 0x2A: return std::make_shared<OPCode2A>(addr,dat_content,this);
                case 0x2B: return std::make_shared<OPCode2B>(addr,dat_content,this);
                case 0x2C: return std::make_shared<OPCode2C>(addr,dat_content,this);
                case 0x2D: return std::make_shared<OPCode2D>(addr,dat_content,this);
                case 0x2E: return std::make_shared<OPCode2E>(addr,dat_content,this);
                case 0x2F: return std::make_shared<OPCode2F>(addr,dat_content,this);
                case 0x30: return std::make_shared<OPCode30>(addr,dat_content,this);
                case 0x31: return std::make_shared<OPCode31>(addr,dat_content,this);
                case 0x32: return std::make_shared<OPCode32>(addr,dat_content,this);
                case 0x33: return std::make_shared<OPCode33>(addr,dat_content,this);
                case 0x34: return std::make_shared<OPCode34>(addr,dat_content,this);
                case 0x35: return std::make_shared<OPCode35>(addr,dat_content,this);
                case 0x36: return std::make_shared<OPCode36>(addr,dat_content,this);
                case 0x37: return std::make_shared<OPCode37>(addr,dat_content,this);
                case 0x38: return std::make_shared<OPCode38>(addr,dat_content,this);
                case 0x39: return std::make_shared<OPCode39>(addr,dat_content,this);
                case 0x3A: return std::make_shared<OPCode3A>(addr,dat_content,this);
                case 0x3B: return std::make_shared<OPCode3B>(addr,dat_content,this);
                case 0x3C: return std::make_shared<OPCode3C>(addr,dat_content,this);
                case 0x3D: return std::make_shared<OPCode3D>(addr,dat_content,this);
                case 0x3E: return std::make_shared<OPCode3E>(addr,dat_content,this);
                case 0x3F: return std::make_shared<OPCode3F>(addr,dat_content,this);
                case 0x40: return std::make_shared<OPCode40>(addr,dat_content,this);
                case 0x41: return std::make_shared<OPCode41>(addr,dat_content,this);
                case 0x42: return std::make_shared<OPCode42>(addr,dat_content,this);
                case 0x43: return std::make_shared<OPCode43>(addr,dat_content,this);
                case 0x44: return std::make_shared<OPCode44>(addr,dat_content,this);
                case 0x45: return std::make_shared<OPCode45>(addr,dat_content,this);
                case 0x46: return std::make_shared<OPCode46>(addr,dat_content,this);
                case 0x47: return std::make_shared<OPCode47>(addr,dat_content,this);
                case 0x48: return std::make_shared<OPCode48>(addr,dat_content,this);
                case 0x49: return std::make_shared<OPCode49>(addr,dat_content,this);
                case 0x4A: return std::make_shared<OPCode4A>(addr,dat_content,this);
                case 0x4B: return std::make_shared<OPCode4B>(addr,dat_content,this);
                case 0x4C: return std::make_shared<OPCode4C>(addr,dat_content,this);
                case 0x4D: return std::make_shared<OPCode4D>(addr,dat_content,this);
//                case 0x4E: return std::make_shared<OPCode4E>(addr,dat_content,this);
                case 0x4F: return std::make_shared<OPCode4F>(addr,dat_content,this);
                case 0x50: return std::make_shared<OPCode50>(addr,dat_content,this);
                case 0x51: return std::make_shared<OPCode51>(addr,dat_content,this);
                case 0x52: return std::make_shared<OPCode52>(addr,dat_content,this);
                case 0x53: return std::make_shared<OPCode53>(addr,dat_content,this);
//                case 0x54: return std::make_shared<OPCode54>(addr,dat_content,this);
                case 0x55: return std::make_shared<OPCode55>(addr,dat_content,this);
                case 0x56: return std::make_shared<OPCode56>(addr,dat_content,this);
                case 0x57: return std::make_shared<OPCode57>(addr,dat_content,this);
                case 0x58: return std::make_shared<OPCode58>(addr,dat_content,this);
//                case 0x59: return std::make_shared<OPCode59>(addr,dat_content,this);
                case 0x5A: return std::make_shared<OPCode5A>(addr,dat_content,this);
                case 0x5B: return std::make_shared<OPCode5B>(addr,dat_content,this);
//                case 0x5C: return std::make_shared<OPCode5C>(addr,dat_content,this);
                case 0x5D: return std::make_shared<OPCode5D>(addr,dat_content,this);
                case 0x5E: return std::make_shared<OPCode5E>(addr,dat_content,this);
                case 0x5F: return std::make_shared<OPCode5F>(addr,dat_content,this);
                case 0x60: return std::make_shared<OPCode60>(addr,dat_content,this);
                case 0x61: return std::make_shared<OPCode61>(addr,dat_content,this);
                //case 0x62: return std::make_shared<OPCode62>(addr,dat_content,this);
                case 0x63: return std::make_shared<OPCode63>(addr,dat_content,this);
                case 0x64: return std::make_shared<OPCode64>(addr,dat_content,this);
                case 0x65: return std::make_shared<OPCode65>(addr,dat_content,this);
                case 0x66: return std::make_shared<OPCode66>(addr,dat_content,this);
                case 0x67: return std::make_shared<OPCode67>(addr,dat_content,this);
                case 0x68: return std::make_shared<OPCode68>(addr,dat_content,this);
                case 0x69: return std::make_shared<OPCode69>(addr,dat_content,this);
                case 0x6A: return std::make_shared<OPCode6A>(addr,dat_content,this);
                case 0x6B: return std::make_shared<OPCode6B>(addr,dat_content,this);
                case 0x6C: return std::make_shared<OPCode6C>(addr,dat_content,this);
                case 0x6D: return std::make_shared<OPCode6D>(addr,dat_content,this);
                case 0x6E: return std::make_shared<OPCode6E>(addr,dat_content,this);
                case 0x6F: return std::make_shared<OPCode6F>(addr,dat_content,this);
                case 0x70: return std::make_shared<OPCode70>(addr,dat_content,this);
//                case 0x71: return std::make_shared<OPCode71>(addr,dat_content,this);
                case 0x72: return std::make_shared<OPCode72>(addr,dat_content,this);
                case 0x73: return std::make_shared<OPCode73>(addr,dat_content,this);
                case 0x74: return std::make_shared<OPCode74>(addr,dat_content,this);
                case 0x75: return std::make_shared<OPCode75>(addr,dat_content,this);
//                case 0x76: return std::make_shared<OPCode76>(addr,dat_content,this);
//                case 0x78: return std::make_shared<OPCode78>(addr,dat_content,this);
                case 0x79: return std::make_shared<OPCode79>(addr,dat_content,this);
//                case 0x7A: return std::make_shared<OPCode7A>(addr,dat_content,this);
                case 0x7B: return std::make_shared<OPCode7B>(addr,dat_content,this);
//                case 0x7C: return std::make_shared<OPCode7C>(addr,dat_content,this);
                case 0x7D: return std::make_shared<OPCode7D>(addr,dat_content,this);
                case 0x7E: return std::make_shared<OPCode7E>(addr,dat_content,this);
                case 0x7F: return std::make_shared<OPCode7F>(addr,dat_content,this);
                case 0x80: return std::make_shared<OPCode80>(addr,dat_content,this);
                case 0x81: return std::make_shared<OPCode81>(addr,dat_content,this);
                case 0x82: return std::make_shared<OPCode82>(addr,dat_content,this);
                case 0x83: return std::make_shared<OPCode83>(addr,dat_content,this);
                case 0x84: return std::make_shared<OPCode84>(addr,dat_content,this);
                case 0x85: return std::make_shared<OPCode85>(addr,dat_content,this);
                case 0x86: return std::make_shared<OPCode86>(addr,dat_content,this);
                case 0x87: return std::make_shared<OPCode87>(addr,dat_content,this);
                case 0x88: return std::make_shared<OPCode88>(addr,dat_content,this);
                case 0x89: return std::make_shared<OPCode89>(addr,dat_content,this);
                case 0x8A: return std::make_shared<OPCode8A>(addr,dat_content,this);
//                case 0x8B: return std::make_shared<OPCode8B>(addr,dat_content,this);
                case 0x8C: return std::make_shared<OPCode8C>(addr,dat_content,this);
                case 0x8D: return std::make_shared<OPCode8D>(addr,dat_content,this);
                case 0x8E: return std::make_shared<OPCode8E>(addr,dat_content,this);
                case 0x8F: return std::make_shared<OPCode8F>(addr,dat_content,this);
                case 0x90: return std::make_shared<OPCode90>(addr,dat_content,this);
//                case 0x91: return std::make_shared<OPCode91>(addr,dat_content,this);
                case 0x92: return std::make_shared<OPCode92>(addr,dat_content,this);
                case 0x93: return std::make_shared<OPCode93>(addr,dat_content,this);
                case 0x94: return std::make_shared<OPCode94>(addr,dat_content,this);
                case 0x95: return std::make_shared<OPCode95>(addr,dat_content,this);
                case 0x96: return std::make_shared<OPCode96>(addr,dat_content,this);
                case 0x97: return std::make_shared<OPCode97>(addr,dat_content,this);
//                case 0x98: return std::make_shared<OPCode98>(addr,dat_content,this);
//                //case 0x99: return std::make_shared<OPCode99>(addr,dat_content,this);
//                case 0x9A: return std::make_shared<OPCode9A>(addr,dat_content,this);
                case 0x9B: return std::make_shared<OPCode9B>(addr,dat_content,this);
//                case 0x9C: return std::make_shared<OPCode9C>(addr,dat_content,this);
                  case 0x9D: return std::make_shared<OPCode9D>(addr,dat_content,this);
                case 0x9E: return std::make_shared<OPCode9E>(addr,dat_content,this);
                case 0x9F: return std::make_shared<OPCode9F>(addr,dat_content,this);
                case 0xA0: return std::make_shared<OPCodeA0>(addr,dat_content,this);
                case 0xA1: return std::make_shared<OPCodeA1>(addr,dat_content,this);
                case 0xA2: return std::make_shared<OPCodeA2>(addr,dat_content,this);
                case 0xA3: return std::make_shared<OPCodeA3>(addr,dat_content,this);
                case 0xA4: return std::make_shared<OPCodeA4>(addr,dat_content,this);
//                case 0xA5: return std::make_shared<OPCodeA5>(addr,dat_content,this);
                case 0xA6: return std::make_shared<OPCodeA6>(addr,dat_content,this);
                case 0xA7: return std::make_shared<OPCodeA7>(addr,dat_content,this);
                case 0xA8: return std::make_shared<OPCodeA8>(addr,dat_content,this);
                case 0xA9: return std::make_shared<OPCodeA9>(addr,dat_content,this);
                case 0xAA: return std::make_shared<OPCodeAA>(addr,dat_content,this);
                case 0xAB: return std::make_shared<OPCodeAB>(addr,dat_content,this);
                case 0xAC: return std::make_shared<OPCodeAC>(addr,dat_content,this);
                case 0xAD: return std::make_shared<OPCodeAD>(addr,dat_content,this);
//                case 0xAE: return std::make_shared<OPCodeAE>(addr,dat_content,this);
                case 0xAF: return std::make_shared<OPCodeAF>(addr,dat_content,this);
                case 0xB0: return std::make_shared<OPCodeB0>(addr,dat_content,this);
                case 0xB1: return std::make_shared<OPCodeB1>(addr,dat_content,this);
                case 0xB2: return std::make_shared<OPCodeB2>(addr,dat_content,this);
                case 0xB3: return std::make_shared<OPCodeB3>(addr,dat_content,this);
//                case 0xB4: return std::make_shared<OPCodeB4>(addr,dat_content,this);
                case 0xB5: return std::make_shared<OPCodeB5>(addr,dat_content,this);
                case 0xB6: return std::make_shared<OPCodeB6>(addr,dat_content,this);
                case 0xB7: return std::make_shared<OPCodeB7>(addr,dat_content,this);
                case 0xB8: return std::make_shared<OPCodeB8>(addr,dat_content,this);
                case 0xB9: return std::make_shared<OPCodeB9>(addr,dat_content,this);
                case 0xBA: return std::make_shared<OPCodeBA>(addr,dat_content,this);
                case 0xBB: return std::make_shared<OPCodeBB>(addr,dat_content,this);
                case 0xBC: return std::make_shared<OPCodeBC>(addr,dat_content,this);
                case 0xBE: return std::make_shared<OPCodeBE>(addr,dat_content,this);
                case 0xBF: return std::make_shared<OPCodeBF>(addr,dat_content,this);
                case 0xC0: return std::make_shared<OPCodeC0>(addr,dat_content,this);
                case 0xC1: return std::make_shared<OPCodeC1>(addr,dat_content,this);
                case 0xC2: return std::make_shared<OPCodeC2>(addr,dat_content,this);
                case 0xC3: return std::make_shared<OPCodeC3>(addr,dat_content,this);
                case 0xC4: return std::make_shared<OPCodeC4>(addr,dat_content,this);
                case 0xC5: return std::make_shared<OPCodeC5>(addr,dat_content,this);
                case 0xC7: return std::make_shared<OPCodeC7>(addr,dat_content,this);
                case 0xC8: return std::make_shared<OPCodeC8>(addr,dat_content,this);
                case 0xC9: return std::make_shared<OPCodeC9>(addr,dat_content,this);
                case 0xCA: return std::make_shared<OPCodeCA>(addr,dat_content,this);
                default:
                std::stringstream stream;
                stream << "L'OP code " << std::hex << OP << " n'est pas défini !! " << addr;

                error = true;
                std::string result( stream.str() );
                qFatal(result.c_str());

                return std::shared_ptr<Instruction>();
            }
        }

        else if (function_type==2){//the function is a "effect" function

            return std::make_shared<EffectsInstr>(addr,dat_content,this);
        }

        else if (function_type==10){

            return std::make_shared<AnimeClipTable>(addr,dat_content,this);
        }
        else if (function_type==11){

            return std::make_shared<FieldMonsterData>(addr,dat_content,this);
        }

        else if (function_type==13){

            return std::make_shared<FC_autoX>(addr,dat_content,this);
       }
        else if (function_type==14){

            return std::make_shared<BookData99>(addr,dat_content,this);
        }
        else if (function_type==15){

            return std::make_shared<BookDataX>(addr,dat_content,this);
        }


        return std::shared_ptr<Instruction>();
    }
    bool CreateHeaderFromDAT(QByteArray &dat_content){
        display_text("Reading header...");
        uint nb_functions = ReadIntegerFromByteArray(0x14, dat_content);
        uint position_filename = ReadIntegerFromByteArray(0x4, dat_content);
        int position = position_filename, next_position = 0;
        QString filename = ReadStringFromByteArray(position, dat_content);
        SceneName = filename;
        int start_offset_area = ReadIntegerFromByteArray(0x8, dat_content);
        for (uint idx_fun = 0; idx_fun < nb_functions; idx_fun++){
            position = start_offset_area + 4*idx_fun;
            next_position = start_offset_area + 4*(idx_fun+1);
            int addr = ReadIntegerFromByteArray(position, dat_content);
            position = start_offset_area + 4*nb_functions + 2 * idx_fun;
            short name_pos = ReadShortFromByteArray(position, dat_content);
            int name_pos_int = name_pos;
            QString function_name = ReadStringFromByteArray(name_pos_int, dat_content);
            int end_addr;
            if (idx_fun == nb_functions-1) //we are at the last function, so it ends at the end of the file
                end_addr = dat_content.size();
            else
            {
                end_addr = ReadIntegerFromByteArray(next_position, dat_content);
            }
            FunctionsToParse.push_back(function(idx_fun,function_name,name_pos,addr,end_addr));

        }
        display_text("Header parsed.");
        return true;
    }
    std::shared_ptr<Instruction> CreateInstructionFromXLSX(int &addr, int row, QXlsx::Document &xls_content){

        uint OP = xls_content.read(row+1, 2).toInt();
        int i = TXUIFiles.indexOf(SceneName);
        if ((i != -1)&&(OP == 0x13)) return std::make_shared<UI_OP13>(addr,row, xls_content,this);

        switch(OP){
            case 0x00: return std::make_shared<OPCode0>(addr, row, xls_content,this);
            case 0x01: return std::make_shared<OPCode1>(addr, row, xls_content,this);
            case 0x02: return std::make_shared<OPCode02>(addr, row, xls_content,this);
            case 0x03: return std::make_shared<OPCode03>(addr, row, xls_content,this);
            case 0x04: return std::make_shared<OPCode04>(addr, row, xls_content,this);
            case 0x05: return std::make_shared<OPCode05>(addr, row, xls_content,this);
            case 0x06: return std::make_shared<OPCode06>(addr, row, xls_content,this);
            case 0x07: return std::make_shared<OPCode07>(addr, row, xls_content,this);
            case 0x08: return std::make_shared<OPCode08>(addr, row, xls_content,this);
            case 0x0A: return std::make_shared<OPCode0A>(addr, row, xls_content,this);
            case 0x0C: return std::make_shared<OPCode0C>(addr, row, xls_content,this);
            case 0x0D: return std::make_shared<OPCode0D>(addr, row, xls_content,this);
            case 0x0E: return std::make_shared<OPCode0E>(addr, row, xls_content,this);
            case 0x0F: return std::make_shared<OPCode0F>(addr, row, xls_content,this);
            case 0x10: return std::make_shared<OPCode10>(addr, row, xls_content,this);
            case 0x11: return std::make_shared<OPCode11>(addr, row, xls_content,this);
            case 0x12: return std::make_shared<OPCode12>(addr, row, xls_content,this);
            case 0x13: return std::make_shared<OPCode13>(addr, row, xls_content,this);
            case 0x14: return std::make_shared<OPCode14>(addr, row, xls_content,this);
            case 0x15: return std::make_shared<OPCode15>(addr, row, xls_content,this);
            case 0x16: return std::make_shared<OPCode16>(addr, row, xls_content,this);
            case 0x17: return std::make_shared<OPCode17>(addr, row, xls_content,this);
            case 0x18: return std::make_shared<OPCode18>(addr, row, xls_content,this);
            case 0x19: return std::make_shared<OPCode19>(addr, row, xls_content,this);
            case 0x1A: return std::make_shared<OPCode1A>(addr, row, xls_content,this);
            case 0x1B: return std::make_shared<OPCode1B>(addr, row, xls_content,this);
            case 0x1C: return std::make_shared<OPCode1C>(addr, row, xls_content,this);
            case 0x1D: return std::make_shared<OPCode1D>(addr, row, xls_content,this);
            case 0x1E: return std::make_shared<OPCode1E>(addr, row, xls_content,this);
            case 0x1F: return std::make_shared<OPCode1F>(addr, row, xls_content,this);
            case 0x20: return std::make_shared<OPCode20>(addr, row, xls_content,this);
    //                case 0x21: return std::make_shared<OPCode21>(addr, row, xls_content,this);
            case 0x22: return std::make_shared<OPCode22>(addr, row, xls_content,this);
            case 0x23: return std::make_shared<OPCode23>(addr, row, xls_content,this);
            case 0x24: return std::make_shared<OPCode24>(addr, row, xls_content,this);
            case 0x25: return std::make_shared<OPCode25>(addr, row, xls_content,this);
            case 0x26: return std::make_shared<OPCode26>(addr, row, xls_content,this);
            case 0x27: return std::make_shared<OPCode27>(addr, row, xls_content,this);
            case 0x28: return std::make_shared<OPCode28>(addr, row, xls_content,this);
            case 0x29: return std::make_shared<OPCode29>(addr, row, xls_content,this);
            case 0x2A: return std::make_shared<OPCode2A>(addr, row, xls_content,this);
            case 0x2B: return std::make_shared<OPCode2B>(addr, row, xls_content,this);
            case 0x2C: return std::make_shared<OPCode2C>(addr, row, xls_content,this);
            case 0x2D: return std::make_shared<OPCode2D>(addr, row, xls_content,this);
            case 0x2E: return std::make_shared<OPCode2E>(addr, row, xls_content,this);
            case 0x2F: return std::make_shared<OPCode2F>(addr, row, xls_content,this);
            case 0x30: return std::make_shared<OPCode30>(addr, row, xls_content,this);
            case 0x31: return std::make_shared<OPCode31>(addr, row, xls_content,this);
            case 0x32: return std::make_shared<OPCode32>(addr, row, xls_content,this);
            case 0x33: return std::make_shared<OPCode33>(addr, row, xls_content,this);
            case 0x34: return std::make_shared<OPCode34>(addr, row, xls_content,this);
            case 0x35: return std::make_shared<OPCode35>(addr, row, xls_content,this);
            case 0x36: return std::make_shared<OPCode36>(addr, row, xls_content,this);
            case 0x37: return std::make_shared<OPCode37>(addr, row, xls_content,this);
            case 0x38: return std::make_shared<OPCode38>(addr, row, xls_content,this);
            case 0x39: return std::make_shared<OPCode39>(addr, row, xls_content,this);
            case 0x3A: return std::make_shared<OPCode3A>(addr, row, xls_content,this);
            case 0x3B: return std::make_shared<OPCode3B>(addr, row, xls_content,this);
            case 0x3C: return std::make_shared<OPCode3C>(addr, row, xls_content,this);
            case 0x3D: return std::make_shared<OPCode3D>(addr, row, xls_content,this);
            case 0x3E: return std::make_shared<OPCode3E>(addr, row, xls_content,this);
            case 0x3F: return std::make_shared<OPCode3F>(addr, row, xls_content,this);
            case 0x40: return std::make_shared<OPCode40>(addr, row, xls_content,this);
            case 0x41: return std::make_shared<OPCode41>(addr, row, xls_content,this);
            case 0x42: return std::make_shared<OPCode42>(addr, row, xls_content,this);
            case 0x43: return std::make_shared<OPCode43>(addr, row, xls_content,this);
            case 0x44: return std::make_shared<OPCode44>(addr, row, xls_content,this);
            case 0x45: return std::make_shared<OPCode45>(addr, row, xls_content,this);
            case 0x46: return std::make_shared<OPCode46>(addr, row, xls_content,this);
            case 0x47: return std::make_shared<OPCode47>(addr, row, xls_content,this);
            case 0x48: return std::make_shared<OPCode48>(addr, row, xls_content,this);
            case 0x49: return std::make_shared<OPCode49>(addr, row, xls_content,this);
            case 0x4A: return std::make_shared<OPCode4A>(addr, row, xls_content,this);
            case 0x4B: return std::make_shared<OPCode4B>(addr, row, xls_content,this);
            case 0x4C: return std::make_shared<OPCode4C>(addr, row, xls_content,this);
            case 0x4D: return std::make_shared<OPCode4D>(addr, row, xls_content,this);
    //                case 0x4E: return std::make_shared<OPCode4E>(addr, row, xls_content,this);
            case 0x4F: return std::make_shared<OPCode4F>(addr, row, xls_content,this);
            case 0x50: return std::make_shared<OPCode50>(addr, row, xls_content,this);
            case 0x51: return std::make_shared<OPCode51>(addr, row, xls_content,this);
            case 0x52: return std::make_shared<OPCode52>(addr, row, xls_content,this);
            case 0x53: return std::make_shared<OPCode53>(addr, row, xls_content,this);
    //                case 0x54: return std::make_shared<OPCode54>(addr, row, xls_content,this);
            case 0x55: return std::make_shared<OPCode55>(addr, row, xls_content,this);
            case 0x56: return std::make_shared<OPCode56>(addr, row, xls_content,this);
            case 0x57: return std::make_shared<OPCode57>(addr, row, xls_content,this);
            case 0x58: return std::make_shared<OPCode58>(addr, row, xls_content,this);
    //                case 0x59: return std::make_shared<OPCode59>(addr, row, xls_content,this);
            case 0x5A: return std::make_shared<OPCode5A>(addr, row, xls_content,this);
            case 0x5B: return std::make_shared<OPCode5B>(addr, row, xls_content,this);
    //                case 0x5C: return std::make_shared<OPCode5C>(addr, row, xls_content,this);
            case 0x5D: return std::make_shared<OPCode5D>(addr, row, xls_content,this);
            case 0x5E: return std::make_shared<OPCode5E>(addr, row, xls_content,this);
            case 0x5F: return std::make_shared<OPCode5F>(addr, row, xls_content,this);
            case 0x60: return std::make_shared<OPCode60>(addr, row, xls_content,this);
            case 0x61: return std::make_shared<OPCode61>(addr, row, xls_content,this);
    //                case 0x62: return std::make_shared<OPCode62>(addr, row, xls_content,this);
            case 0x63: return std::make_shared<OPCode63>(addr, row, xls_content,this);
            case 0x64: return std::make_shared<OPCode64>(addr, row, xls_content,this);
            case 0x65: return std::make_shared<OPCode65>(addr, row, xls_content,this);
            case 0x66: return std::make_shared<OPCode66>(addr, row, xls_content,this);
            case 0x67: return std::make_shared<OPCode67>(addr, row, xls_content,this);
            case 0x68: return std::make_shared<OPCode68>(addr, row, xls_content,this);
            case 0x69: return std::make_shared<OPCode69>(addr, row, xls_content,this);
            case 0x6A: return std::make_shared<OPCode6A>(addr, row, xls_content,this);
                    case 0x6B: return std::make_shared<OPCode6B>(addr, row, xls_content,this);
            case 0x6C: return std::make_shared<OPCode6C>(addr, row, xls_content,this);
                    case 0x6D: return std::make_shared<OPCode6D>(addr, row, xls_content,this);
            case 0x6E: return std::make_shared<OPCode6E>(addr, row, xls_content,this);
            case 0x6F: return std::make_shared<OPCode6F>(addr, row, xls_content,this);
            case 0x70: return std::make_shared<OPCode70>(addr, row, xls_content,this);
    //                case 0x71: return std::make_shared<OPCode71>(addr, row, xls_content,this);
            case 0x72: return std::make_shared<OPCode72>(addr, row, xls_content,this);
            case 0x73: return std::make_shared<OPCode73>(addr, row, xls_content,this);
            case 0x74: return std::make_shared<OPCode74>(addr, row, xls_content,this);
            case 0x75: return std::make_shared<OPCode75>(addr, row, xls_content,this);
    //                case 0x76: return std::make_shared<OPCode76>(addr, row, xls_content,this);
    //                case 0x78: return std::make_shared<OPCode78>(addr, row, xls_content,this);
            case 0x79: return std::make_shared<OPCode79>(addr, row, xls_content,this);
    //                case 0x7A: return std::make_shared<OPCode7A>(addr, row, xls_content,this);
                    case 0x7B: return std::make_shared<OPCode7B>(addr, row, xls_content,this);
    //                case 0x7C: return std::make_shared<OPCode7C>(addr, row, xls_content,this);
              case 0x7D: return std::make_shared<OPCode7D>(addr, row, xls_content,this);
              case 0x7E: return std::make_shared<OPCode7E>(addr, row, xls_content,this);
              case 0x7F: return std::make_shared<OPCode7F>(addr, row, xls_content,this);
            case 0x80: return std::make_shared<OPCode80>(addr, row, xls_content,this);
            case 0x81: return std::make_shared<OPCode81>(addr, row, xls_content,this);
            case 0x82: return std::make_shared<OPCode82>(addr, row, xls_content,this);
            case 0x83: return std::make_shared<OPCode83>(addr, row, xls_content,this);
            case 0x84: return std::make_shared<OPCode84>(addr, row, xls_content,this);
            case 0x85: return std::make_shared<OPCode85>(addr, row, xls_content,this);
            case 0x86: return std::make_shared<OPCode86>(addr, row, xls_content,this);
            case 0x87: return std::make_shared<OPCode87>(addr, row, xls_content,this);
            case 0x88: return std::make_shared<OPCode88>(addr, row, xls_content,this);
            case 0x89: return std::make_shared<OPCode89>(addr, row, xls_content,this);
            case 0x8A: return std::make_shared<OPCode8A>(addr, row, xls_content,this);
    //                case 0x8B: return std::make_shared<OPCode8B>(addr, row, xls_content,this);
            case 0x8C: return std::make_shared<OPCode8C>(addr, row, xls_content,this);
                    case 0x8D: return std::make_shared<OPCode8D>(addr, row, xls_content,this);
            case 0x8E: return std::make_shared<OPCode8E>(addr, row, xls_content,this);
            case 0x8F: return std::make_shared<OPCode8F>(addr, row, xls_content,this);
            case 0x90: return std::make_shared<OPCode90>(addr, row, xls_content,this);
    //                case 0x91: return std::make_shared<OPCode91>(addr, row, xls_content,this);
            case 0x92: return std::make_shared<OPCode92>(addr, row, xls_content,this);
            case 0x93: return std::make_shared<OPCode93>(addr, row, xls_content,this);
                    case 0x94: return std::make_shared<OPCode94>(addr, row, xls_content,this);
            case 0x95: return std::make_shared<OPCode95>(addr, row, xls_content,this);
            case 0x96: return std::make_shared<OPCode96>(addr, row, xls_content,this);
            case 0x97: return std::make_shared<OPCode97>(addr, row, xls_content,this);
    //                case 0x98: return std::make_shared<OPCode98>(addr, row, xls_content,this);
    //                //case 0x99: return std::make_shared<OPCode99>(addr, row, xls_content,this);
    //                case 0x9A: return std::make_shared<OPCode9A>(addr, row, xls_content,this);
            case 0x9B: return std::make_shared<OPCode9B>(addr, row, xls_content,this);
    //                case 0x9C: return std::make_shared<OPCode9C>(addr, row, xls_content,this);
            case 0x9D: return std::make_shared<OPCode9D>(addr, row, xls_content,this);
            case 0x9E: return std::make_shared<OPCode9E>(addr, row, xls_content,this);
            case 0x9F: return std::make_shared<OPCode9F>(addr, row, xls_content,this);
            case 0xA0: return std::make_shared<OPCodeA0>(addr, row, xls_content,this);
            case 0xA1: return std::make_shared<OPCodeA1>(addr, row, xls_content,this);
            case 0xA2: return std::make_shared<OPCodeA2>(addr, row, xls_content,this);
            case 0xA3: return std::make_shared<OPCodeA3>(addr, row, xls_content,this);
            case 0xA4: return std::make_shared<OPCodeA4>(addr, row, xls_content,this);
    //                case 0xA5: return std::make_shared<OPCodeA5>(addr, row, xls_content,this);
            case 0xA6: return std::make_shared<OPCodeA6>(addr, row, xls_content,this);
            case 0xA7: return std::make_shared<OPCodeA7>(addr, row, xls_content,this);
            case 0xA8: return std::make_shared<OPCodeA8>(addr, row, xls_content,this);
            case 0xA9: return std::make_shared<OPCodeA9>(addr, row, xls_content,this);
            case 0xAA: return std::make_shared<OPCodeAA>(addr, row, xls_content,this);
            case 0xAB: return std::make_shared<OPCodeAB>(addr, row, xls_content,this);
            case 0xAC: return std::make_shared<OPCodeAC>(addr, row, xls_content,this);
            case 0xAD: return std::make_shared<OPCodeAD>(addr, row, xls_content,this);
    //                case 0xAE: return std::make_shared<OPCodeAE>(addr, row, xls_content,this);
            case 0xAF: return std::make_shared<OPCodeAF>(addr, row, xls_content,this);
            case 0xB0: return std::make_shared<OPCodeB0>(addr, row, xls_content,this);
            case 0xB1: return std::make_shared<OPCodeB1>(addr, row, xls_content,this);
            case 0xB2: return std::make_shared<OPCodeB2>(addr, row, xls_content,this);
            case 0xB3: return std::make_shared<OPCodeB3>(addr, row, xls_content,this);
    //                case 0xB4: return std::make_shared<OPCodeB4>(addr, row, xls_content,this);
            case 0xB5: return std::make_shared<OPCodeB5>(addr, row, xls_content,this);
            case 0xB6: return std::make_shared<OPCodeB6>(addr, row, xls_content,this);
            case 0xB7: return std::make_shared<OPCodeB7>(addr, row, xls_content,this);
            case 0xB8: return std::make_shared<OPCodeB8>(addr, row, xls_content,this);
            case 0xB9: return std::make_shared<OPCodeB9>(addr, row, xls_content,this);
            case 0xBA: return std::make_shared<OPCodeBA>(addr, row, xls_content,this);
            case 0xBB: return std::make_shared<OPCodeBB>(addr, row, xls_content,this);
            case 0xBC: return std::make_shared<OPCodeBC>(addr, row, xls_content,this);
            case 0xBE: return std::make_shared<OPCodeBE>(addr, row, xls_content,this);
            case 0xBF: return std::make_shared<OPCodeBF>(addr, row, xls_content,this);
            case 0xC0: return std::make_shared<OPCodeC0>(addr, row, xls_content,this);
            case 0xC1: return std::make_shared<OPCodeC1>(addr, row, xls_content,this);
            case 0xC2: return std::make_shared<OPCodeC2>(addr, row, xls_content,this);
            case 0xC3: return std::make_shared<OPCodeC3>(addr, row, xls_content,this);
            case 0xC4: return std::make_shared<OPCodeC4>(addr, row, xls_content,this);
            case 0xC5: return std::make_shared<OPCodeC5>(addr, row, xls_content,this);
            case 0xC7: return std::make_shared<OPCodeC7>(addr, row, xls_content,this);
            case 0xC8: return std::make_shared<OPCodeC8>(addr, row, xls_content,this);
            case 0xC9: return std::make_shared<OPCodeC9>(addr, row, xls_content,this);
            case 0xCA: return std::make_shared<OPCodeCA>(addr, row, xls_content,this);
            case 256: return std::make_shared<CreateMonsters>(addr, row, xls_content,this);
            case 257: return std::make_shared<EffectsInstr>(addr, row, xls_content,this);
            case 258: return std::make_shared<ActionTable>(addr, row, xls_content,this);
            case 259: return std::make_shared<AlgoTable>(addr, row, xls_content,this);
            case 260: return std::make_shared<WeaponAttTable>(addr, row, xls_content,this);
            case 261: return std::make_shared<BreakTable>(addr, row, xls_content,this);
            case 262: return std::make_shared<SummonTable>(addr, row, xls_content,this);
            case 263: return std::make_shared<ReactionTable>(addr, row, xls_content,this);
            case 264: return std::make_shared<PartTable>(addr, row, xls_content,this);
            case 265: return std::make_shared<AnimeClipTable>(addr, row, xls_content,this);
            case 266: return std::make_shared<FieldMonsterData>(addr, row, xls_content,this);
            case 267: return std::make_shared<FieldFollowData>(addr, row, xls_content,this);
            case 268: return std::make_shared<FC_autoX>(addr, row, xls_content,this);
            case 269: return std::make_shared<BookData99>(addr, row, xls_content,this);
            case 270: return std::make_shared<BookDataX>(addr, row, xls_content,this);
            case 271: return std::make_shared<AddCollision>(addr, row, xls_content,this);
            case 272: return std::make_shared<ConditionTable>(addr, row, xls_content,this);
            case 273: return std::make_shared<AnimeClipData>(addr, row, xls_content,this);
            default:
                std::stringstream stream;
                stream << "L'OP code " << std::hex << OP << " n'est pas défini !! " << this->SceneName.toStdString();
                error = true;
                addr++;
                /*std::string result( stream.str() );

                qFatal(result.c_str());*/

                return std::shared_ptr<Instruction>();
        }
    }


    QByteArray CreateHeaderBytes(){

        QByteArray header;

        QByteArray scene_name_bytes = SceneName.toUtf8();
        scene_name_bytes.append('\x0');
        int size_of_scene_name = scene_name_bytes.length();
        header.append(GetBytesFromInt(0x20));
        header.append(GetBytesFromInt(0x20));
        header.append(GetBytesFromInt(0x20+size_of_scene_name));
        header.append(GetBytesFromInt(FunctionsParsed.size()*4));
        header.append(GetBytesFromInt(0x20+size_of_scene_name + FunctionsParsed.size()*4));
        header.append(GetBytesFromInt(FunctionsParsed.size()));
        int length_of_names_section = 0;
        for (uint idx_fun = 0; idx_fun<FunctionsParsed.size(); idx_fun++) length_of_names_section = length_of_names_section + FunctionsParsed[idx_fun].name.toUtf8().length() + 1;
        header.append(GetBytesFromInt(0x20+size_of_scene_name + FunctionsParsed.size()*4 + FunctionsParsed.size()*2 + length_of_names_section));
        header.append(GetBytesFromInt(0xABCDEF00));
        header.append(scene_name_bytes);
        if (FunctionsParsed.size()>0){
            QByteArray position_names;
            QByteArray actual_names;
            int offset_names = 0;
            for (uint idx_fun = 0; idx_fun<FunctionsParsed.size(); idx_fun++) {
                header.append(GetBytesFromInt(FunctionsParsed[idx_fun].actual_addr));
                QByteArray name = FunctionsParsed[idx_fun].name.toUtf8();
                name.append('\x0');
                position_names.append(GetBytesFromShort(0x20+size_of_scene_name + FunctionsParsed.size()*4 + FunctionsParsed.size()*2 + offset_names));
                actual_names.append(name);
                offset_names = offset_names + name.size();

            }
            header.append(position_names);
            header.append(actual_names);
            int multiple = 4;
            if (FunctionsParsed[0].name.startsWith("_")) multiple = 0x10;
            int nb_byte_to_add = (((int) ceil((float)header.size()/multiple)))*multiple - header.size();
            QByteArray remaining;
            for (int i = 0; i < nb_byte_to_add;i++) {
                remaining.push_back('\x0');
            }
            header.append(remaining);
        }
        return header;
    }

};


#endif // TXINSTRUCTIONSSET_H
