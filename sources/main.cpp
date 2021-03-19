#include <QCoreApplication>
#include "headers/global_vars.h"
#include "headers/decompiler.h"
#include <QDir>
#include <QSettings>
QString Game;
QString InputDatFileEncoding;
QString OutputDatFileEncoding;


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

    /*Decompiler Dc;
    Dc.SetupGame("CS1");
    QDir directory("C:/Users/Antoine/Desktop/CS3Debug");
    QStringList dat = directory.entryList(QStringList() << "*.dat",QDir::Files);
    Dc.CheckAllFiles(dat, "C:/Users/Antoine/Desktop/CS3Debug", "C:/Users/Antoine/Documents/build-SenScriptsDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Release");
    */
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    Game = settings.value("Game", "CS3").toString();
    InputDatFileEncoding = settings.value("InputDatFileEncoding", "UTF-8").toString();
    OutputDatFileEncoding = settings.value("OutputDatFileEncoding", "UTF-8").toString();

    if (argc < 2){
        for (int i = 0; i < 32; i++) display_text(header[i]);
        display_text("----------------------------------------------------------------------------------------------------------");
        display_text("This tool was made by Twn (@twn_kek on Twitter) to decompile the script files from The Legend of Heroes: Trails of Cold Steel III.");
        display_text("It now supports CS1, CS2, CS3, and Tokyo Xanadu.");
        display_text("It is not perfect and requires further testing, therefore if you encounter any type of issue, please contact me.");
        display_text("---------------------------------------------HOW TO USE---------------------------------------------------");
        display_text("There is only one command: SenScriptsDecompiler.exe <filepath_to_convert>");
        display_text("All the other parameters are retrieved from the config.ini file. Please make sure it is correctly filled.");
        display_text("The output format is decided by the extension of the input file: XLSX gives a DAT and DAT gives a XLSX.");

    }
    else
    {
        const QString full_path = QCoreApplication::arguments().at(1);
        QDir directory = QFileInfo(full_path).absoluteDir();
        QString file = QFileInfo(full_path).fileName();
        QStringList dat = directory.entryList(QStringList() << file,QDir::Files);
        foreach(QString file_, dat){

            Decompiler Dc;

            Dc.SetupGame(Game);
            Dc.ReadFile(file_);
            Dc.WriteFile(file_);
            display_text("Done.");
        }
    }



    //return a.exec();
    return a.exec();
}
