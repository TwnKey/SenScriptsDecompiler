#ifndef READER_H
#define READER_H

#include "headers/translationfile.h"

#include <QFile>
#include <QString>
#include <filesystem>
#include <memory>

/*
The Decompiler class is here to write the XLSX and DAT output files, as well as setting some global parameters such as
the game name, or run regression tests The point is that through the two reading functions, you will build a Translation
File object that could then be written in both output format

SetupGame: selects the correct Builder depending on which game name is provided to the program
ReadXLSX: calls the builder's reading function for XLSX files, then update the pointers (executes the whole reading
process in fact) ReadDAT: calls the builder's reading function for DAT files, then update the pointers ReadFile: selects
the correct reading function depending on the provided file path UpdateCurrentTF: transfers all functions from the
builder to the translation file object WriteXLSX: writes the translation file to an xlsx spreadsheet WriteDAT: writes
the translation file to a dat file readable by the game CheckAllFiles: runs a regression test GetTF: returns the
translation file being edited WriteFile: writes the correct file depending on the provided file path
*/
class Decompiler {
  public:
    Decompiler();
    bool setup_game(const std::string& game);
    bool read_xlsx(const std::filesystem::path& filename);
    bool read_dat(QFile& filename);
    bool read_file(const std::filesystem::path& filepath);
    bool update_current_tf();
    bool write_xlsx(const std::filesystem::path& output_dir);
    bool write_dat(const std::filesystem::path& output_dir);
    bool check_all_files(const std::filesystem::path& log_filename,
                         const std::vector<std::filesystem::path>& files_to_read,
                         const std::filesystem::path& reference_dir,
                         const std::filesystem::path& output_dir);
    TranslationFile get_tf();
    bool write_file(const std::filesystem::path& filepath, const std::filesystem::path& output_dir);

  private:
    TranslationFile current_tf;
    std::string _game;
    std::unique_ptr<Builder> ib = nullptr;
};

#endif // READER_H
