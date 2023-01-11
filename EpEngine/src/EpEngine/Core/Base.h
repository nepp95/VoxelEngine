﻿#pragma once

#include <memory>

namespace EpEngine
{
	// Pointers
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#if defined(EP_DEBUG)
	#define ENABLE_ASSERTS
	#define ENABLE_PROFILING
	#define WRITE_PROFILES
#endif

#if defined(EP_RELEASE)
	#define ENABLE_PROFILING
#endif

#define EP_EXPAND_MACRO(x) x
#define EP_STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "EpEngine/Core/Log.h"
#include "EpEngine/Core/Assert.h"
#include "EpEngine/Debug/Profiler.h"