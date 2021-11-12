#include "headers/data_converter.h"
#include "headers/excel_converter.h"

namespace ssd {
ForeignDatDataReader::~ForeignDatDataReader() = default;
ForeignDatDataWriter::~ForeignDatDataWriter() = default;

std::unique_ptr<ForeignDatDataReader> ForeignDatDataConverter::get_reader(ForeignDatFormat format,
                                                                          const std::string& encoding,
                                                                          Builder* builder) {
    switch (format) {
        case ForeignDatFormat::EXCEL:
        default:
            return std::make_unique<ExcelDatDataReader>(encoding, builder);
    }
}

std::unique_ptr<ForeignDatDataWriter> ForeignDatDataConverter::get_writer(ForeignDatFormat format,
                                                                          const std::string& game_code,
                                                                          const std::string& encoding,
                                                                          Builder* builder) {
    switch (format) {
        case ForeignDatFormat::EXCEL:
        default:
            return std::make_unique<ExcelDatDataWriter>(game_code, encoding, builder);
    }
}
}; // namespace ssd
