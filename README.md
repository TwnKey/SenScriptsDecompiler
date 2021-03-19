# Sen Scripts Decompiler

This tool was originally meant to decompile the scenario files from The Legend of Heroes: Trails of Cold Steel III, located in data/scripts/scena and talk.\
Then I added support for ani, btl, ui, book.\
Then I added support for CS1, CS2, and now Tokyo Xanadu.\
I plan to add support for CS4 after I beat the game when it releases on PC.\
I also plan to add support for Hajimari when the Clouded Leopard Entertainment PC version releases this summer.

# Instruction codes

Script files from recent Falcom games are made of functions, which are themselves made of instructions. Each instruction has an OP Code (one signature byte) 
and a functionality, which might vary between games.
To help understand what the instructions do, I plan to make a documentation (probably only for CS3).

# What is the point of this tool? 

First, this tool sorts the instructions separately (one instruction per row in the output file), instead of the serialized format of the dat files.
This greatly helps the understanding of the files.

Second, it updates pointers in the case you change the text length for dialogs or add new instructions. If the pointers were not updated, the game 
would jump to a random position in the file and interpret them as the start of a new instruction, causing the game to crash or simply have weird behaviour.

What this tool can be used for:
- Mixing crafts in Cold Steel series
- Fixing broken characters
- Fixing broken scenes in scenario
- Create new scenes
- Edit text without limit
- and many other things...

Here are some examples of what could be done:
- Translating multiple scenes in french in CS1: https://www.youtube.com/watch?v=uNm77TWt2XQ
- Making new scenes and stealing crafts from other characters: https://www.youtube.com/watch?v=2RlzRV6KvjA
- Playable Azure Siegfried in CS3: https://www.youtube.com/watch?v=pDbCoFvR1eQ

# Why QT?
I wanted to use Qt for the QXlsx library https://github.com/QtExcel/QXlsx which I believe is the most feature-complete library existing for XLSX file creation. 
I know the XLSX format can be problematic, but I find it useful for pointers and formulas. It also doesn't require a special environment like Python,
which was the main issue I had with EDDecompiler, used for Sky and Crossbell games (although it's probably the only issue I had with it, fantastic tool).

I won't make other output files formats to suit your needs, because it would complexify the tool even more and I don't want that to happen. 
You will have to create your own tools to parse my output files, such as https://github.com/Megaflan/Erebonia which is used for translation of Cold Steel games.

# How to use

First, make sure to fill out the "config.ini" file depending on your settings. You have to specify which game the dat files are from.
Example of "config.ini" file:

[General]
Game = "TX"                                 <- The game the files are from, possible games are: "TX", "CS1", "CS2", "CS3"\
InputDatFileEncoding = "UTF-8"              <- Encoding used in the input dat files (sometimes it can be Shift-JIS)\
OutputDatFileEncoding = "UTF-8"             <- Encoding used in the output dat files 

Then, just drag & drop the dat/xlsx file to the SenScriptsDecompiler exe and it will produce the desired output.

You can also run it from batch files by using SenScriptsDecompiler.exe "filepath", and cover multiple files by using  "folder_containing_dats/*.dat"
# Status

Below are some issues that can happen with this tool in its current state:

- Some instructions in the files might not be correctly delimited
- Bad quality of code (I'm still some sort of beginner, actually very insecure about that, please give me advice!)
- Some types might not be accurate (float / int) (easy to fix, though)
- Some pointers might be missing (easy to fix, though)
- Some (a very small amount though) files might be broken and therefore not supported.

I want this tool to be reliable. Currently I want to say it is (no issue found so far), but I only toyed with a few scenes from the games.\
If anybody finds a bug or any issue at all, please contact me immediately at feitaishi45@gmail.com
