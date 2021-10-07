#ifndef UTILITIES_H
#define UTILITIES_H
#include <QTextCodec>
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
#include "headers/spdlog.h"

namespace ssd {
/**
 * A replacement for QByteArray
 *
 * It only implements the QByteArray methods currently used in the codebase
 */
class Buffer : private std::vector<int8_t> {
  private:
    using base_vector = std::vector<int8_t>;

  public:
    using base_vector::base_vector;
    using size_type = typename base_vector::size_type;
    using iterator = typename base_vector::iterator;
    using const_iterator = typename base_vector::const_iterator;

    using base_vector::operator[];

    using base_vector::at;
    using base_vector::begin;
    using base_vector::clear;
    using base_vector::data;
    using base_vector::end;
    using base_vector::erase;
    using base_vector::push_back;
    using base_vector::reserve;
    using base_vector::resize;
    using base_vector::size;

    /**
     * This push_back is equivalent to QByteArray's append
     */
    void push_back(const Buffer& other) { this->insert(this->end(), std::begin(other), std::end(other)); }

    /**
     * This replace only replaces char, since that's all it's currently used for
     */
    void replace(const char s, const char r) { std::replace(this->begin(), this->end(), s, r); }

    static ssd::Buffer fromStdString(const std::string& std_string) { return { std::begin(std_string), std::end(std_string) }; }
};
} // namespace ssd

namespace ssd::utils {
ssd::Buffer read_file(const std::filesystem::path& filepath);
void write_file(const std::filesystem::path& filepath, const ssd::Buffer& content);
} // namespace ssd::utils

void display_text(const std::string& text);

std::string ReadStringFromByteArray(int start_pos, const ssd::Buffer& content);
int ReadIntegerFromByteArray(int start_pos, const ssd::Buffer& content);
int16_t ReadShortFromByteArray(int start_pos, const ssd::Buffer& content);
ssd::Buffer ReadSubByteArray(const ssd::Buffer& content, int& addr, size_t size);
ssd::Buffer ReadStringSubByteArray(const ssd::Buffer& content, int& addr);
float QByteArrayToFloat(const ssd::Buffer& content);
ssd::Buffer GetBytesFromFloat(float f);
bool isMultiple0x10(const std::string& fun_name);

template<typename T>
concept integral_least_16 = std::same_as<T, uint16_t> || std::same_as<T, int16_t> || std::same_as<T, uint32_t> ||
  std::same_as<T, int32_t> || std::same_as<T, int64_t> || std::same_as<T, uint64_t> || std::same_as<T, size_t>;

ssd::Buffer GetBytesFromInt(integral_least_16 auto i) {
    ssd::Buffer q_b;
    q_b.push_back(static_cast<int8_t>(i & 0x000000ff));
    q_b.push_back(static_cast<int8_t>((i & 0x0000ff00) >> 8));
    q_b.push_back(static_cast<int8_t>((i & 0x00ff0000) >> 16));
    q_b.push_back(static_cast<int8_t>((i) >> 24));

    return q_b;
}
ssd::Buffer GetBytesFromShort(integral_least_16 auto i) {
    ssd::Buffer q_b;
    q_b.push_back(static_cast<int8_t>(i & 0x00ff));
    q_b.push_back(static_cast<int8_t>((i & 0xff00) >> 8));

    return q_b;
}

#endif // UTILITIES_H
