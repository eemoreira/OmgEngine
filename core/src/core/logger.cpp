#include "logger.h"
#include <memory>

std::shared_ptr<spdlog::logger> Logger::s_core_logger;
std::shared_ptr<spdlog::logger> Logger::s_app_logger;

void Logger::init() {
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("%^[%T] %n: %v%$");

  s_core_logger = spdlog::stdout_color_mt("CORE");
  s_core_logger->set_level(spdlog::level::trace);

  s_app_logger = spdlog::stdout_color_mt("APP");
  s_app_logger->set_level(spdlog::level::trace);

}
