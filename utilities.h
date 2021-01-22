#ifndef UTILITIES_H
#define UTILITIES_H
#include <QTextStream>

void display_text(QString text);


QString ReadStringFromByteArray(int start_pos, QByteArray &content);
int ReadIntegerFromByteArray(int start_pos, QByteArray &content);
short ReadShortFromByteArray(int start_pos, QByteArray &content);
#endif // UTILITIES_H
