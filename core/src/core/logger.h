#pragma once

#include <memory>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>

struct Logger {
public:
  inline static std::shared_ptr<spdlog::logger> get_core_logger() { return s_core_logger; }
  inline static std::shared_ptr<spdlog::logger> get_app_logger() { return s_app_logger; }
  static void init();

private:
  static std::shared_ptr<spdlog::logger> s_core_logger;
  static std::shared_ptr<spdlog::logger> s_app_logger;
};

// clang-format off
#define CORE_LOG_TRACE(...)   Logger::get_core_logger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)    Logger::get_core_logger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)    Logger::get_core_logger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)   Logger::get_core_logger()->error(__VA_ARGS__)

#define APP_LOG_TRACE(...)    Logger::get_app_logger()->trace(__VA_ARGS__)
#define APP_LOG_INFO(...)     Logger::get_app_logger()->info(__VA_ARGS__)
#define APP_LOG_WARN(...)     Logger::get_app_logger()->warn(__VA_ARGS__)
#define APP_LOG_ERROR(...)    Logger::get_app_logger()->error(__VA_ARGS__)
// clang-format on
