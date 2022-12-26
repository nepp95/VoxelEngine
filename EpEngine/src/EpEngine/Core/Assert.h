#pragma once

#include "EpEngine/Core/Base.h"
#include "EpEngine/Core/Log.h"

#include <filesystem>

#ifdef ENABLE_ASSERTS
	#define EP_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { EP##type##ERROR(msg, __VA_ARGS__); __debugbreak(); } }
	#define EP_INTERNAL_ASSERT_WITH_MSG(type, check, ...) EP_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define EP_INTERNAL_ASSERT_NO_MSG(type, check) EP_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", EP_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define EP_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define EP_INTERNAL_ASSERT_GET_MACRO(...) EP_EXPAND_MACRO(EP_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, EP_INTERNAL_ASSERT_WITH_MSG, EP_INTERNAL_ASSERT_NO_MSG))

	#define EP_ASSERT(...) EP_EXPAND_MACRO(EP_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define EP_CORE_ASSERT(...) EP_EXPAND_MACRO(EP_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define EP_ASSERT(...)
	#define EP_CORE_ASSERT(...)
#endif