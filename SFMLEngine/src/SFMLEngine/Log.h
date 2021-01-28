#pragma once

#include "spdlog/spdlog.h"

#include <memory>

namespace SFMLEngine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// engine log macros
#define LOG_CORE_ERROR(...)     ::SFMLEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...)      ::SFMLEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...)      ::SFMLEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_TRACE(...)     ::SFMLEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

// app log macros
#define LOG_ERROR(...)     ::SFMLEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)      ::SFMLEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)      ::SFMLEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)     ::SFMLEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
