#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include "headers/Builder.h"
#include "headers/functions.h"

namespace ssd {
enum class ForeignDatFormat {
    EXCEL = 0,
};

struct DatDataReadResult {
    std::string game_code;
    std::string scene_name;
    std::vector<Function> functions;
};

class ForeignDatDataReader {
  public:
    virtual ~ForeignDatDataReader();
    virtual DatDataReadResult from_file(const std::filesystem::path& filepath) = 0;
};

class ForeignDatDataWriter {
  public:
    virtual ~ForeignDatDataWriter();
    virtual void to_file(const std::vector<Function>& functions, const std::filesystem::path& filepath) = 0;
};

/**
 * NOTE: Builder is only needed for the create_instruction and create_instruction_from_dat methods
 */
class ForeignDatDataConverter {
  public:
    static std::unique_ptr<ForeignDatDataReader> get_reader(ForeignDatFormat format, const std::string& encoding, Builder* builder);
    static std::unique_ptr<ForeignDatDataWriter> get_writer(ForeignDatFormat format,
                                                            const std::string& game_code,
                                                            const std::string& encoding,
                                                            Builder* builder);
};
}; // namespace ssd
