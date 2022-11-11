#pragma once

#include "Engine/Core/Log.h"

#define VE_CORE_ASSERT(condition, msg) { \
	if (!(condition)) \
	{ \
		VE_CORE_CRITICAL(msg); \
		__debugbreak; \
	} \
}
