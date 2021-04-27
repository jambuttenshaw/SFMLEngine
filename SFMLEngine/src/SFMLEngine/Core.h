#pragma once

#define ENABLE_PROFILER

#ifdef ENABLE_PROFILER
	#include <Tracy.hpp>
#else
	#define ZoneScoped
	#define ZoneName(...)
	#define FrameMark
#endif


#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif


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

#ifdef SFMLENGINE_DEBUG
	// engine log macros
	#define LOG_CORE_ERROR(...)     ::SFMLEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define LOG_CORE_WARN(...)      ::SFMLEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define LOG_CORE_INFO(...)      ::SFMLEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define LOG_CORE_TRACE(...)     ::SFMLEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

	// app log macros
	#define LOG_ERROR(...)			::SFMLEngine::Log::GetClientLogger()->error(__VA_ARGS__)
	#define LOG_WARN(...)			::SFMLEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define LOG_INFO(...)			::SFMLEngine::Log::GetClientLogger()->info(__VA_ARGS__)
	#define LOG_TRACE(...)			::SFMLEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
	#define LOG_CORE_ERROR(...)     
	#define LOG_CORE_WARN(...)      
	#define LOG_CORE_INFO(...)      
	#define LOG_CORE_TRACE(...)     

	#define LOG_ERROR(...)			
	#define LOG_WARN(...)			
	#define LOG_INFO(...)			
	#define LOG_TRACE(...)			
#endif


#ifdef SFMLENGINE_ENABLE_ASSERTS
	#define SFMLE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
	#define SFMLE_CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
	#define SFMLE_ASSERT(x, ...)
	#define SFMLE_CORE_ASSERT(x, ...)
#endif
