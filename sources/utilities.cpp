#include "headers/utilities.h"
#include <fstream>
#include <iostream>

namespace ssd::utils {
namespace fs = std::filesystem;
ssd::Buffer read_file(const std::filesystem::path& filepath) {
    if (!fs::exists(filepath)) throw std::runtime_error("ERROR: File \"" + filepath.string() + "\" does not exist");
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    file.open(filepath, std::ios::in | std::ios::binary);

    ssd::Buffer temp_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return temp_content;
}

void write_file(const std::filesystem::path& filepath, const ssd::Buffer& content) {
    auto dir = filepath.parent_path();
    if (!fs::exists(dir)) throw std::runtime_error("ERROR: Directory \"" + dir.string() + "\" does not exist");

    std::ofstream file;
    file.exceptions(std::ofstream::badbit);
    file.open(filepath, std::ios::out | std::ios::binary);

    file.write(reinterpret_cast<const char*>(content.data()), std::ssize(content));
    file.close();
}
} // namespace ssd::utils

void display_text(const std::string& text) { std::cout << text << std::endl; }

std::string ReadStringFromByteArray(int start_pos, const ssd::Buffer& content) {

    std::string filename;
    while (content.at(start_pos) != 0) {
        filename.push_back(content.at(start_pos));
        start_pos++;
    }

    return filename;
}
ssd::Buffer ReadStringSubByteArray(const ssd::Buffer& content, int& addr) {
    ssd::Buffer result;
    while (content.at(addr) != 0) {
        result.push_back(content.at(addr));
        addr++;
    }
    result.push_back(content.at(addr)); // should be the null byte
    addr++;
    return result;
}
int ReadIntegerFromByteArray(int start_pos, const ssd::Buffer& content) {
    int size = static_cast<int>(((static_cast<unsigned int>(content[start_pos + 0]) & 0xFF) << 0) +
                                ((static_cast<unsigned int>(content[start_pos + 1]) & 0xFF) << 8) +
                                ((static_cast<unsigned int>(content[start_pos + 2]) & 0xFF) << 16) +
                                ((static_cast<unsigned int>(content[start_pos + 3]) & 0xFF) << 24));
    return size;
}
ssd::Buffer GetBytesFromFloat(float f) {
    ssd::Buffer data(sizeof(f));
    std::memcpy(data.data(), &f, sizeof(f));

    return data;
}
bool isMultiple0x10(const std::string& fun_name) {
    return (fun_name == "PTN_TABLE") || (fun_name.starts_with("FC_auto")) || (fun_name.starts_with("_"));
}

float QByteArrayToFloat(const ssd::Buffer& content) // thanks to jabk https://stackoverflow.com/questions/36859447/qbytearray-to-float
{
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    uint32_t temp = ((unsigned char)content[3] << 24) | ((unsigned char)content[2] << 16) | ((unsigned char)content[1] << 8) |
                    (unsigned char)content[0]; // Big endian
    auto* out = reinterpret_cast<float*>(&temp);

    return *out;
}

int16_t ReadShortFromByteArray(int start_pos, const ssd::Buffer& content) {
    auto size = static_cast<int16_t>(((static_cast<int16_t>(content[start_pos + 0]) & 0xFF) << 0) +
                                     ((static_cast<int16_t>(content[start_pos + 1]) & 0xFF) << 8));

    return size;
}
ssd::Buffer ReadSubByteArray(const ssd::Buffer& content, int& addr, size_t size) {
    int start = addr;
    addr = start + static_cast<int>(size);

    return { std::begin(content) + start, std::begin(content) + start + static_cast<int>(size) };
}
