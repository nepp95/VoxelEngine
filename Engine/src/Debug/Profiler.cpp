#include "pch.h"
#include "Profiler.h"

namespace VoxelEngine
{
	void Profiler::SetProfileTimerData(const char* name, std::chrono::microseconds time)
	{
		if (m_profileData.find(name) == m_profileData.end())
			m_profileData[name] = std::chrono::microseconds(0);

		m_profileData[name] += time;
	}

	void Profiler::Clear()
	{
		m_profileData.clear();
	}

	ProfileTimer::ProfileTimer(const char* name, Profiler* profiler)
		: m_name(name), m_profiler(profiler)
	{
		m_startTimePoint = std::chrono::high_resolution_clock::now();
	}

	ProfileTimer::~ProfileTimer()
	{
		using namespace std::chrono;

		auto endTimePoint = high_resolution_clock::now();

		auto highResStart = duration<double, std::micro>{ m_startTimePoint.time_since_epoch() };
		auto elapsedTime = time_point_cast<microseconds>(endTimePoint).time_since_epoch() - time_point_cast<microseconds>(m_startTimePoint).time_since_epoch();

		m_profiler->SetProfileTimerData(m_name, elapsedTime);
	}
}