#include "pch.h"
#include "Log.h"

namespace VoxelEngine
{
	void Log::Init()
	{

	}

	void Log::Shutdown()
	{

	}

	void Log::SetOutputColor(const LogLevel& level)
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

	void Log::ResetOutputColor()
	{
		std::cout << "\33[0m";
	}

	void Log::EndMessage()
	{
		std::cout << std::endl;
	}
}