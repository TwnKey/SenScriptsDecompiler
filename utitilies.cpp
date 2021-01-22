#include "utilities.h"
void display_text(QString text){
    QTextStream out(stdout);
    out << text << endl;
}

QString ReadStringFromByteArray(int start_pos, QByteArray &content){
    int idx_name = start_pos;
    QString filename = "";
    while (content.at(idx_name)!=0) {
        filename.push_back((char)content.at(idx_name));
        idx_name++;
    }
    return filename;
}
int ReadIntegerFromByteArray(int start_pos, QByteArray &content){
    int size = ((static_cast<unsigned int>(content[start_pos+0]) & 0xFF) << 0)
            + ((static_cast<unsigned int>(content[start_pos+1]) & 0xFF) << 8)
            + ((static_cast<unsigned int>(content[start_pos+2]) & 0xFF) << 16)
            + ((static_cast<unsigned int>(content[start_pos+3]) & 0xFF) << 24);
    return size;

}
short ReadShortFromByteArray(int start_pos, QByteArray &content){
    short size = ((static_cast<short>(content[start_pos+0]) & 0xFF) << 0)
             + ((static_cast<short>(content[start_pos+1]) & 0xFF) << 8);
    return size;
}
