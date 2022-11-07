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

	void Profiler::SetProfileTimerData(const ProfileResult& result)
	{
		m_profileData.push_back(result);
	}

	void Profiler::Clear()
	{
		for (auto& profileData : m_profileData)
			WriteProfile(profileData);

		//m_categoryData.clear();
		m_profileData.clear();
	}

	std::unordered_map<std::string, std::vector<ProfileResult>> Profiler::GetCategorizedProfileTimerData()
	{
		// Put all categories into a vector
		std::vector<std::string> categories;

		for (auto& result : m_profileData)
		{
			bool isPresent{ false };
			for (auto category : categories)
			{
				if (category == result.Category)
					isPresent = true;
			}

			if (!isPresent)
				categories.push_back(result.Category);
		}

		// Fill a vector with profile results for each category
		std::unordered_map<std::string, std::vector<ProfileResult>> categorizedData;

		for (auto category : categories)
		{
			if (category.empty())
				continue;
			
			std::vector<ProfileResult> results;

			for (auto& result : m_profileData)
			{
				if (result.Category == category)
					results.push_back(result);
			}

			categorizedData.insert({ category, results });
		}

		return categorizedData;
	}

	ProfileTimer::ProfileTimer(const char* name, Profiler* profiler, const std::string& category)
		: m_name(name), m_profiler(profiler), m_category(category)
	{
		m_startTimePoint = std::chrono::steady_clock::now();
	}

	ProfileTimer::~ProfileTimer()
	{
		using namespace std::chrono;

		auto endTimePoint = steady_clock::now();

		auto highResStart = duration<double, std::micro>{ m_startTimePoint.time_since_epoch() };
		auto elapsedTime = time_point_cast<microseconds>(endTimePoint).time_since_epoch() - time_point_cast<microseconds>(m_startTimePoint).time_since_epoch();

		ProfileResult result{ m_name, m_category, highResStart, elapsedTime, std::this_thread::get_id() };

		m_profiler->SetProfileTimerData(result);
	}
}