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
class OPCode2 : public Instruction
{
    public:
    OPCode2():Instruction(2,nullptr){}
    OPCode2(Builder *Maker):Instruction("Call",2,Maker){}
    OPCode2(int &addr, QByteArray &content, Builder *Maker):Instruction("Call", 2,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray function_name = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr,"string", function_name));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            qDebug() << control_byte.size();
            switch((int)control_byte[0]){
                case 0xB:
                    //here we're calling a function that was defined at the beginning
                    std::vector<function>::iterator it = find_function_by_name(Maker->FunctionsToParse,ConvertBytesToString(function_name));
                    Maker->ReadIndividualFunction(*it,content);
                    Maker->FunctionsParsed.push_back(*it);
                    //Maker->FunctionsToParse.erase(it);
                    break;
            }


    }


};
class OPCode5 : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCode5():Instruction(5,nullptr){}
    OPCode5(Builder *Maker):Instruction("???",5,Maker){}
    OPCode5(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 5,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            qDebug() << "OP CODE 5 ";
            while ((int)control_byte[0]!=0x1){

                    switch ((int)control_byte[0])  {
                        case 0x0:
                            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                            break;

                        case 0x1c:{
                            //the next byte is the OP code for a new instruction
                            QByteArray OPCode_byte = ReadSubByteArray(content, addr, 1);
                            this->AddOperande(operande(addr,"byte", OPCode_byte));
                            std::shared_ptr<Instruction> instr = Maker->CreateInstructionFromDAT(addr, content);
                            this->AddOperande(operande(addr,"bytearray", instr->getBytes()));
                            break;
                        }


                        case 0x1e:
                            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                            break;
                        case 0x1f:
                        case 0x20:
                        case 0x23:

                            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                            break;
                        case 0x21:
                            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                            break;
                        default:
                            ;
                    }

                    control_byte = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr,"byte", control_byte));

                }
    }
};
class OPCode38 : public Instruction //This one is similar to 5C
{
    public:
    OPCode38():Instruction(0x38,nullptr){}
    OPCode38(Builder *Maker):Instruction("???",0x38,Maker){}
    OPCode38(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x38,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
    }
};
class OPCode5C : public Instruction //This one seems strange?
{
    public:
    OPCode5C():Instruction(0x5C,nullptr){}
    OPCode5C(Builder *Maker):Instruction("???",0x5C,Maker){}
    OPCode5C(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x5C,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
    }
};
class OPCodeAC : public Instruction
{
    public:
    OPCodeAC():Instruction(0xAC,nullptr){}
    OPCodeAC(Builder *Maker):Instruction("0xAC",0xAC,Maker){}
    OPCodeAC(int &addr, QByteArray &content, Builder *Maker):Instruction("0xAC", 0xAC,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((int)control_byte[0]){
                case 0x0:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                case 0x1:
                    //here we're calling a function that was defined at the beginning
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x5:
                case '\a':
                case '\t':
                    //here we're calling a function that was defined at the beginning
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
            }

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
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content){
        qDebug() << hex << addr;
        int OP = (dat_content[addr]&0xFF);
        switch(OP){
            case 0: return std::make_shared<OPCode0>(addr,dat_content,this);
            case 1: return std::make_shared<OPCode1>(addr,dat_content,this);
            case 2: return std::make_shared<OPCode2>(addr,dat_content,this);
            case 5: return std::make_shared<OPCode5>(addr,dat_content,this);
            case 0x5C: return std::make_shared<OPCode5C>(addr,dat_content,this);
            case 0xAC: return std::make_shared<OPCodeAC>(addr,dat_content,this);
            case 256: return std::make_shared<CreateMonsters>(addr,dat_content,this);
            default:
            std::stringstream stream;
            stream << "L'OP code " << std::hex << OP << " n'est pas défini !!";
            std::string result( stream.str() );

            qFatal(result.c_str());

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
