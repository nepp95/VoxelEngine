#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <unordered_map>

namespace EpEngine
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
		void Clear();

		const std::vector<ProfileResult>& GetProfileTimerData() const { return m_profileData; }
		std::unordered_map<std::string, std::vector<ProfileResult>> GetCategorizedProfileTimerData();

	private:
		std::vector<ProfileResult> m_profileData;

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

#ifdef ENABLE_PROFILING
	#define EP_PROFILE_SCOPE(name) ProfileTimer timer__LINE__(name, Application::Get().GetProfiler());
	#define EP_PROFILE_SCOPE_CAT(category, name) ProfileTimer timer__LINE__(name, Application::Get().GetProfiler(), category);
	#define EP_PROFILE_FUNCTION() EP_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define EP_PROFILE_SCOPE(name)
	#define EP_PROFILE_SCOPE_CAT(category, name)
	#define EP_PROFILE_FUNCTION()
#endif