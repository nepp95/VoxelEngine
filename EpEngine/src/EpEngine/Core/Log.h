#pragma once

#include <iostream>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace EpEngine
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

// Log vectors
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

// Log matrices
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

// Logging macros
#define LOG(...) { \
	::EpEngine::Log::LogMessage(__VA_ARGS__); \
	::EpEngine::Log::ResetOutputColor(); \
	::EpEngine::Log::EndMessage(); \
}

#define LOG_WITH_LOCATION(...) { \
	::EpEngine::Log::ParseLocation(__FILE__, __LINE__); \
	LOG(__VA_ARGS__); \
}

#define EP_CORE_INFO(...) { \
	::EpEngine::Log::SetOutputColor(::EpEngine::LogLevel::Info); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

#define EP_CORE_WARN(...) { \
	::EpEngine::Log::SetOutputColor(::EpEngine::LogLevel::Warn); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

#define EP_CORE_ERROR(...) { \
	::EpEngine::Log::SetOutputColor(::EpEngine::LogLevel::Error); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}

// Only used in assertions! TODO: Make inaccessible global
#define EP_CORE_CRITICAL(...) { \
	::EpEngine::Log::SetOutputColor(::EpEngine::LogLevel::Critical); \
	LOG_WITH_LOCATION(__VA_ARGS__); \
}
