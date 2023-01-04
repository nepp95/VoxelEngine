#include "pch.h"
#include "ScriptGlue.h"

#include "EpEngine/Core/Input.h"
#include "EpEngine/Core/UUID.h"
#include "EpEngine/Scene/Entity.h"
#include "EpEngine/Scene/Scene.h"
#include "EpEngine/Scripting/ScriptEngine.h"

#include <box2d/b2_body.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace EpEngine
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_entityHasComponentFuncs;

	#define EP_ADD_INTERNAL_CALL(func) mono_add_internal_call("EpEngine.InternalCalls::" #func, func);

	static bool Entity_HasComponent(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EP_CORE_ASSERT(s_entityHasComponentFuncs.find(managedType) != s_entityHasComponentFuncs.end());
		return s_entityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* string)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);

		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		Entity entity = scene->FindEntityByName(str);
		EP_CORE_ASSERT(entity);

		return entity.GetUUID();
	}

	static MonoObject* GetScriptInstance(UUID uuid)
	{
		return ScriptEngine::GetManagedInstance(uuid);
	}

	static void TransformComponent_GetTranslation(UUID uuid, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID uuid, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void RigidBodyComponent_ApplyLinearImpulse(UUID uuid, glm::vec2* impulse, glm::vec2* worldPosition, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		auto& rb = entity.GetComponent<RigidBodyComponent>();
		b2Body* body = (b2Body*) rb.RuntimeBody;

		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(worldPosition->x, worldPosition->y), wake);
	}

	static void RigidBodyComponent_ApplyLinearImpulseToCenter(UUID uuid, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EP_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EP_CORE_ASSERT(entity);

		auto& rb = entity.GetComponent<RigidBodyComponent>();
		b2Body* body = (b2Body*) rb.RuntimeBody;

		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

	template<typename T>
	static void RegisterComponent()
	{
		std::string_view typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');
		std::string_view structName = typeName.substr(pos + 1);
		std::string managedTypeName = fmt::format("EpEngine.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
		if (!managedType)
		{
			EP_CORE_ERROR("Could not find component type {}", managedTypeName);
			return;
		}

		auto func = [](Entity entity) { return entity.HasComponent<T>(); };
		s_entityHasComponentFuncs.insert_or_assign(managedType, func);
	}

	void ScriptGlue::RegisterComponents()
	{
		s_entityHasComponentFuncs.clear();

		RegisterComponent<TransformComponent>();
		RegisterComponent<CameraComponent>();
		RegisterComponent<SpriteComponent>();
		RegisterComponent<RigidBodyComponent>();
		RegisterComponent<BoxColliderComponent>();
		RegisterComponent<ScriptComponent>();
	}

	void ScriptGlue::RegisterFunctions()
	{
		EP_ADD_INTERNAL_CALL(Entity_HasComponent);
		EP_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		EP_ADD_INTERNAL_CALL(GetScriptInstance);
		EP_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EP_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		EP_ADD_INTERNAL_CALL(RigidBodyComponent_ApplyLinearImpulse);
		EP_ADD_INTERNAL_CALL(RigidBodyComponent_ApplyLinearImpulseToCenter);
		EP_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
