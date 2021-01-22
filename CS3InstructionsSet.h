#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "utilities.h"
#include "functions.h"
#include "instruction.h"
#include "Builder.h"
#include "translationfile.h"
class OPCode0 : public Instruction
{
    public:
    OPCode0():Instruction(0){

    }
    void WriteXLSX(){
        qDebug() << "WriteXLSX test";
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
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int addr){
        return std::make_shared<OPCode0>();
    }
    bool CreateHeaderFromDAT(TranslationFile &TF, QByteArray &dat_content){

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

        QString filename = ReadStringFromByteArray(0x20, dat_content);
        TF.setName(filename);
        int start_offset_area = 0x20 + filename.length();
        for (int idx_fun = 0; idx_fun < nb_functions; idx_fun++){
            int addr = ReadIntegerFromByteArray(start_offset_area + 4*idx_fun, dat_content);
            short name_pos = ReadShortFromByteArray(start_offset_area + 4*nb_functions + 2 * idx_fun , dat_content);
            QString function_name = ReadStringFromByteArray(name_pos, dat_content);
            function fun(function_name,name_pos,addr);
        }
        return true;
    }
    bool CreateHeaderFromXLSX(TranslationFile &TF){
        return true;
    }
};


#endif // CS3INSTRUCTIONSSET_H
