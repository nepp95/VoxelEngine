#pragma once

#include <iostream>

namespace VoxelEngine
{
	enum class LogLevel
	{
		Info,
		Warn,
		Error,
		Critical
	};

	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetOutputColor(const LogLevel& level);
		static void ResetOutputColor();
		static void EndMessage();

		template<typename T>
		static void LogMessage(T t)
		{
			std::cout << t;
		}

		template<typename T, typename... Args>
		static void LogMessage(const char* chars, T t, Args... args)
		{
			// Reference: https://en.cppreference.com/w/cpp/language/parameter_pack

			for (; *chars != '\0'; chars++)
			{
				if (*chars == '%')
				{
					LogMessage(t);
					LogMessage(chars + 1, args...);
					return;
				}

				LogMessage(*chars);
			}
		}
	};
}

// Logging macros
#define LOG(...) { \
	::VoxelEngine::Log::LogMessage(__VA_ARGS__); \
	::VoxelEngine::Log::ResetOutputColor(); \
	::VoxelEngine::Log::EndMessage(); \
}

#define CORE_INFO(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Info); \
	LOG(__VA_ARGS__); \
}

#define CORE_WARN(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Warn); \
	LOG(__VA_ARGS__); \
}

#define CORE_ERROR(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Error); \
	LOG(__VA_ARGS__); \
}

// Only used in assertions! TODO: Make inaccessible global
#define CORE_CRITICAL(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Critical); \
	LOG(__VA_ARGS__); \
}
