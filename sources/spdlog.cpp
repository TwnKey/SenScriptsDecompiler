#include "spdlog/cfg/env.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "headers/spdlog.h"

namespace ssd::spdlog {
/**
 * Simple log initialization, don't log to file if log_level == spdlog::level::off
 * unless overriden by SPDLOG_LEVEL environment variable
 * ex: SPDLOG_LEVEL=level_for_all_loggers,specific_logger=trace ./SenScriptsDecompiler
 */
void init(::spdlog::level::level_enum default_log_level, const std::filesystem::path& log_path) {
    try {
        auto logger = ::spdlog::default_logger();

        logger->set_level(default_log_level);
        // set log levels via SPDLOG_LEVEL environment variable
        ::spdlog::cfg::load_env_levels();

        auto console_sink = logger->sinks().at(0);
        // See https://github.com/gabime/spdlog/wiki/3.-Custom-formatting for formatting details
        console_sink->set_pattern("[%s:%!]%^[%l]%$ %v");
        console_sink->set_level(::spdlog::level::info);

        if (logger->level() != ::spdlog::level::off) {
            auto file_sink = std::make_shared<::spdlog::sinks::basic_file_sink_mt>(log_path.string(), true);
            file_sink->set_pattern("[%s:%!] %v");

            logger->sinks().push_back(file_sink);
        }

    } catch (const ::spdlog::spdlog_ex& ex) {
        throw std::runtime_error(fmt::format("Log initialization failed: {}", ex.what()));
    }
}

} // namespace ssd::spdlog
