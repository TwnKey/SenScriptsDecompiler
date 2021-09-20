#include "headers/utilities.h"
#include <fstream>

namespace ssd::utils {

namespace fs = std::filesystem;
std::vector<std::filesystem::directory_entry> find_files(const std::filesystem::path& root,
                                                         bool recursive,
                                                         const std::set<std::string>& extensions) {
    auto&& result = std::vector<fs::directory_entry>{};

    auto filter = [extensions](const auto& entry) {
        if (!fs::is_regular_file(entry)) return false;
        if (!extensions.empty()) {
            return extensions.count(entry.path().extension().string()) > 0;
        }
        return true;
    };

    if (recursive) {
        auto it = fs::recursive_directory_iterator{ root };
        std::copy_if(begin(it), end(it), back_inserter(result), filter);
    } else {
        auto it = fs::directory_iterator{ root };
        std::copy_if(begin(it), end(it), back_inserter(result), filter);
    }

    return std::move(result);
}

QByteArray read_file(const std::filesystem::path& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    file.exceptions(std::ifstream::badbit);

    std::vector<char> temp_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return QByteArray(reinterpret_cast<const char*>(temp_content.data()), (int)temp_content.size());
}

void write_file(const std::filesystem::path& filename, const QByteArray& contents) {

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    file.exceptions(std::ofstream::badbit);

    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    file.close();
}

} // namespace ssd::utils

void display_text(const QString& text) {
    QTextStream out(stdout);
    out << text << Qt::endl;
}

std::string ReadStringFromByteArray(int start_pos, const QByteArray& content) {

    std::string filename;
    while (content.at(start_pos) != 0) {
        filename.push_back(content.at(start_pos));
        start_pos++;
    }

    return filename;
}
QByteArray ReadStringSubByteArray(const QByteArray& content, int& addr) {
    QByteArray result;
    while (content.at(addr) != 0) {
        result.push_back(content.at(addr));
        addr++;
    }
    result.push_back(content.at(addr)); // should be the null byte
    addr++;
    return result;
}
int ReadIntegerFromByteArray(int start_pos, const QByteArray& content) {
    int size = static_cast<int>(((static_cast<unsigned int>(content[start_pos + 0]) & 0xFF) << 0) +
                                ((static_cast<unsigned int>(content[start_pos + 1]) & 0xFF) << 8) +
                                ((static_cast<unsigned int>(content[start_pos + 2]) & 0xFF) << 16) +
                                ((static_cast<unsigned int>(content[start_pos + 3]) & 0xFF) << 24));
    return size;
}
QByteArray GetBytesFromFloat(float f) {
    QByteArray array(reinterpret_cast<const char*>(&f), sizeof(f));
    return array;
}
bool isMultiple0x10(const std::string& fun_name) {
    return (fun_name == "PTN_TABLE") || (fun_name.starts_with("FC_auto")) || (fun_name.starts_with("_"));
}

float QByteArrayToFloat(const QByteArray& content) // thanks to jabk https://stackoverflow.com/questions/36859447/qbytearray-to-float
{
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    quint32 temp = ((unsigned char)content[3] << 24) | ((unsigned char)content[2] << 16) | ((unsigned char)content[1] << 8) |
                   (unsigned char)content[0]; // Big endian
    auto* out = reinterpret_cast<float*>(&temp);

    return *out;
}

int16_t ReadShortFromByteArray(int start_pos, const QByteArray& content) {
    int16_t size = static_cast<int16_t>(((static_cast<int16_t>(content[start_pos + 0]) & 0xFF) << 0) +
                                        ((static_cast<int16_t>(content[start_pos + 1]) & 0xFF) << 8));

    return size;
}
QByteArray ReadSubByteArray(const QByteArray& content, int& addr, int size) {
    int start = addr;
    addr = start + size;

    return content.mid(start, size);
}
