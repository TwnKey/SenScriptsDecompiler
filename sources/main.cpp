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
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    Game = settings.value("Game", "CS4").toString();
    InputDatFileEncoding = settings.value("InputDatFileEncoding", "UTF-8").toString();
    OutputDatFileEncoding = settings.value("OutputDatFileEncoding", "UTF-8").toString();

    /*Decompiler Dc;
    Dc.SetupGame("CS4");
    QDir directory("C:/Users/Antoine/Desktop/CS4Debug");
    QStringList dat = directory.entryList(QStringList() << "*.dat",QDir::Files);
    Dc.CheckAllFiles(dat, "C:/Users/Antoine/Desktop/CS4Debug", "C:/Users/Antoine/Documents/build-SenScriptsDecompiler-Desktop_Qt_5_9_9_MSVC2015_32bit-Release");
    */




    if (argc < 2){
        for (int i = 0; i < 32; i++) display_text(header[i]);
        display_text("----------------------------------------------------------------------------------------------------------");
        display_text("This tool was first meant for translation projects of the Trails of Cold Steel games, starting with ToCS3.");
        display_text("Then it has become a tool for modding in general.");
        display_text("Be warned: it is not perfect, and some files might be wrongly parsed. ");
        display_text("However, it has already been used in many situations, without any major issue reported yet.");
        display_text("If you encounter a problem, please contact me at feitaishi45@gmail.com");
        display_text("---------------------------------------------HOW TO USE---------------------------------------------------");
        display_text("There is only one command: SenScriptsDecompiler.exe <filepath_to_convert>");
        display_text("All the other parameters are specified inside the config.ini file. Please make sure it is correctly written.");
        display_text("The output format is decided by the extension of the input file: XLSX gives a DAT and DAT gives a XLSX.");
        display_text("If you need more assistance, please join the discord server for modding Trails games: https://discord.gg/XpFrXWht6j");
        display_text("Credits to NZerker and kirigaia for their support and testing.");
    }
    else if (argc >= 2)
    {
        QString full_path;
        if (argc > 2) {
            Game = QCoreApplication::arguments().at(1);
            full_path = QCoreApplication::arguments().at(2);
        }
        else {
            full_path = QCoreApplication::arguments().at(1);
        }
        QFileInfo info_file = QFileInfo(full_path);

        QDir directory = info_file.absoluteDir();
        QString file = info_file.fileName();
        QFileInfoList dat = directory.entryInfoList(QStringList() << file,QDir::Files);

        if (!dat.isEmpty())
        {
            foreach(QFileInfo file_, dat){

                Decompiler Dc;

                Dc.SetupGame(Game);
                Dc.ReadFile(file_.absoluteFilePath());
                Dc.WriteFile(file_.absoluteFilePath());
            }
        }
        else {
            display_text("No file was found.");
        }
    }


    return a.exec();
}
