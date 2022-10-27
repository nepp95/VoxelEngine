#pragma once

#include <chrono>
#include <fstream>
#include <map>
#include <mutex>
#include <unordered_map>

namespace VoxelEngine
{
	using floatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;
		std::string Category;
		floatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	class Profiler
	{
	public:
		Profiler() = default;
		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		~Profiler();

		void BeginSession(const std::string& filepath = "results.json");
		void EndSession();

		static Profiler& Get();

		void WriteProfile(const ProfileResult& result);
		void WriteHeader();
		void WriteFooter();

		void SetProfileTimerData(const ProfileResult& result);
		//void SetProfileTimerData(const char* name, floatingPointMicroseconds startTime, std::chrono::microseconds elapsedTime);
		//void SetProfileTimerCategory(const char* timer, const char* category);
		void Clear();

		const std::vector<ProfileResult>& GetProfileTimerData() const { return m_profileData; }
		std::unordered_map<std::string, std::vector<ProfileResult>> GetCategorizedProfileTimerData();
		//const std::unordered_map<const char*, std::pair<floatingPointMicroseconds, std::chrono::microseconds>>& GetProfileTimerData() const { return m_profileData; }
		//std::map<std::string, std::unordered_map<const char*, std::chrono::microseconds>> GetCategorizedProfileTimerData() const;

	private:
		std::vector<ProfileResult> m_profileData;
		//std::unordered_map<const char*, std::pair<std::chrono::duration<double, std::micro>, std::chrono::microseconds>> m_profileData;
		//std::unordered_map<const char*, const char*> m_categoryData;

		std::mutex m_mutex;
		std::ofstream m_outputStream;
	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name, Profiler* profiler, const std::string& category = std::string());
		~ProfileTimer();

	private:
		const char* m_name;
		std::string m_category;
		Profiler* m_profiler;

		std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
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