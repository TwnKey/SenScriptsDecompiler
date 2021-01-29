#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "utilities.h"
#include "functions.h"
#include "translationfile.h"

#include <QString>

void fun_1403c90e0(int &addr, QByteArray &content, Instruction * instr, int param){
    QByteArray control_byte3_arr = ReadSubByteArray(content, addr, 1);
    instr->AddOperande(operande(addr,"byte", control_byte3_arr));
    unsigned char control_byte3 = (unsigned char)control_byte3_arr[0];

    if (control_byte3 == '\x11'){
        instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

    }
    else if (control_byte3 != '3'){

        if (control_byte3 == '\"'){
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }
        else if (control_byte3 != 0x44){

            if (control_byte3 == 0xDD){
                qDebug() << "reading string starting at " << hex << addr;
                //there is a XOR,XOR EDI which causes a jump that ignores the strcpy
                if (param!=0)instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }
            else if (control_byte3 == 0xFF){
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            }
            else{
                if (control_byte3 == 0xEE){

                }
                else{
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));}
            }


        }

    }
}
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
        addr++;

        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
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
            QByteArray control_byte2_arr = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte2_arr));

            int control_byte2 = (int)control_byte2_arr[0];

            if (control_byte2!=0){
                for (int i = 0; i < control_byte2; i++){
                    fun_1403c90e0(addr, content, this, 0);
                }

            }
            switch((int)control_byte[0]){
                case 0xB:

                    qDebug() << "Calling function " << ConvertBytesToString(function_name);
                    //here we're calling a function that was defined at the beginning
                    std::vector<function>::iterator it = find_function_by_name(Maker->FunctionsToParse,ConvertBytesToString(function_name));
                    qDebug() << "found " << it->name;
                    if (!std::count(Maker->FunctionsParsed.begin(), Maker->FunctionsParsed.end(), *it)){
                        Maker->ReadIndividualFunction(*it,content);
                        Maker->FunctionsParsed.push_back(*it);
                    }
                    //Maker->FunctionsToParse.erase(it);
                    break;
            }



    }


};
class OPCode3 : public Instruction
{
    public:
    OPCode3():Instruction(3,nullptr){}
    OPCode3(Builder *Maker):Instruction("???",3,Maker){}
    OPCode3(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 3,Maker){
        addr++;
        this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4)));
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
            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4))); // pointer
    }
};
class OPCodeC : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCodeC():Instruction(0xC,nullptr){}
    OPCodeC(Builder *Maker):Instruction("???",0xC,Maker){}
    OPCodeC(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0xC,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode10 : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCode10():Instruction(0x10,nullptr){}
    OPCode10(Builder *Maker):Instruction("???",0x10,Maker){}
    OPCode10(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x10,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode14 : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCode14():Instruction(0x14,nullptr){}
    OPCode14(Builder *Maker):Instruction("???",0x14,Maker){}
    OPCode14(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x14,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode1D : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCode1D():Instruction(0x1D,nullptr){}
    OPCode1D(Builder *Maker):Instruction("???",0x1D,Maker){}
    OPCode1D(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x1D,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//6
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//E
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x12
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x16
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x1A
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x1E
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x22
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            QByteArray IDFun_bytearray = ReadSubByteArray(content, addr,4);
            int IDfun = ReadIntegerFromByteArray(0, IDFun_bytearray);
            if (IDfun!=-1){
                std::vector<function>::iterator it = find_function_by_ID(Maker->FunctionsToParse,IDfun);
                if (!std::count(Maker->FunctionsParsed.begin(), Maker->FunctionsParsed.end(), *it)){
                    qDebug() << "Calling CreateMonster function at addr " << hex << it->actual_addr << " ID: "<< hex << it->ID;
                    it->AddInstruction(std::make_shared<CreateMonsters>(it->actual_addr,content,Maker));
                    Maker->FunctionsParsed.push_back(*it);
                }
            }
            this->AddOperande(operande(addr,"int", IDFun_bytearray));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode1E : public Instruction //This one might actually be exactly the same than CS1
{
    public:
    OPCode1E():Instruction(0x1E,nullptr){}
    OPCode1E(Builder *Maker):Instruction("???",0x1E,Maker){}
    OPCode1E(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x1E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

    }
};
class OPCode32 : public Instruction
{
    public:
    OPCode32():Instruction(0x32,nullptr){}
    OPCode32(Builder *Maker):Instruction("???",0x32,Maker){}
    OPCode32(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x32,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch ((int)control_byte[0])  {
            case '\n':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                break;

            case '\v':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\f':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                //Here we need to use the function fun_1403c90e0 multiple times
                break;
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
    }
};

class OPCode3A : public Instruction //This one is similar to 5C
{
    public:
    OPCode3A():Instruction(0x3A,nullptr){}
    OPCode3A(Builder *Maker):Instruction("???",0x3A,Maker){}
    OPCode3A(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x3A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((int)control_byte[0])  {
                case 0x0:
                case 0x4:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x1:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x5:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x6:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x7:

                    break;
                case 0x8:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x9:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }


    }
};

class OPCode47 : public Instruction //This one is similar to 5C
{
    public:
    OPCode47():Instruction(0x47,nullptr){}
    OPCode47(Builder *Maker):Instruction("???",0x47,Maker){}
    OPCode47(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x47,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
    }
};

class OPCode50 : public Instruction
{
    public:
    OPCode50():Instruction(0x50,nullptr){}
    OPCode50(Builder *Maker):Instruction("0x50",0x50,Maker){}
    OPCode50(int &addr, QByteArray &content, Builder *Maker):Instruction("0x50", 0x50,Maker){
            addr++;
            fun_1403c90e0(addr, content, this, 1);

    }
};
class OPCode54 : public Instruction
{
    public:
    OPCode54():Instruction(0x54,nullptr){}
    OPCode54(Builder *Maker):Instruction("???",0x54,Maker){}
    OPCode54(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x54,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            QByteArray control_byte2;
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((int)control_byte[0])  {
                case 0x1:
                case 0x3:
                case 0x8:
                case 0x0:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0x20-4)));
                    break;
                case 10:
                case 0xE:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0xB:

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,1)));
                    break;
                case 0xD:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x14:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x15:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;
                case 0x17:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0x24:
                case 0x22:
                case 0x18:
                case 0x28:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x19:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x1C:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x21:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;

                case 0x23:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x26:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;

                case 0x29:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x2B:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr,"byte", control_byte2));
                    if ((int)control_byte2[0]==0){
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));}
                    break;
                case 0x2C:

                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x31:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr,"byte", control_byte2));
                    if ((int)control_byte2[0]==0){
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    }
                    else if ((int)control_byte2[0]==1){
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    }
                    break;
            case 0x34:

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x35:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x37:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            case 0x38:

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            case 0x39:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x3A:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //0
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //2
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//C
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1))); //0x14

                break;
            case 0x3B:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //0
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //2
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8
                break;

            case 0x3C:

                this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0xA)));
                break;
            case 0x3E:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x40:
            case 0x4B:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x42:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x44:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x45:
            case 0x46:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x47:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x48:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x49:
            case 0x4A:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x4C:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            case 0x4D:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            case 0x4F:
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr,"byte", control_byte2)); //1
                switch((int)control_byte2[0]){
                    case 0:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    case 1:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 2:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 3:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 4:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case '\n':
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //3
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1))); //4
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4))); //8
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //10
                        break;
                }
                break;
            case 0x50:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x51:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x52:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


                break;
            case 0x53:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;

            }


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
class OPCode68 : public Instruction //This one is similar to 5C
{
    public:
    OPCode68():Instruction(0x68,nullptr){}
    OPCode68(Builder *Maker):Instruction("???",0x68,Maker){}
    OPCode68(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x68,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            switch((int)control_byte[0]){
            case 0:
            case 1:
            case '\v':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 2:
            case 3:
            case 4:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case '\a':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\b':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\x18':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case '\x1E':
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
    }
};
class OPCode74 : public Instruction //This one is similar to 5C
{
    public:
    OPCode74():Instruction(0x74,nullptr){}
    OPCode74(Builder *Maker):Instruction("???",0x74,Maker){}
    OPCode74(int &addr, QByteArray &content, Builder *Maker):Instruction("???", 0x74,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((int)control_byte[0]){
                case 0:
                case 0x14:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 2:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//0
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//2
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//6
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//A
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case 3:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
            }
    }
};
class OPCode86 : public Instruction
{
    public:
    OPCode86():Instruction(0x86,nullptr){}
    OPCode86(Builder *Maker):Instruction("0x86",0x86,Maker){}
    OPCode86(int &addr, QByteArray &content, Builder *Maker):Instruction("0x86", 0x86,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//2
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//4
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//6
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//8
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//A
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//c
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


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
class OPCodeCA : public Instruction
{
    public:
    OPCodeCA():Instruction(0xCA,nullptr){}
    OPCodeCA(Builder *Maker):Instruction("0xCA",0xCA,Maker){}
    OPCodeCA(int &addr, QByteArray &content, Builder *Maker):Instruction("0xCA", 0xCA,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((int)control_byte[0]){
                case 0x0:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
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

        int OP = (dat_content[addr]&0xFF);
        qDebug() << "OP CODE ???: " << hex << OP << "at addr: " << hex << addr;
        switch(OP){
            case 0: return std::make_shared<OPCode0>(addr,dat_content,this);
            case 1: return std::make_shared<OPCode1>(addr,dat_content,this);
            case 2: return std::make_shared<OPCode2>(addr,dat_content,this);
            case 3: return std::make_shared<OPCode3>(addr,dat_content,this);
            case 5: return std::make_shared<OPCode5>(addr,dat_content,this);
            case 0xC: return std::make_shared<OPCodeC>(addr,dat_content,this);
            case 0x10: return std::make_shared<OPCode10>(addr,dat_content,this);
            case 0x14: return std::make_shared<OPCode14>(addr,dat_content,this);
            case 0x1D: return std::make_shared<OPCode1D>(addr,dat_content,this);
            case 0x1E: return std::make_shared<OPCode1E>(addr,dat_content,this);
            case 0x38: return std::make_shared<OPCode38>(addr,dat_content,this);
            case 0x3A: return std::make_shared<OPCode3A>(addr,dat_content,this);
            case 0x47: return std::make_shared<OPCode47>(addr,dat_content,this);
            case 0x50: return std::make_shared<OPCode50>(addr,dat_content,this);
            case 0x54: return std::make_shared<OPCode54>(addr,dat_content,this);
            case 0x5C: return std::make_shared<OPCode5C>(addr,dat_content,this);
            case 0x68: return std::make_shared<OPCode68>(addr,dat_content,this);
            case 0x74: return std::make_shared<OPCode74>(addr,dat_content,this);
            case 0x86: return std::make_shared<OPCode86>(addr,dat_content,this);
            case 0xAC: return std::make_shared<OPCodeAC>(addr,dat_content,this);
            case 0xCA: return std::make_shared<OPCodeCA>(addr,dat_content,this);
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
