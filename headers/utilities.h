#ifndef UTILITIES_H
#define UTILITIES_H
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <iomanip>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "headers/exceptions.h"

void display_text(const QString& text);

std::string ReadStringFromByteArray(int start_pos, const QByteArray& content);
int ReadIntegerFromByteArray(int start_pos, const QByteArray& content);
int16_t ReadShortFromByteArray(int start_pos, const QByteArray& content);
QByteArray ReadSubByteArray(const QByteArray& content, int& addr, int size);
QByteArray ReadStringSubByteArray(const QByteArray& content, int& addr);
float QByteArrayToFloat(const QByteArray& content);
QByteArray GetBytesFromFloat(float f);
bool isMultiple0x10(const std::string& fun_name);

template<typename T>
concept integral_least_16 = std::same_as<T, uint16_t> || std::same_as<T, int16_t> || std::same_as<T, uint32_t> ||
  std::same_as<T, int32_t> || std::same_as<T, int64_t> || std::same_as<T, uint64_t> || std::same_as<T, size_t>;

QByteArray GetBytesFromInt(integral_least_16 auto i) {
    QByteArray q_b;
    q_b.push_back(static_cast<int8_t>(i & 0x000000ff));
    q_b.push_back(static_cast<int8_t>((i & 0x0000ff00) >> 8));
    q_b.push_back(static_cast<int8_t>((i & 0x00ff0000) >> 16));
    q_b.push_back(static_cast<int8_t>((i) >> 24));

    return q_b;
}
QByteArray GetBytesFromShort(integral_least_16 auto i) {
    QByteArray q_b;
    q_b.push_back(static_cast<int8_t>(i & 0x00ff));
    q_b.push_back(static_cast<int8_t>((i & 0xff00) >> 8));

    return q_b;
}

#endif // UTILITIES_H
