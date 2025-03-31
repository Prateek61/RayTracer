#pragma once

#include "Engine/Commons/PlatformDetection.h"

#define ENABLE_CORE_LOG
#define ENABLE_CLIENT_LOG
#define ENABLE_LUA_LOG

#define ENABLE_ASSERTS
#define ENABLE_PERFORMANCE_PROFILE

#define BIT(x) (1 << (x))

#if defined(BUILD_DEBUG) || defined(BUILD_RELEASE)
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x)
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

// Debug Break
#ifndef BUILD_DIST
	#if defined(PLATFORM_WINDOWS)
		#define DEBUGBREAK() __debugbreak()
	#elif defined(PLATFORM_LINUX)
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define DEBUGBREAK()
#endif
