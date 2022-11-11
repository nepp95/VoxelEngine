﻿#pragma once

#include <iostream>
#include <sstream>

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

		static void ParseLocation(std::string file, int line)
		{
			std::stringstream ss;

			size_t pos = file.find("src");
			ss << "[" << file.substr(pos) << "]:" << line << ": ";
			LogMessage(ss.str());
		}

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

#define LOG_WITH_LOCATION(...) { \
	::VoxelEngine::Log::ParseLocation(__FILE__, __LINE__); \
	LOG(__VA_ARGS__); \
}

#define VE_CORE_INFO(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Info); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

#define VE_CORE_WARN(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Warn); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

#define VE_CORE_ERROR(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Error); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

// Only used in assertions! TODO: Make inaccessible global
#define VE_CORE_CRITICAL(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::LogLevel::Critical); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}