#pragma once

#include "Commons/Defines.h"

#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

namespace Engine
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json");
		void EndSession();
		void WriteProfile(const ProfileResult& result);
		static Instrumentor& Get();
		static bool IsProfilingEnabled();
		static void EnableProfiling();
		static void DisableProfiling();

	private:
		Instrumentor();
		~Instrumentor();
		void WriteHeader();
		void WriteFooter();

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession();

		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		bool m_ProfilingEnabled = true;
	};


	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name);
		~InstrumentationTimer();
		void Stop();

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		bool m_Stopped;
	};


	namespace InstrumentorUtils
	{
		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t src_index = 0;
			size_t dst_index = 0;
			while (src_index < N)
			{
				size_t match_index = 0;
				while (match_index < K - 1 && src_index + match_index < N - 1 && expr[src_index + match_index] ==
					remove[match_index]) match_index++;
				if (match_index == K - 1) src_index += match_index;
				result.Data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
				src_index++;
			}

			return result;
		}
	}
}

#ifdef ENABLE_PERFORMANCE_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define ENGINE_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define ENGINE_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define ENGINE_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define ENGINE_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define ENGINE_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define ENGINE_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define ENGINE_FUNC_SIG __func__
#else
#define ENGINE_FUNC_SIG "ENGINE_FUNC_SIG unknown!"
#endif

#define PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::Get().BeginSession(name, filepath)
#define PROFILE_END_SESSION() ::Engine::Instrumentor::Get().EndSession()
#define ENGINE_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Engine::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
												   ::Engine::InstrumentationTimer timer##line(fixedName##line.Data)
#define ENGINE_PROFILE_SCOPE_LINE(name, line) ENGINE_PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) ENGINE_PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(ENGINE_FUNC_SIG)

#define PROFILE_ENABLE() ::Engine::Instrumentor::EnableProfiling()
#define PROFILE_DISABLE() ::Engine::Instrumentor::DisableProfiling()
#else
#define PROFILE_BEGIN_SESSION(name, filepath)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()

#define PROFILE_ENABLE()
#define PROFILE_DISABLE()
#endif