#pragma once
#include <filesystem>

#include "spdlog/common.h"
#include "spdlog/spdlog.h"

#ifdef __cpp_lib_source_location
#include <source_location>
using source_location = std::source_location;
#else
#include <experimental/source_location>
using source_location = std::experimental::source_location;
#endif

namespace ssd::spdlog {
// convert std::source_location to spdlog's internal representation
[[nodiscard]] constexpr auto get_log_source_location(const source_location& location) {
    return ::spdlog::source_loc{ location.file_name(), static_cast<std::int32_t>(location.line()), location.function_name() };
}

// Rely on implicit conversion so we end up with both source_location and the format string
struct FormatWithLocation {
    std::string_view value;
    ::spdlog::source_loc loc;

    template<typename String>
    // NOLINTNEXTLINE(google-explicit-constructor)
    FormatWithLocation(const String& s, const source_location& location = source_location::current())
      : value{ s }
      , loc{ get_log_source_location(location) } {}
};

template<typename... Args>
void log(::spdlog::level::level_enum level, FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, level, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

template<typename... Args>
void err(FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, ::spdlog::level::err, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

template<typename... Args>
void info(FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, ::spdlog::level::info, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

template<typename... Args>
void warn(FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, ::spdlog::level::warn, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

template<typename... Args>
void trace(FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, ::spdlog::level::trace, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

template<typename... Args>
void debug(FormatWithLocation fmt, Args&&... args) {
    ::spdlog::default_logger_raw()->log(fmt.loc, ::spdlog::level::debug, fmt::runtime(fmt.value), std::forward<Args>(args)...);
}

void init(::spdlog::level::level_enum default_log_level, const std::filesystem::path& log_path);

} // namespace ssd::spdlog
