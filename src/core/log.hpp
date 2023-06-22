
#pragma once

#include <memory>

#define FMT_DEPRECATED
#include "core/core.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace cppsim {

class Log
{
  public:
    Log() = delete;
    ~Log() = delete;

    static void init();
    inline static std::shared_ptr<spdlog::logger>& get_logger() { return logger; }
    inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return core_logger; }
    inline static std::shared_ptr<spdlog::logger>& get_error_logger() { return error_logger; }

  private:
    static std::shared_ptr<spdlog::logger> logger;
    static std::shared_ptr<spdlog::logger> core_logger;
    static std::shared_ptr<spdlog::logger> error_logger;
};

} // namespace cppsim

#ifdef CPPSIM_ENABLE_LOGGING
#define CPPSIM_ERROR(...) ::cppsim::Log::get_error_logger()->error(__VA_ARGS__)
#define CPPSIM_WARN(...) ::cppsim::Log::get_logger()->warn(__VA_ARGS__)
#define CPPSIM_INFO(...) ::cppsim::Log::get_logger()->info(__VA_ARGS__)
#define CPPSIM_TRACE(...) ::cppsim::Log::get_logger()->trace(__VA_ARGS__)
#define CPPSIM_CRITICAL(...) ::cppsim::Log::get_logger()->critical(__VA_ARGS__)
#else
#define CPPSIM_ERROR(...)
#define CPPSIM_WARN(...)
#define CPPSIM_INFO(...)
#define CPPSIM_TRACE(...)
#define CPPSIM_CRITICAL(...)
#endif

// These are always present. Use for logging that is actually part of the program
#define CPPSIM_CORE_ERROR(...) ::cppsim::Log::get_core_logger()->error(__VA_ARGS__)
#define CPPSIM_CORE_WARN(...) ::cppsim::Log::get_core_logger()->warn(__VA_ARGS__)
#define CPPSIM_CORE_INFO(...) ::cppsim::Log::get_core_logger()->info(__VA_ARGS__)
#define CPPSIM_CORE_TRACE(...) ::cppsim::Log::get_core_logger()->trace(__VA_ARGS__)
#define CPPSIM_CORE_CRITICAL(...) ::cppsim::Log::get_core_logger()->critical(__VA_ARGS__)