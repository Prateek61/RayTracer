#pragma once

#include "Commons/Defines.h"
#include <memory>

// Ignore all warning raise indise External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Engine
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static std::shared_ptr<spdlog::logger>& GetLuaLogger() { return s_LuaLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_LuaLogger;
	};
}

// Engine log macros
#ifdef ENABLE_CORE_LOG
#define LOG_CORE_FATAL(...)   ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LOG_CORE_ERROR(...)   ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...)    ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...)    ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_TRACE(...)   ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#else
#define CORE_FATAL(...)
#define CORE_ERROR(...)
#define CORE_WARN(...)
#define CORE_INFO(...)
#define CORE_TRACE(...)
#endif

// Client log macros
#ifdef ENABLE_CLIENT_LOG
#define LOG_FATAL(...)        ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_ERROR(...)        ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)         ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)         ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)        ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
#define FATAL(...)
#define ERROR(...)
#define WARN(...)
#define INFO(...)
#define TRACE(...)
#endif
