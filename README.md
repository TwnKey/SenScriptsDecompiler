# CS3ScenarioDecompiler
 This tool is meant to decompile the game files from The Legend of Heroes: Trails of Cold Steel III, located in data/scripts/scena and data/scripts/talk.
 
# Why QT?
I wanted to use Qt for the QXlsx library https://github.com/QtExcel/QXlsx which I believe is the most feature-complete library existing for XLSX file creation. 
I want to be able to customize the sheets as much as possible to make it comfortable for translators (currently it is not, though).

# Status
This is a tool I mostly made in two weeks while working full time, thus it suffers from several issues:
- Lack of testing
- Some instructions in the files might not be correctly delimited
- Bad quality of code (I'm still a beginner)
- Some types might not be accurate (float / int)
- Some pointers might be missing (easy to fix, though)
- Performance issues (Again I did all of that in two weeks, I focused more on the "reverse engineering" part than the code)

So far it was able to read all files from my version of the game, produce proper XLSX, then rewriting them in dat format with the same number of bytes.
The content of the files itself was the same except for the case where the game uses -0.0 instead of 0.0 (apparently those two have different bytes representations)
Since I can't write -0.0 in the excel (I believe), I write 0x00000000 instead of 0x80000000 in the dat file, thus causing a mismatch. I doubt there is a consequence. 

I encourage anyone using this tool and having trouble to contact me immediately (feitaishi45@gmail.com) so that I can fix their problem and make the tool more reliable!
