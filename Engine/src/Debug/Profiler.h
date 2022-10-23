#pragma once

#include <chrono>
#include <unordered_map>

namespace VoxelEngine
{
	class Profiler
	{
	public:
		void SetProfileTimerData(const char* name, std::chrono::microseconds time);
		void Clear();

		const std::unordered_map<const char*, std::chrono::microseconds>& GetProfileTimerData() const { return m_profileData; }

	private:
		std::unordered_map<const char*, std::chrono::microseconds> m_profileData;
	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name, Profiler* profiler);
		~ProfileTimer();

	private:
		const char* m_name;
		Profiler* m_profiler;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	};
}

#define ENABLE_PROFILING
#ifdef ENABLE_PROFILING
	#define VE_PROFILE_SCOPE(name) ProfileTimer timer__LINE__(name, Application::Get().GetProfiler());
	#define VE_PROFILE_FUNCTION() VE_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define VE_PROFILE_SCOPE(name)
	#define VE_PROFILE_FUNCTION()
#endif