# Sen Scripts Decompiler

Current Build Status: ![buildstatus](https://github.com/TwnKey/SenScriptsDecompiler/actions/workflows/build.yml/badge.svg)

This tool can be used to turn the script files of various Falcom games into xlsx spreadsheets, making their content more readable and editable.
I was inspired by [EDDecompiler](https://github.com/Ouroboros/EDDecompiler) although the editing process is not as easy unfortunately, and a lot of research still needs to be done for the recent Falcom games.

The current compatible games are Trails of Cold Steel 1, 2, 3, 4, Tokyo Xanadu and Trails into Reverie. 
I won't provide decrypted script files for the latter so no need to reach out to me about that.

# Instruction codes

Script files from recent Falcom games are made of functions, which are themselves made of instructions. Each instruction has an OP Code (one signature byte) 
and a functionality, which might vary between games.
To help understand what the instructions do, I wrote a simple [doc](https://docs.google.com/document/d/1YVjFSkPsj9M0UgsI6_de4TSz35MeL_rGuhSQDtRTXxw/edit?usp=sharing).

# What is the point of this tool? 

First, this tool sorts the instructions separately (one instruction per row in the output file), instead of the serialized format of the dat files.
This greatly helps the understanding of the files.

Second, it updates pointers in the case you change the text length for dialogs or add new instructions. If the pointers were not updated, the game 
would jump to a random position in the file and interpret them as the start of a new instruction, causing the game to crash or simply have weird behaviour.

What this tool can be used for:
- [Mixing crafts in Cold Steel series](https://www.youtube.com/watch?v=cC9knrPk4sQ&ab_channel=NBigboyC2)
- Fixing broken characters
- Fixing broken scenes in scenario
- [Create new scenes](https://www.youtube.com/watch?v=qeEzfZHiSLM)
- Edit text without limit
- and many other things...

Here are some examples of what could be done:
- [Translating multiple scenes in french in CS1](https://www.youtube.com/watch?v=uNm77TWt2XQ)
- [Making new scenes and stealing crafts from other characters](https://www.youtube.com/watch?v=2RlzRV6KvjA)
- [Playable Azure Siegfried in CS3](https://www.youtube.com/watch?v=pDbCoFvR1eQ)

# How to use

First, make sure to fill out the "config.ini" file depending on your settings. You have to specify which game the dat files are from.
Example of "config.ini" file:

[General]\
Game = "TX"                                 <- The game the files are from, possible games are: "TX", "CS1", "CS2", "CS3", "CS4", "Reverie"\
InputDatFileEncoding = "UTF-8"              <- Encoding used in the input dat files (sometimes it can be Shift-JIS)\
OutputDatFileEncoding = "UTF-8"             <- Encoding used in the output dat files 

Then, just drag & drop the dat/xlsx file to the SenScriptsDecompiler exe and it will produce the desired output.

You can also use command lines as follows:\
SenScriptsDecompiler.exe <filepath_to_convert>\
SenScriptsDecompiler.exe <Game> <filepath_to_convert>\
SenScriptsDecompiler.exe <Game> <filepath_to_convert> <output_folder>
