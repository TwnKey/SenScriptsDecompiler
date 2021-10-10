#include "headers/decompiler.h"
#include "headers/global_vars.h"
#include <QCoreApplication>
#include <QSettings>

namespace fs = std::filesystem;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,fuchsia-statically-constructed-objects)
std::string InputDatFileEncoding;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,fuchsia-statically-constructed-objects)
std::string OutputDatFileEncoding;

std::vector<std::string> get_header() {
    return { ".                           ,##(&%%@                                            ",
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
             "(/(/////%            ,(/%*,  .,,((..,#..*.(,(,*##%%%%(,,.,*/#(..(, ,.*...#**##*." };
}

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    auto log_file = fs::path(settings.value("log_file", "log.txt").toString().toStdString());
    auto log_level = spdlog::level::from_str(settings.value("log_level", "off").toString().toStdString());

    auto Game = settings.value("Game", "cs4").toString().toStdString();
    InputDatFileEncoding = settings.value("InputDatFileEncoding", "UTF-8").toString().toStdString();
    OutputDatFileEncoding = settings.value("OutputDatFileEncoding", "UTF-8").toString().toStdString();
    fs::path output_dir = fs::path(QCoreApplication::applicationDirPath().toStdString()) / "recompiled_files";

    auto log_path = log_file.is_relative() ? fs::path(QCoreApplication::applicationDirPath().toStdString()) / log_file : log_file;
    ssd::spdlog::init(log_level, log_path);
    if (argc < 2) {
        for (const auto& i : get_header()) {
            display_text(i);
        }
        display_text("----------------------------------------------------------------------------------------------------------");
        display_text("This tool was first meant for translation projects of the Trails of Cold Steel games, starting with ToCS3.");
        display_text("Then it has become a tool for modding in general.");
        display_text("---------------------------------------------HOW TO USE---------------------------------------------------");
        display_text("Here's how to use it:");
        display_text("1- SenScriptsDecompiler.exe <filepath_to_convert>");
        display_text("2- SenScriptsDecompiler.exe <Game> <filepath_to_convert>");
        display_text("3- SenScriptsDecompiler.exe <Game> <filepath_to_convert> <output_folder>");
        display_text("Encodings can be defined in the config.ini file.");
        display_text("The output format is decided by the extension of the input file: XLSX gives a DAT and DAT gives a XLSX.");
        display_text("If you need more assistance, please join the discord server for modding Trails games: "
                     "https://discord.gg/XpFrXWht6j");
        display_text("Credits to NZerker and kirigaia for their support and testing.");
    } else if (argc >= 2) {
        fs::path full_path = QCoreApplication::arguments().at(1).toStdString();
        if (argc == 3) {
            Game = QCoreApplication::arguments().at(1).toStdString();
            full_path = QCoreApplication::arguments().at(2).toStdString();
        } else if (argc == 4) {
            Game = QCoreApplication::arguments().at(1).toStdString();
            full_path = QCoreApplication::arguments().at(2).toStdString();
            output_dir = fs::path(QCoreApplication::arguments().at(3).toStdString());
        }

        std::vector<fs::path> files{ full_path };
        if (!files.empty()) {
            for (const auto& file : files) {
                try {
                    Decompiler Dc;
                    Dc.setup_game(Game);
                    Dc.read_file(fs::absolute(file));
                    Dc.write_file(fs::absolute(file), output_dir);
                } catch (const std::exception& e) {
                    display_text(e.what());
                }
            }
        } else {
            display_text("No file was found.");
        }
    }

    // Asynchronous loggers must be shutdown manually for Windows
    spdlog::shutdown();

    return 0;
}
