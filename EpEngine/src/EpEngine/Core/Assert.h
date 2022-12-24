#pragma once

#include "EpEngine/Core/Log.h"

#define EP_CORE_ASSERT(condition, msg) { \
	if (!(condition)) \
	{ \
		EP_CORE_CRITICAL(msg); \
		__debugbreak; \
	} \
}
