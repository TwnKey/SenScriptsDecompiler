# Sen Scripts Decompiler

Current Build Status: ![example workflow](https://github.com/TwnKey/SenScriptsDecompiler/actions/workflows/build/badge.svg)

This tool was originally meant to decompile the scenario files from The Legend of Heroes: Trails of Cold Steel III for translation purpose,
but the modding community asked me to make it compatible with the other CS games. I plan to update it for Hajimari no Kiseki
as soon as the August CLE release happens.

# Warning
When creating custom functions in XLSX files, never call your function with a name starting with "\_". I don't see a reason to do that and it's safer not to.
In fact my tool is not handling functions starting with "_" correctly: they are either called from another function (then they use instructions), or never called and follow a very specific format. For now my tool is using a simple workaround that has no guarantee to work for future games, but I don't see any risk right now.
In the very unlikely case that anyone was having a problem related to that, I would update it though. 

# Instruction codes

Script files from recent Falcom games are made of functions, which are themselves made of instructions. Each instruction has an OP Code (one signature byte) 
and a functionality, which might vary between games.
To help understand what the instructions do, I wrote a simple doc: https://docs.google.com/document/d/1YVjFSkPsj9M0UgsI6_de4TSz35MeL_rGuhSQDtRTXxw/edit?usp=sharing.

# What is the point of this tool? 

First, this tool sorts the instructions separately (one instruction per row in the output file), instead of the serialized format of the dat files.
This greatly helps the understanding of the files.

Second, it updates pointers in the case you change the text length for dialogs or add new instructions. If the pointers were not updated, the game 
would jump to a random position in the file and interpret them as the start of a new instruction, causing the game to crash or simply have weird behaviour.

What this tool can be used for:
- Mixing crafts in Cold Steel series (https://www.youtube.com/watch?v=cC9knrPk4sQ&ab_channel=NBigboyC2)
- Fixing broken characters
- Fixing broken scenes in scenario
- Create new scenes
- Edit text without limit
- and many other things...

Here are some examples of what could be done:
- Translating multiple scenes in french in CS1: https://www.youtube.com/watch?v=uNm77TWt2XQ
- Making new scenes and stealing crafts from other characters: https://www.youtube.com/watch?v=2RlzRV6KvjA
- Playable Azure Siegfried in CS3: https://www.youtube.com/watch?v=pDbCoFvR1eQ

# How to use

First, make sure to fill out the "config.ini" file depending on your settings. You have to specify which game the dat files are from.
Example of "config.ini" file:

[General]\
Game = "TX"                                 <- The game the files are from, possible games are: "TX", "CS1", "CS2", "CS3", "CS4"\
InputDatFileEncoding = "UTF-8"              <- Encoding used in the input dat files (sometimes it can be Shift-JIS)\
OutputDatFileEncoding = "UTF-8"             <- Encoding used in the output dat files 

Then, just drag & drop the dat/xlsx file to the SenScriptsDecompiler exe and it will produce the desired output.

You can also use command lines as follows:\
SenScriptsDecompiler.exe <filepath_to_convert>\
SenScriptsDecompiler.exe <Game> <filepath_to_convert>\
SenScriptsDecompiler.exe <Game> <filepath_to_convert> <output_folder>\

# Status

Below are some issues that can happen with this tool in its current state:

- Some instructions in the files might not be correctly delimited
- Bad quality of code (I'm still some sort of beginner, actually very insecure about that, please give me advice!)
- Some types might not be accurate (float / int) (easy to fix, though)
- Some (a very small amount though) files might be broken and therefore not supported.

If anybody finds a bug or any issue at all, please contact me immediately at feitaishi45@gmail.com or report the issue on github

