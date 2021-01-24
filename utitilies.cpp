#include "utilities.h"

void display_text(QString text){
    QTextStream out(stdout);
    out << text << endl;
}
QString ConvertBytesToString(QByteArray Bytes){
    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(Bytes);
    return DataAsString;
}
QString ReadStringFromByteArray(int start_pos, QByteArray &content){

    QString filename = "";
    while (content.at(start_pos)!=0) {
        filename.push_back((char)content.at(start_pos));
        start_pos++;
    }

    return filename;
}
QByteArray ReadStringSubByteArray(QByteArray &content, int &addr){
    QByteArray result;
    while (content.at(addr)!=0) {
        result.push_back(content.at(addr));
        addr++;
    }
    addr++;
    return result;
}
int ReadIntegerFromByteArray(int start_pos, QByteArray &content){
    int size = ((static_cast<unsigned int>(content[start_pos+0]) & 0xFF) << 0)
            + ((static_cast<unsigned int>(content[start_pos+1]) & 0xFF) << 8)
            + ((static_cast<unsigned int>(content[start_pos+2]) & 0xFF) << 16)
            + ((static_cast<unsigned int>(content[start_pos+3]) & 0xFF) << 24);
    start_pos+=4;
    return size;

}
short ReadShortFromByteArray(int start_pos, QByteArray &content){
    short size = ((static_cast<short>(content[start_pos+0]) & 0xFF) << 0)
             + ((static_cast<short>(content[start_pos+1]) & 0xFF) << 8);
    start_pos+=2;
    return size;
}
QByteArray ReadSubByteArray(QByteArray &content, int &addr, int size){
    int start = addr;
    addr = start + size;

    return content.mid(start, size);
}
