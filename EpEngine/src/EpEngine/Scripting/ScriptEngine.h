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

	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_buffer, 0, sizeof(m_buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_buffer, &value, sizeof(T));
		}

	private:
		uint8_t m_buffer[16];

		friend class ScriptInstance;
		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

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
		static Ref<ScriptClass> GetEntityClass(const std::string& name);		
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(UUID uuid);

		static MonoImage* GetCoreAssemblyImage();
		static MonoObject* GetManagedInstance(UUID uuid);

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
		MonoObject* GetManagedObject() { return m_instance; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			if (!GetFieldValueInternal(name, s_fieldValueBuffer))
				return T();

			return *(T*) s_fieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			SetFieldValueInternal(name, &value);
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

		inline static char s_fieldValueBuffer[16];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	};

	namespace Utils
	{
		inline std::string ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
				case ScriptFieldType::Float:	return "Float";
				case ScriptFieldType::Double:	return "Double";
				case ScriptFieldType::Bool:		return "Bool";
				case ScriptFieldType::Char:		return "Char";
				case ScriptFieldType::Int16:	return "Int16";
				case ScriptFieldType::Int32:	return "Int32";
				case ScriptFieldType::Int64:	return "Int64";
				case ScriptFieldType::Byte:		return "Byte";
				case ScriptFieldType::UInt16:	return "UInt16";
				case ScriptFieldType::UInt32:	return "UInt32";
				case ScriptFieldType::UInt64:	return "UInt64";

				case ScriptFieldType::Vector2:	return "Vector2";
				case ScriptFieldType::Vector3:	return "Vector3";
				case ScriptFieldType::Vector4:	return "Vector4";

				case ScriptFieldType::Entity:	return "Entity";
			}

			EP_CORE_ASSERT(false, "Unknown ScriptFieldType!");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view type)
		{
			if (type == "Float")		return ScriptFieldType::Float;
			if (type == "Double")		return ScriptFieldType::Double;
			if (type == "Bool")			return ScriptFieldType::Bool;
			if (type == "Char")			return ScriptFieldType::Char;
			if (type == "Int16")		return ScriptFieldType::Int16;
			if (type == "Int32")		return ScriptFieldType::Int32;
			if (type == "Int64")		return ScriptFieldType::Int64;
			if (type == "Byte")			return ScriptFieldType::Byte;
			if (type == "UInt16")		return ScriptFieldType::UInt16;
			if (type == "UInt32")		return ScriptFieldType::UInt32;
			if (type == "UInt64")		return ScriptFieldType::UInt64;
			if (type == "Vector2")		return ScriptFieldType::Vector2;
			if (type == "Vector3")		return ScriptFieldType::Vector3;
			if (type == "Vector4")		return ScriptFieldType::Vector4;
			if (type == "Entity")		return ScriptFieldType::Entity;

			EP_CORE_ASSERT(false, "Unknown ScriptFieldType!");
			return ScriptFieldType::None;
		}
	}
}
