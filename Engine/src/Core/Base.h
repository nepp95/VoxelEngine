// VoxelEngine - Engine
// Base.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 26-08-2022 22:41
// Last update: 31-08-2022 21:35

#pragma once

#include <memory>

#include "Core/Log.h"
#include "Core/Assert.h"

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

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
