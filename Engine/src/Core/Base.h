// VoxelEngine - Engine
// Base.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 26-08-2022 22:41
// Last update: 27-08-2022 11:22

#pragma once

#include <memory>

#include "Core/Log.h"
#include "Core/Assert.h"

namespace VoxelEngine
{
	// Pointers
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;
}
