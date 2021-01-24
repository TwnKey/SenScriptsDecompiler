#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "utilities.h"
#include "functions.h"
#include "translationfile.h"

#include <QString>
class CreateMonsters : public Instruction
{
    public:
    CreateMonsters():Instruction(256,nullptr){}
    CreateMonsters(Builder *Maker):Instruction("CreateMonsters", 256, Maker){}
    CreateMonsters(int &addr, QByteArray &content,Builder *Maker):Instruction("CreateMonsters", 256,Maker){
        int initial_addr = addr;
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2))); //RSI + 0x28
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4))); //0x2C

        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x30
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x32
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x34
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x36

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x38

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x3C
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x40
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x44
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x48
        //0x4C The first part is done
        //from here, it's just a guess. There is a maximum of 8 enemies hardcoded in the function.
        QByteArray array = ReadSubByteArray(content, addr, 4);
        this->AddOperande(operande(addr,"int", array));
        int first = ReadIntegerFromByteArray(0,array);
        do{
            int counter = 0;

            do{
                counter++;

                QByteArray monsters_name = ReadSubByteArray(content, addr, 16); //I believe the name of the monsters has to be 8 bytes long,
                //but I saw a check for the \x0 character so I include the additional 0 and will deal with them later
                this->AddOperande(operande(addr,"string", monsters_name));
            }
            while(counter < 0x7);
            for (int idx_byte = 0; idx_byte < 8; idx_byte++) this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 8))); //??

            QByteArray array = ReadSubByteArray(content, addr, 4);
            this->AddOperande(operande(addr,"int", array));
            first = ReadIntegerFromByteArray(0,array);

        }
        while(first != -1);
        this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 0x18))); //??
        this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
    }


};
class OPCode0 : public Instruction
{
    public:
    OPCode0():Instruction(0,nullptr){}
    OPCode0(Builder *Maker):Instruction("Instruction 0", 0, Maker){}
    OPCode0(int &addr, QByteArray &content,Builder *Maker):Instruction("Instruction 0", 0,Maker){

    }


};
class OPCode1 : public Instruction
{
    public:
    OPCode1():Instruction(1,nullptr){}
    OPCode1(Builder *Maker):Instruction("Return",1,Maker){}
    OPCode1(int &addr, QByteArray &content, Builder *Maker):Instruction("Return", 1,Maker){

    }


};
class CS3TranslationFile : public TranslationFile
{
    public:
    CS3TranslationFile():TranslationFile(){}

};
class CS3Builder : public Builder
{
    public:
    CS3Builder(){

    }
    std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row){
        return std::make_shared<OPCode0>();
    }
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int addr, QByteArray &dat_content){
        int OP = dat_content[addr];
        switch(OP){
            case 0: return std::make_shared<OPCode0>(addr,dat_content,this);
            case 1: return std::make_shared<OPCode1>(addr,dat_content,this);

            case 256: return std::make_shared<CreateMonsters>(addr,dat_content,this);
            default:
            std::string error_msg = std::to_string(OP) + "n'est pas défini!!";
            qFatal(error_msg.c_str());

            return std::shared_ptr<Instruction>();
        }

    }
    bool CreateHeaderFromDAT(QByteArray &dat_content){

        //Header structure:
        //The first 0x20 I'd say is probably the position of the name of the file; which shouldn't change
        //The second 0x20 probably doesn't change too.
        //The next integer is the position of the first pointer
        //The 4th: probably the length in bytes of the pointer section
        //The fifth: probably the position of the "names positions" section (right after the pointer section)
        //The sixth: the number of functions inside the file
        //the seventh: the position one byte after the 0x00 separator at the end of the functions section
        //the eighth: 0xABCDEF00 => seems to always be there (no idea why)
        //Then the name of the file
        //Then the pointer section
        //Then the "names positions" section
        //Then the functions section
        //Done; here the function objects holds the number of functions, the addr, name positions
        //everything else can be deduced
        int nb_functions = dat_content[0x14];
        int position = 0x20;
        QString filename = ReadStringFromByteArray(position, dat_content);
        SceneName = filename;
        int start_offset_area = 0x20 + filename.length()+1;
        for (int idx_fun = 0; idx_fun < nb_functions; idx_fun++){
            position = start_offset_area + 4*idx_fun;
            int addr = ReadIntegerFromByteArray(position, dat_content);
            position = start_offset_area + 4*nb_functions + 2 * idx_fun;
            short name_pos = ReadShortFromByteArray(position, dat_content);
            int name_pos_int = name_pos;
            QString function_name = ReadStringFromByteArray(name_pos_int, dat_content);
            FunctionsToParse.push_back(function(idx_fun,function_name,name_pos,addr));
        }
        return true;
    }
    bool CreateHeaderFromXLSX(){
        return true;
    }
};


#endif // CS3INSTRUCTIONSSET_H
