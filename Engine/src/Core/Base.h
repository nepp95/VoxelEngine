#pragma once

#include <memory>

#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Ref.h"
#include "Debug/Profiler.h"

namespace VoxelEngine
{
	// Pointers
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
