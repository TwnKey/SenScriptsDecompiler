# Sen Scripts Decompiler
This tool was meant to decompile the game files from The Legend of Heroes: Trails of Cold Steel III, located in data/scripts.
But I added support for CS1 scripts as well! 
 
# Instruction codes
Sen script files are made of functions, which are themselves made of instructions. Each instruction has an OP Code (one signature byte) 
and a functionality, which might vary between games. Here are some known instructions 
(credits to NZerker (https://www.youtube.com/channel/UCpeimzf-vGWJuGWI3br6Sag)
for helping me finding some of them) (also please note that the following names are pure inventions)

SEN3:
2   : Call
29  : CreateCharacter (CharacterID, Character Model, Character name, ...)
36  : CharacterTalk (CharacterID, .., Text, ...)
44  : UseAnimation
47  : UseAnimationFromFile
73  : AddCharacterToParty (.., CharacterID)
116 : ReadActiveVoiceEvent

# Why QT?
I wanted to use Qt for the QXlsx library https://github.com/QtExcel/QXlsx which I believe is the most feature-complete library existing for XLSX file creation. 
I want to be able to customize the sheets as much as possible to make it comfortable for modders/translators.

# How to use

Previously it was dragging and dropping the file onto the executable, but since I added CS1 support, it is now mandatory to provide the name
of the game that uses the file. Indeed the files' structure doesn't change between the games, only the functions and instructions might, therefore
there is no way to know the game from the file content only.

To generate a XLSX/dat file, just run the command: SenScriptsDecompiler.exe "game" "filepath"

where "game" can currently take "CS1" and "CS3" as values. If the file designated by filepath is a xlsx one, the tool will create a dat file, vice versa.

# Status

This is a tool I mostly made in four weeks (including CS1 support), thus it suffers from several issues:

- Lack of testing
- Some instructions in the files might not be correctly delimited
- Bad quality of code (I'm still some sort of beginner, actually very insecure about that, please give me advice!)
- Some types might not be accurate (float / int) (easy to fix, though)
- Some pointers might be missing (easy to fix, though)
- Performance issues (Again I did all of that in four weeks, I focused more on the "reverse engineering" part than the code but I'm still willing to learn)

So far the tool was able to read most of the correctly generated files from my versions of the games, produce proper XLSX, then rewriting them in dat format with the same number of bytes (with some exceptions though).
The content of the files itself was the same except for the cases where the game uses -0.0 instead of 0.0 (apparently those two have different bytes representations)
and minor cases where I would write more zeros at the end of each functions, using different strategies, when I think there is actually no need to worry about that. 
Since I can't write -0.0 in the excel (I believe), I write 0x00000000 instead of 0x80000000 in the dat file, thus causing a mismatch. I doubt there is a consequence. 

I tested it on some scenes from both games with only successes so far. As a demonstration I translated a scene from CS1 in french here, spoilers ahead: https://youtu.be/uNm77TWt2XQ

I encourage anyone using this tool and having trouble to contact me immediately (feitaishi45@gmail.com) so that I can fix their problem and make the tool more reliable!

As for other Falcom games that might need a decompilation, I'm thinking about dealing with Sen IV and Sen II later this year. Since there is the CLE Hajimari PC release coming this summer,
I might also try to decompile Hajimari scripts and integrate the current english spreadsheet in it.
