#pragma once

#include "EpEngine/Scene/Entity.h"
#include "EpEngine/Scene/Scene.h"

#include <filesystem>
#include <string>
#include <map>

extern "C" {
	using MonoClass = struct _MonoClass;
	using MonoObject = struct _MonoObject;
	using MonoMethod = struct _MonoMethod;
	using MonoAssembly = struct _MonoAssembly;
	using MonoImage = struct _MonoImage;
}

namespace EpEngine
{
	class ScriptClass;
	class ScriptInstance;

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float ts);

		static Scene* GetSceneContext();
		static std::map<std::string, Ref<ScriptClass>> GetEntityClasses();

		static MonoImage* GetCoreAssemblyImage();

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int paramCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_classNamespace;
		std::string m_className;

		MonoClass* m_monoClass{ nullptr };
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

	private:
		Ref<ScriptClass> m_scriptClass;
		MonoObject* m_instance{ nullptr };
		MonoMethod* m_constructor{ nullptr };
		MonoMethod* m_onCreateMethod{ nullptr };
		MonoMethod* m_onUpdateMethod{ nullptr };
	};
}
