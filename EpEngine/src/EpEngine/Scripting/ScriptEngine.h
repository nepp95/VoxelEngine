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
	using MonoClassField = struct _MonoClassField;
}

namespace EpEngine
{
	class ScriptClass;
	class ScriptInstance;

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool,
		Char, Int16, Int32, Int64,
		Byte, UInt16, UInt32, UInt64,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float ts);

		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID uuid);
		static std::map<std::string, Ref<ScriptClass>> GetEntityClasses();

		static MonoImage* GetCoreAssemblyImage();

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);
		ScriptClass(MonoClass* monoClass);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int paramCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::unordered_map<std::string, ScriptField>& GetFields() const { return m_fields; }

	private:
		std::string m_classNamespace;
		std::string m_className;

		std::unordered_map<std::string, ScriptField> m_fields;

		MonoClass* m_monoClass{ nullptr };

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass() { return m_scriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_fieldValueBuffer);
			if (!success)
				return T();

			return *(T*) s_fieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			if (!SetFieldValueInternal(name, &value))
				EP_CORE_ERROR("Failed to set field value (Field: {}, Value: {})", name, value);
		}

	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);

	private:
		Ref<ScriptClass> m_scriptClass;
		MonoObject* m_instance{ nullptr };
		MonoMethod* m_constructor{ nullptr };
		MonoMethod* m_onCreateMethod{ nullptr };
		MonoMethod* m_onUpdateMethod{ nullptr };

		inline static char s_fieldValueBuffer[8];
	};
}
