#pragma once

#include "Log.h"

#ifdef SFMLENGINE_ENABLE_ASSERTS
	#define SFMLE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
	#define SFMLE_CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
	#define SFMLE_ASSERT(x, ...)
	#define SFMLE_CORE_ASSERT(x, ...)
#endif
