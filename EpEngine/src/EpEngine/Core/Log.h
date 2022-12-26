#pragma once

#include "EpEngine/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace EpEngine
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

	private:
		static Ref<spdlog::logger> s_coreLogger;
		static Ref<spdlog::logger> s_clientLogger;
	};
}

// Log vectors
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

// Log matrices
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

#define EP_CORE_TRACE(...)		::EpEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EP_CORE_INFO(...)		::EpEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EP_CORE_WARN(...)		::EpEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EP_CORE_ERROR(...)		::EpEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EP_CORE_CRITICAL(...)	::EpEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define EP_TRACE(...)			::EpEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EP_INFO(...)			::EpEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define EP_WARN(...)			::EpEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EP_ERROR(...)			::EpEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define EP_CRITICAL(...)		::EpEngine::Log::GetClientLogger()->critical(__VA_ARGS__)