#include "headers/utilities.h"

void display_text(QString text) {
    QTextStream out(stdout);
    out << text << Qt::endl;
}
QString ConvertBytesToString(QByteArray Bytes) {
    QString DataAsString = QString::fromStdString(Bytes.toStdString());
    return DataAsString;
}
QString ReadStringFromByteArray(int start_pos, QByteArray& content) {

    QString filename = "";
    while (content.at(start_pos) != 0) {
        filename.push_back((char)content.at(start_pos));
        start_pos++;
    }

    return filename;
}
QByteArray ReadStringSubByteArray(QByteArray& content, int& addr) {
    QByteArray result;
    while (content.at(addr) != 0) {
        result.push_back(content.at(addr));
        addr++;
    }

    addr++;
    return result;
}
int ReadIntegerFromByteArray(int start_pos, QByteArray& content) {
    int size = ((static_cast<unsigned int>(content[start_pos + 0]) & 0xFF) << 0) +
               ((static_cast<unsigned int>(content[start_pos + 1]) & 0xFF) << 8) +
               ((static_cast<unsigned int>(content[start_pos + 2]) & 0xFF) << 16) +
               ((static_cast<unsigned int>(content[start_pos + 3]) & 0xFF) << 24);
    return size;
}
QByteArray GetBytesFromFloat(float f) {
    QByteArray array(reinterpret_cast<const char*>(&f), sizeof(f));
    return array;
}
bool isMultiple0x10(QString fun_name) {
    if ((fun_name == "PTN_TABLE") || (fun_name.startsWith("FC_auto")) || (fun_name.startsWith("_"))) {
        return true; //||(fun_name.startsWith("ReactionTable"))
    } else {
        return false;
    }
}
QByteArray GetBytesFromInt(int i) {
    QByteArray q_b;
    q_b.push_back(i & 0x000000ff);
    q_b.push_back((i & 0x0000ff00) >> 8);
    q_b.push_back((i & 0x00ff0000) >> 16);
    q_b.push_back((i) >> 24);

    return q_b;
}
QByteArray GetBytesFromShort(int16_t i) {
    QByteArray q_b;
    q_b.push_back(i & 0x00ff);
    q_b.push_back((i & 0xff00) >> 8);

    return q_b;
}
float QByteArrayToFloat(QByteArray& arr) // thanks to jabk https://stackoverflow.com/questions/36859447/qbytearray-to-float
{
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    quint32 temp =
      ((unsigned char)arr[3] << 24) | ((unsigned char)arr[2] << 16) | ((unsigned char)arr[1] << 8) | (unsigned char)arr[0]; // Big endian
    float* out = reinterpret_cast<float*>(&temp);

    return *out;
}

int16_t ReadShortFromByteArray(int start_pos, QByteArray& content) {
    int16_t size =
      ((static_cast<int16_t>(content[start_pos + 0]) & 0xFF) << 0) + ((static_cast<int16_t>(content[start_pos + 1]) & 0xFF) << 8);

    return size;
}
QByteArray ReadSubByteArray(QByteArray& content, int& addr, int size) {
    int start = addr;
    addr = start + size;

    return content.mid(start, size);
}
