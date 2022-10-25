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

	void Profiler::SetProfileTimerCategory(const char* timer, const char* category)
	{
		m_categoryData[timer] = category;
	}

	void Profiler::Clear()
	{
		m_categoryData.clear();
		m_profileData.clear();
	}

	std::map<std::string, std::unordered_map<const char*, std::chrono::microseconds>> Profiler::GetCategorizedProfileTimerData() const
	{
		// TODO: Currently if the name of the profile is the same, but in a different category, only the last one is shown.

		// We use an ordered map with std::string, because const char* comparison is done by memory address instead of the actual string form.
		// This does require us to convert forth and back from/to const char* and std::string
		std::map<std::string, std::unordered_map<const char*, std::chrono::microseconds>> categorizedData;

		// Get categories
		std::vector<const char*> categories;
		for (auto& entry : m_categoryData)
		{
			bool isPresent{ false };
			for (auto category : categories)
			{
				if (entry.second == category)
					isPresent = true;
			}

			if (!isPresent)
				categories.push_back(entry.second);
		}

		// Loop through categories and create a map with data per category
		std::unordered_map<const char*, std::chrono::microseconds> uncategorizedData;

		for (auto& category : categories)
		{
			std::unordered_map<const char*, std::chrono::microseconds> categoryData;

			for (auto& timerData : m_profileData)
			{
				if (m_categoryData.find(timerData.first) != m_categoryData.end())
				{
					if (m_categoryData.at(timerData.first) == category)
						categoryData.insert({ timerData.first, timerData.second });
				} else
				{
					uncategorizedData.insert({ timerData.first, timerData.second });
				}
			}

			categorizedData.insert({ category, categoryData });
		}

		if (!uncategorizedData.empty())
			categorizedData.insert({ "Uncategorized", uncategorizedData });

		return categorizedData;
	}

	ProfileTimer::ProfileTimer(const char* name, Profiler* profiler, const char* category)
		: m_name(name), m_profiler(profiler)
	{
		if (category != "")
			m_profiler->SetProfileTimerCategory(name, category);
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