// VoxelEngine - Engine
// Log.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 26-08-2022 21:45
// Last update: 29-08-2022 20:38

#pragma once

#include <iostream>

namespace VoxelEngine
{
	namespace Log
	{
		enum class LogLevel
		{
			Info,
			Warn,
			Error,
			Critical
		};

		inline void SetOutputColor(const LogLevel& level)
		{
			if (level == LogLevel::Info)
				std::cout << "\033[36m";
			if (level == LogLevel::Warn)
				std::cout << "\033[33m";
			if (level == LogLevel::Error)
				std::cout << "\033[1;31m";
			if (level == LogLevel::Critical)
				std::cout << "\033[31m";
		}

		inline void ResetOutputColor()
		{
			std::cout << "\33[0m";
		}

		template <typename T>
		void LogMessage(T t)
		{
			// Nothing special, just log the parameter
			std::cout << t << std::endl;
		}

		template <typename T, typename... Args>
		void LogMessage(T t, Args ... args)
		{
			// TODO: Attempt of parameter logging
			//std::string tempStr{t};
			//size_t pos = tempStr.find_first_of('{');
			//

			std::cout << t << std::endl;

			LogMessage(args...);
		}
	};
}

// Logging macros
#define LOG(...) { \
	::VoxelEngine::Log::LogMessage(__VA_ARGS__); \
	::VoxelEngine::Log::ResetOutputColor(); \
}

#define CORE_INFO(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::Log::LogLevel::Info); \
	LOG(__VA_ARGS__); \
}

#define CORE_WARN(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::Log::LogLevel::Warn); \
	LOG(__VA_ARGS__); \
}

#define CORE_ERROR(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::Log::LogLevel::Error); \
	LOG(__VA_ARGS__); \
}

// Only used in assertions! TODO: Make inaccessible global
#define CORE_CRITICAL(...) { \
	::VoxelEngine::Log::SetOutputColor(::VoxelEngine::Log::LogLevel::Critical); \
	LOG(__VA_ARGS__); \
}
