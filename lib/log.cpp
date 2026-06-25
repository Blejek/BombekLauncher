#include "log.hpp"


#include <fstream>

namespace bbl
{

class sink_add_to_log final : public spdlog::sinks::base_sink<std::mutex> {
public:
    explicit sink_add_to_log() {}
    sink_add_to_log(const sink_add_to_log&) = delete;
    sink_add_to_log& operator=(const sink_add_to_log&) = delete;

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        spdlog::memory_buf_t formatted;
        base_sink<std::mutex>::formatter_->format(msg, formatted);
        Logger::logs.emplace_back(std::string(formatted.data(), static_cast<std::streamsize>(formatted.size())),
                                  msg.logger_name.data(),
                                  (Logger::level)msg.level);
    }

    void flush_() override {}
};

void Logger::init(str path) {
    if (m_init) {
        return;
    }
    m_sinks.push_back(newRef<sink_add_to_log>());

    std::string logPath{ path };
    m_sinks.push_back(newRef<spdlog::sinks::basic_file_sink_mt>(logPath, true));

    m_sinks[0]->set_pattern("%^[%T] %n: %v%$");
    m_sinks[1]->set_pattern("[%T] [%l] %n: %v");

    vec<std::pair<level, u16>> colors{
        {level::trace, 0x8},
        {level::debug, 0x7},
        {level::info, 0x2},
        {level::warn, 0x6},
        {level::error, 0xc},
        {level::critical, 0x4}
    };

    m_default = add("default", colors);
    add("core", colors);

    colors[2].second = 0xe;
    add("asset", colors);

    colors[2].second = 0xd;
    add("renderer", colors);

    colors[2].second = 0xb;
    add("game", colors);

    m_init = true;
}

ref<spdlog::logger> Logger::add(str name, const vec<std::pair<level, u16>>& colors) {
    vec<spdlog::sink_ptr> sinks(m_sinks.begin(), m_sinks.end());

    auto sink = newRef<spdlog::sinks::stdout_color_sink_mt>();
    sink->set_pattern("%T [%n] %^%v%$");

    for (const auto& [lev, col] : colors) {
        sink->set_color(static_cast<spdlog::level::level_enum>(lev), col);
    }
    sinks.push_back(sink);

    auto logger = newRef<spdlog::logger>(string(name), sinks.begin(), sinks.end());

    spdlog::register_logger(logger);
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);

    m_loggers[string(name)] = logger;
    return logger;
}

ref<spdlog::logger> Logger::get(str name) {
    Logger::init();

    if (auto l = m_loggers.find(name); l != m_loggers.end()) {
        return l->second;
    }
    return m_default;
}

}