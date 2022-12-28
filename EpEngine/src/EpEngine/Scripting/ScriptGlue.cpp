#include "pch.h"
#include "ScriptGlue.h"

#include "EpEngine/Core/Input.h"
#include "EpEngine/Core/UUID.h"
#include "EpEngine/Scene/Entity.h"
#include "EpEngine/Scene/Scene.h"
#include "EpEngine/Scripting/ScriptEngine.h"

#include <mono/metadata/object.h>

namespace EpEngine
{
	#define EP_ADD_INTERNAL_CALL(func) mono_add_internal_call("EpEngine.InternalCalls::" #func, func);

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

	static void Entity_GetTranslation(UUID uuid, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID uuid, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		EP_ADD_INTERNAL_CALL(NativeLog);
		EP_ADD_INTERNAL_CALL(NativeLog_Vector);
		EP_ADD_INTERNAL_CALL(Entity_GetTranslation);
		EP_ADD_INTERNAL_CALL(Entity_SetTranslation);
		EP_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
