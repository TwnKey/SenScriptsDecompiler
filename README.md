# CS3ScriptDecompiler
 This tool is meant to decompile the game files from The Legend of Heroes: Trails of Cold Steel III, located in data/scripts.
 It was initially supposed to only deal with the scena files, but I added support for book, ani and battle files. So it should actually be able to parse all the script files.
 EXCEPT:
 - chr_enemy_template.dat
 - mon_template.dat
 - mon000s.dat
 - mon037_c00.dat
 - mon042_c00.dat
 - mon042_c01.dat
 - mon046_c00.dat
 - ply000.dat
 - ply001.dat
 - rob013_c00.dat
 
 All of those files have instructions that didn't make sense to me. When swapping them with other ani files, it made my game crash (which confirms there is something wrong with them). I'm not even sure those files are used by the game at any point, if someone has more info, please contact me.
 
# Why QT?
I wanted to use Qt for the QXlsx library https://github.com/QtExcel/QXlsx which I believe is the most feature-complete library existing for XLSX file creation. 
I want to be able to customize the sheets as much as possible to make it comfortable for modders/translators.

# Instructions
Just drag and drop a dat file onto the executable, it will produce a XLSX file after some time.
The XLSX file contains each instruction in the file with its operands; you'll need to find the dialog instructions (they have the text as operands)
and edit them to translate the game.
When your edit of the XLSX file is complete, just drag and drop the file onto the executable and it will produce another dat file in a new folder.
Then copy this dat file to your game directory in the data/scripts directory.
You can operate on multiple files by writing a simple bat file that calls the executable with the name of each of your files to convert as argument.

# Status
This is a tool I mostly made in three weeks, thus it suffers from several issues:
- Lack of testing
- Some instructions in the files might not be correctly delimited
- Bad quality of code (I'm still a beginner)
- Some types might not be accurate (float / int) (easy to fix, though)
- Some pointers might be missing (easy to fix, though)
- Performance issues (Again I did all of that in three weeks, I focused more on the "reverse engineering" part than the code)

So far it was able to read all files from my version of the game, produce proper XLSX, then rewriting them in dat format with the same number of bytes.
The content of the files itself was the same except for the case where the game uses -0.0 instead of 0.0 (apparently those two have different bytes representations)
Since I can't write -0.0 in the excel (I believe), I write 0x00000000 instead of 0x80000000 in the dat file, thus causing a mismatch. I doubt there is a consequence. 

I tested it on some scenes with success. But really I don't know how robust it is.

I encourage anyone using this tool and having trouble to contact me immediately (feitaishi45@gmail.com) so that I can fix their problem and make the tool more reliable!
