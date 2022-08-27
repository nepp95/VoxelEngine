// VoxelEngine - Engine
// Assert.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 27-08-2022 11:18
// Last update: 27-08-2022 11:26

#pragma once

#include "Core/Log.h"

#define CORE_ASSERT(condition, msg) { \
	if (!(condition)) \
	{ \
		CORE_CRITICAL(msg); \
		__debugbreak; \
	} \
}
