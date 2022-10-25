#include "pch.h"
#include "Profiler.h"

namespace VoxelEngine
{
	Profiler::~Profiler()
	{
		EndSession();
	}

	void Profiler::BeginSession(const std::string& filepath)
	{
		std::lock_guard lock(m_mutex);

		m_outputStream.open(filepath);

		if (m_outputStream.is_open())
		{
			WriteHeader();
		}
	}

	void Profiler::EndSession()
	{
		std::lock_guard lock(m_mutex);

		WriteFooter();
		m_outputStream.close();
	}

	Profiler& Profiler::Get()
	{
		static Profiler instance;
		return instance;
	}

	void Profiler::WriteProfile(const ProfileResult& result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::lock_guard lock(m_mutex);

		m_outputStream << json.str();
		m_outputStream.flush();
	}

	void Profiler::WriteHeader()
	{
		m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		m_outputStream.flush();
	}

	void Profiler::WriteFooter()
	{
		m_outputStream << "]}";
		m_outputStream.flush();
	}

	void Profiler::SetProfileTimerData(const char* name, floatingPointMicroseconds startTime, std::chrono::microseconds elapsedTime)
	{
		if (m_profileData.find(name) != m_profileData.end())
		{
			auto category = m_categoryData.at(name);
			Clear();
			SetProfileTimerCategory(name, category);
		}

		m_profileData[name].first = startTime;
		m_profileData[name].second = elapsedTime;
	}

	void Profiler::SetProfileTimerCategory(const char* timer, const char* category)
	{
		m_categoryData[timer] = category;
	}

	void Profiler::Clear()
	{
		for (auto& profileData : m_profileData)
			WriteProfile({ profileData.first, profileData.second.first, profileData.second.second, std::this_thread::get_id() });

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
			if (category != "")
			{
				std::unordered_map<const char*, std::chrono::microseconds> categoryData;

				for (auto& timerData : m_profileData)
				{
					if (m_categoryData.find(timerData.first) != m_categoryData.end())
					{
						if (m_categoryData.at(timerData.first) == category)
							categoryData.insert({ timerData.first, timerData.second.second });
					}
					else
					{
						uncategorizedData.insert({ timerData.first, timerData.second.second });
					}
				}

				categorizedData.insert({ category, categoryData });
			}
		}

		if (!uncategorizedData.empty())
			categorizedData.insert({ "Uncategorized", uncategorizedData });

		return categorizedData;
	}

	ProfileTimer::ProfileTimer(const char* name, Profiler* profiler, const char* category)
		: m_name(name), m_profiler(profiler)
	{
		m_profiler->SetProfileTimerCategory(name, category);
		m_startTimePoint = std::chrono::steady_clock::now();
	}

	ProfileTimer::~ProfileTimer()
	{
		using namespace std::chrono;

		auto endTimePoint = steady_clock::now();

		auto highResStart = duration<double, std::micro>{ m_startTimePoint.time_since_epoch() };
		auto elapsedTime = time_point_cast<microseconds>(endTimePoint).time_since_epoch() - time_point_cast<microseconds>(m_startTimePoint).time_since_epoch();

		m_profiler->SetProfileTimerData(m_name, highResStart, elapsedTime);
	}
}