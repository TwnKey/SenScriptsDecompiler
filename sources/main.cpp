#include <QCoreApplication>

#include "headers/decompiler.h"
#include <QDir>

QString header[] = {".                           ,##(&%%@                                            ",
                    "          .           .  ..(##/%%%%%%%&. .                  ....                ",
                    "          .           (.../##/%%%##(%%%%%/,.....,*(/  .  (%#((%%/.              ",
                    "                  .. ....(##(%%%#(/#..............*#,....*&%%%%%&               ",
                    "                   /....####%%%%/....................*..//*..,/#%. .            ",
                    "                  (.,.*%##*%%%....... ..........................%%,             ",
                    "               . #.(.,#%%%%%(......... ...,,*,.....,,........... .#, .          ",
                    "                (.(,%(((%%%*.......,.*....*,.,.*.#............,. .., .          ",
                    "              .#.(#####%&%*.............,(.., .(#..,........,..*......          ",
                    "              (.(%#(/%%%%%.,..,..*...*.#,(*..#,(,(/..... ...#.........          ",
                    "             ,..(/%%%&%%%....(.....,*.(*(*..,.%/./*,........(.../..,..*         ",
                    "          .. *.,(/(%%%&%%.(.(.../.,#.*,*%./. *(,*(,.,......//,..(..,..(         ",
                    "           .(..,(.((&%%%*./##...(./*/,  #/./((*#((,(,...,./*.*.(/..,..(         ",
                    "          . ....(.#(((((.(,(/,..((#%@@@@&* **#%*#(%*...*.*,*%,#(......*.        ",
                    "          .(...,,/(((((%,,,/*...(,/.,#(&,/,@&(,/(/,.//((/ /**,#...*... .        ",
                    "         . ....,.#((//((/(,*,...#,   .((,       */,..@@@@@@**#...,..(*          ",
                    "        . #....,.((,/,(,..*,,...(,                 . * .( #./..(,..%*           ",
                    "         /,...*,,(,*.((..(&.*...(,                     ,  /(,##../ ,(.          ",
                    "        ..,...(*(,/..((..*##/.../,,              *       ,(((#./ .  (           ",
                    "        ,,....((/,.,(((...(%#..,&/,,                     %(((##  .              ",
                    "       /.*...((#../(#((,,..(%..,%,,#,                 ./%(((((#  .              ",
                    "      *.(...,((..(((#((%,..,(...* ,,,*% .     .... #####%(((((#                 ",
                    "   . ..(*...(*..((((%(#%.(..,#...,%#/,,,*//.*&&%#%%###((#*((((#  .              ",
                    "   . *((.../* **      , (.*....../,  %(((*,,(#&//&&%#((((.(((((  .              ",
                    "    #((/.,...,    ,      .%*......(.   ,,#(#%&&&&&&&,&#/(,(/(((*                ",
                    "  .#(((*.*,#,,,      ,   **%/......#((  .*(#%&&&&&*.%/%( /.#(((( .              ",
                    " ./#,.%.../.           .  #(((......*.  ./(%%%(*(&&&&/&#,#.*#*((( ..            ",
                    ",((((//. (#             .  #(,(......#. (((. *./*((/@/&&,,..##,.((%.            ",
                    "%(/(/(*./(/.           , .  /(,(....../,((*,(  #(.,(((#*,,*..%#(...,/.         (",
                    "( (((//(,**/*/*(      ,,.*,   (*(..*.,./(%..((&*/.%(.../# *..,,*&,....*      #**",
                    " (/#///((*,*...       ,,(((/***(%/..(....#%,,.(#%#&#.,,,,%*#../,  #*...../ %**##",
                    "(/(/////%            ,(/%*,  .,,((..,#..*.(,(,*##%%%%(,,.,*/#(..(, ,.*...#**##*."};


int main(int argc, char *argv[])
    {
    QCoreApplication a(argc, argv);

    Decompiler Dc;
    Dc.SetupGame("TX");
    QDir directory("C:/Users/Antoine/Desktop/TX_Debug");
    QStringList dat = directory.entryList(QStringList() << "*.dat",QDir::Files);
    Dc.CheckAllFiles(dat, "C:/Users/Antoine/Desktop/TX_Debug", "C:/Users/Antoine/Documents/build-SenScriptsDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Release");


    if (argc < 3){
        display_text("Here is a Tio, you're welcome:");
        for (int i = 0; i < 32; i++) display_text(header[i]);
        display_text("----------------------------------------------------------------------------------------------------------");
        display_text("This tool was made by Twn (@twn_kek on Twitter) to decompile the script files from The Legend of Heroes: Trails of Cold Steel III.");
        display_text("I recently added support for CS1 & CS2 script files as well.");
        display_text("It is not perfect and requires further testing, therefore if you encounter any type of issue, please contact me.");
        display_text("---------------------------------------------HOW TO USE---------------------------------------------------");
        display_text("There is only one command: SenScriptsDecompiler.exe <game> <filepath_to_convert>");
        display_text("where <game> should be replaced with CS1 or CS2 or CS3 (other games are not compatible (yet))");
        display_text("The output format is decided by the extension of the input file: XLSX gives a DAT and DAT gives a XLSX.");

    }
    else
    {
        const QString filepath = QCoreApplication::arguments().at(2);
        const QString Game = QCoreApplication::arguments().at(1);
        display_text("Processing " + filepath);
        Decompiler Dc;
        Dc.SetupGame(Game);
        Dc.ReadFile(filepath);
        Dc.WriteFile(filepath);
    }



    return a.exec();
}
