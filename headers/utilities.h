#ifndef UTILITIES_H
#define UTILITIES_H
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <iomanip>
#include <sstream>
void display_text(QString text);

QString ReadStringFromByteArray(int start_pos, QByteArray& content);
int ReadIntegerFromByteArray(int start_pos, QByteArray& content);
int16_t ReadShortFromByteArray(int start_pos, QByteArray& content);
QByteArray ReadSubByteArray(QByteArray& content, int& addr, int size);
QByteArray ReadStringSubByteArray(QByteArray& content, int& addr);
QString ConvertBytesToString(QByteArray Bytes);
float QByteArrayToFloat(QByteArray& content);
QByteArray GetBytesFromInt(int i);
QByteArray GetBytesFromFloat(float f);
QByteArray GetBytesFromShort(int16_t i);
bool isMultiple0x10(QString fun_name);
#endif // UTILITIES_H
