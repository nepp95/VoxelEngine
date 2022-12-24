#include "pch.h"
#include "ScriptGlue.h"

#include <mono/metadata/object.h>

namespace EpEngine
{
	#define VE_ADD_INTERNAL_CALL(func) mono_add_internal_call("EpEngine.InternalCalls::" #func, func);

	static void NativeLog(MonoString* string, int param)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		EP_CORE_INFO(str);
		EP_CORE_INFO(param);
	}

	static void NativeLog_Vector(glm::vec3* param, glm::vec3* outParam)
	{
		EP_CORE_INFO(*param);
		*outParam = glm::normalize(glm::vec3(*param));
	}

	void ScriptGlue::RegisterFunctions()
	{
		VE_ADD_INTERNAL_CALL(NativeLog);
		VE_ADD_INTERNAL_CALL(NativeLog_Vector);
	}
}
