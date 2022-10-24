#pragma once

#include <chrono>
#include <map>
#include <unordered_map>

namespace VoxelEngine
{
	class Profiler
	{
	public:
		void SetProfileTimerData(const char* name, std::chrono::microseconds time);
		void SetProfileTimerCategory(const char* timer, const char* category);
		void Clear();

		const std::unordered_map<const char*, std::chrono::microseconds>& GetProfileTimerData() const { return m_profileData; }
		std::map<std::string, std::unordered_map<const char*, std::chrono::microseconds>> GetCategorizedProfileTimerData() const;

	private:
		std::unordered_map<const char*, std::chrono::microseconds> m_profileData;
		std::unordered_map<const char*, const char*> m_categoryData;
	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name, Profiler* profiler, const char* category = "");
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
	#define VE_PROFILE_SCOPE_CAT(category, name) ProfileTimer timer__LINE__(name, Application::Get().GetProfiler(), category);
	#define VE_PROFILE_FUNCTION() VE_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define VE_PROFILE_SCOPE(name)
	#define VE_PROFILE_SCOPE_CAT(category, name)
	#define VE_PROFILE_FUNCTION()
#endif