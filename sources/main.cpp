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
<<<<<<< Updated upstream
    Dc.SetupGame("CS4");
    QString filepath = "C:/Users/Antoine/Desktop/scripts/t4000.dat";
    qDebug() << filepath;
    Dc.ReadFile(filepath);
    Dc.WriteFile(filepath);
=======
    Dc.SetupGame("CS3");
    QDir directory("C:/Users/Antoine/Desktop/other_files/battle/dat_en");
    QStringList dat_files = directory.entryList(QStringList() << "*.dat" << "*.DAT",QDir::Files);
    Dc.CheckAllFiles(dat_files,  "C:/Users/Antoine/Desktop/other_files/battle/dat_en","C:/Users/Antoine/Documents/build-CS3ScenarioDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Debug");
    //QString filepath = "C:/Users/Antoine/Desktop/scripts/t4000.dat";

   /* Dc.ReadFile(filepath);
    Dc.WriteFile(filepath);*/
>>>>>>> Stashed changes

    if (argc < 2){
        for (int i = 0; i < 32; i++) display_text(header[i]);
        display_text("----------------------------------------------------------------------------------------------------------");
        display_text("This tool was made by Twn to decompile the scenario files from The Legend of Heroes: Trails of Cold Steel III.");
        display_text("It is not perfect and requires further testing, therefore if you encounter any type of issue, please contact me");
        display_text("at feitaishi45@gmail.com");
        display_text("---------------------------------------------HOW TO USE---------------------------------------------------");
        display_text("There is only one command for the moment: CS3ScenarioDecompiler.exe <filepath_to_convert>");
        display_text("The output format is decided by the extension of the input file: XLSX gives a DAT and DAT gives a XLSX.");

    }
    else
    {
        const QString filepath = QCoreApplication::arguments().at(1);
        display_text("Processing " + filepath);

    }



    return a.exec();
}
