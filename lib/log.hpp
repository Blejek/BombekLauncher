#pragma once

#include "core.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace bbl
{

class Logger {
public:
    enum class level {
        trace = 0,
        debug,
        info,
        warn,
        error,
        critical
    };

    struct item {
        string data;
        str logger;
        level level;
    };

public:
    static void init(str path = {"log.txt"});

    static ref<spdlog::logger> add(str name, const vec<std::pair<level, u16>>& colors);

    static ref<spdlog::logger> get(str name);

    inline static vec<item> logs;
private:
    inline static dict<string, ref<spdlog::logger>, StrHash, StrEq> m_loggers;
    inline static vec<spdlog::sink_ptr> m_sinks;
    inline static ref<spdlog::logger> m_default;
    inline static bool m_init = false;
};

template<FixedStr name>
class LoggerBase {
public:
    template <typename... Args>
    constexpr static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->trace(fmt, std::forward<Args>(args)...); }
    constexpr static void trace(str msg) { Logger::get(name)->trace(msg); }

    template <typename... Args>
    constexpr static void debug(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->debug(fmt, std::forward<Args>(args)...); }
    constexpr static void debug(str msg) { Logger::get(name)->debug(msg); }

    template <typename... Args>
    constexpr static void info(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->info(fmt, std::forward<Args>(args)...); }
    constexpr static void info(str msg) { Logger::get(name)->info(msg); }

    template <typename... Args>
    constexpr static void warn(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->warn(fmt, std::forward<Args>(args)...); }
    constexpr static void warn(str msg) { Logger::get(name)->warn(msg); }

    template <typename... Args>
    constexpr static void error(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->error(fmt, std::forward<Args>(args)...); }
    constexpr static void error(str msg) { Logger::get(name)->error(msg); }

    template <typename... Args>
    constexpr static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args) { Logger::get(name)->critical(fmt, std::forward<Args>(args)...); }
    constexpr static void critical(str msg) { Logger::get(name)->critical(msg); }

    template <typename... Args>
    constexpr static void assertion(bool val, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (!val) {
            critical("Assertion has failed:");
            critical(fmt, std::forward<Args>(args)...);
            __debugbreak();
        }
    }
    constexpr static void assertion(bool val, str msg) {
        if (!val) {
            critical("Assertion has failed:\n{}", msg);
            __debugbreak();
        }
    }
    constexpr static void assertion(bool val) {
        if (!val) {
            critical("Assertion has failed");
            __debugbreak();
        }
    }
};

}