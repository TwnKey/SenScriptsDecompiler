#include "headers/utilities.h"

void display_text(const std::string& text) {
    QTextStream out(stdout);
    out << QString::fromStdString(text) << Qt::endl;
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
