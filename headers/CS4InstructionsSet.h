#ifndef CS4INSTRUCTIONSSET_H
#define CS4INSTRUCTIONSSET_H
#include "headers/utilities.h"
#include "headers/functions.h"
#include "headers/translationfile.h"

#include <QString>

//START FROM CS3 AT PC RELEASE!!!!!!!!!!!!!

/*The CSXBuilder contains hundreds of nested classes, every one of them describing the behaviour of a single instruction in the game
 * Those behaviours were deduced from a combination of analysis directly from the hexadecimal content of the dat files,
 * study of assembly/pseudo code from Ghidra
 * and also testing directly in game by modifying the ram and studying with Cheat Engine
 * all of them are different from one game to another (well except for CS1/CS2 and CS3/CS4 which present similarities)
*/
class CS4TranslationFile : public TranslationFile
{
    public:
    CS4TranslationFile():TranslationFile(){}

};
class CS4Builder : public Builder
{
    public:
    CS4Builder(){

    }

    bool CreateHeaderFromDAT(QByteArray &dat_content){

        //Here the main difference with CS3 is that the name of the file seems to always be located at a multiple of 0x04
        display_text("Reading header...");
        uint nb_functions = ReadIntegerFromByteArray(0x14, dat_content);
        int position = 0x20, next_position = 0;
        QString filename = ReadStringFromByteArray(position, dat_content);
        SceneName = filename;
        int nb_byte_to_add_name = (((int) ceil((float)SceneName.size()/4)))*4 - SceneName.size();

        int start_offset_area = 0x20 + filename.length()+nb_byte_to_add_name;
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


};


#endif // CS4INSTRUCTIONSSET_H
