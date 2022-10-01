#pragma once

#include "Core/Log.h"

#define CORE_ASSERT(condition, msg) { \
	if (!(condition)) \
	{ \
		CORE_CRITICAL(msg); \
		__debugbreak; \
	} \
}
