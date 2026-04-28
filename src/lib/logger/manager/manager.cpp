#include "lib/logger/manager/manager.hpp"

#include <fmt/format.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "lib/logger/constants/constants.hpp"

std::shared_ptr<spdlog::logger> LogManager::_globalLogger;
std::shared_ptr<spdlog::logger> LogManager::_printLogger;

using namespace ServiceConstants::Log;

void LogManager::Init() {
    if (_printLogger != nullptr && _globalLogger != nullptr) {
        return;
    }
    _printLogger = spdlog::stdout_color_mt(Type::Print);
    _printLogger->set_pattern(Pattern::Print::Console);
    _printLogger->set_level(spdlog::level::trace);

    const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern(Pattern::Core::Console);

    const auto fileSink =
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/app_log.txt", MaxFileSize, MaxRotatingFiles);
    fileSink->set_pattern(Pattern::Core::File);

    _globalLogger = std::make_shared<spdlog::logger>(Type::Core, spdlog::sinks_init_list{consoleSink, fileSink});
    spdlog::register_logger(_globalLogger);
    _globalLogger->enable_backtrace(32);
    spdlog::flush_on(spdlog::level::err);
}

std::shared_ptr<spdlog::logger>& LogManager::Global() {
    return _globalLogger;
}

std::shared_ptr<spdlog::logger>& LogManager::Print() {
    return _printLogger;
}

void LogManager::InjectSpacer() {
    _globalLogger->set_pattern("\n");
    _globalLogger->critical("");
    const auto& sinks = _globalLogger->sinks();
    sinks[0]->set_pattern(Pattern::Core::Console);
    sinks[1]->set_pattern(Pattern::Core::File);
}

void LogManager::ReportCoreFatal(const char* file, int line, std::string_view message) {
    InjectSpacer();
    Global()->critical("---FATAL ERROR DETECTED----");
    Global()->critical("[{}:{}] {}", file, line, message);
    Global()->dump_backtrace();
    InjectSpacer();
    Global()->flush();
}

void LogManager::ReportPrintFatal(const char* file, int line, std::string_view message) {
    fmt::print("\n");
    Print()->critical("---FATAL ERROR DETECTED----");
    Print()->critical("[{}:{}] {}", file, line, message);
    Print()->dump_backtrace();
    fmt::print("\n");
    Print()->flush();
}
