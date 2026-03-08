#pragma once

#include <iostream>
#include <string_view>
#include <format>
#include <chrono>
#include <source_location>

namespace Mines {
    inline std::string_view getFilename(const char* path) {
        std::string_view sv{ path };
        auto pos = sv.find_last_of("/\\");
        return (pos == std::string_view::npos) ? sv : sv.substr(pos + 1);
    }

    template<typename... Args>
    inline void log(
        std::string_view level,
        std::string_view color,
        const std::source_location& loc,
        std::format_string<Args...> fmt,
        Args&&... args
    ) {
        auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
        auto local_now = std::chrono::zoned_time{ std::chrono::current_zone(), now };
        std::string user_msg = std::format(fmt, std::forward<Args>(args)...);

        std::cout << std::format(
            "{}[{:%H:%M:%S}] [{}] [{}:{}] [{}] {}\033[0m\n",
            color,
            local_now,
            level,
            getFilename(loc.file_name()),
            loc.line(),
            loc.function_name(),
            user_msg
        );
    }
} // namespace Mines

#ifndef NDEBUG
#define LOG_INFO(fmt, ...)  Mines::log("INFO", "\033[92m", std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Mines::log("WARN", "\033[93m", std::source_location::current(), fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Mines::log("ERROR", "\033[91m", std::source_location::current(), fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#define LOG_WARN(fmt, ...)
#define LOG_ERROR(fmt, ...)
#endif
