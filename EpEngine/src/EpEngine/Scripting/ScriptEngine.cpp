#include "pch.h"
#include "ScriptEngine.h"

#include "EpEngine/Scripting/ScriptGlue.h"

#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>

namespace EpEngine
{
	static std::unordered_map<std::string, ScriptFieldType> s_scriptFieldTypeMap{
		{ "System.Single",		ScriptFieldType::Float },
		{ "System.Double",		ScriptFieldType::Double },
		{ "System.Boolean",		ScriptFieldType::Bool },
		{ "System.Char",		ScriptFieldType::Char },
		{ "System.Int16",		ScriptFieldType::Int16 },
		{ "System.Int32",		ScriptFieldType::Int32 },
		{ "System.Int64",		ScriptFieldType::Int64 },
		{ "System.Byte",		ScriptFieldType::Byte },
		{ "System.UInt16",		ScriptFieldType::UInt16 },
		{ "System.UInt32",		ScriptFieldType::UInt32 },
		{ "System.UInt64",		ScriptFieldType::UInt64 },

		{ "EpEngine.Vector2",	ScriptFieldType::Vector2 },
		{ "EpEngine.Vector3",	ScriptFieldType::Vector3 },
		{ "EpEngine.Vector4",	ScriptFieldType::Vector4 },

		{ "EpEngine.Entity",	ScriptFieldType::Entity },
	};

	namespace Utils
	{
		/************************************************************************/
		/* Read binary file and return a byte pointer                           */
		/************************************************************************/
		// TODO: Move to filesystem utils? When we create that...
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* size)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
			EP_CORE_ASSERT(stream, "Failed to read file!");

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t fileSize = end - stream.tellg();
			EP_CORE_ASSERT(size != 0, "File is empty!");

			char* buffer = new char[fileSize];
			stream.read((char*)buffer, fileSize);
			stream.close();

			*size = fileSize;

			return buffer;
		}

		/************************************************************************/
		/* Load C# assembly                                                     */
		/************************************************************************/
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize{ 0 };
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// Load mono image
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMsg = mono_image_strerror(status);
				// TODO: Log error?
				return nullptr;
			}

			// Load mono assembly
			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Cleanup pointers
			delete[] fileData;

			return assembly;
		}

		/************************************************************************/
		/* Print every type in our assembly to console                          */
		/************************************************************************/
		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				EP_CORE_INFO("%.%", nameSpace, name);
			}
		}

		/************************************************************************/
		/* Get class reference from assembly                                    */
		/************************************************************************/
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			MonoClass* monoClass = mono_class_from_name(image, namespaceName, className);

			if (monoClass == nullptr)
			{
				EP_CORE_ERROR("Class '{}' not found in namespace '{}'!", className, namespaceName);
				return nullptr;
			}

			return monoClass;
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string type = mono_type_get_name(monoType);

			auto it = s_scriptFieldTypeMap.find(type);
			if (it != s_scriptFieldTypeMap.end())
				return it->second;

			return ScriptFieldType::None;
		}

		std::string ScriptFieldTypeToString(ScriptFieldType type)
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

			return "<Invalid>";
		}

		std::string MonoTypeToString(MonoType* monoType)
		{
			ScriptFieldType type = MonoTypeToScriptFieldType(monoType);
			return ScriptFieldTypeToString(type);
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain{ nullptr };
		MonoDomain* AppDomain{ nullptr };

		MonoAssembly* CoreAssembly{ nullptr };
		MonoImage* CoreAssemblyImage{ nullptr };

		MonoAssembly* AppAssembly{ nullptr };
		MonoImage* AppAssemblyImage{ nullptr };

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scene* SceneContext{ nullptr };
	};

	static ScriptEngineData* s_data;

	void ScriptEngine::Init()
	{
		s_data = new ScriptEngineData();

		InitMono();
		LoadAssembly("Resources/Scripts/EpScriptCore.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();

		// Add internal calls
		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		s_data->EntityClass = ScriptClass("EpEngine", "Entity", true);

#if 0
		MonoObject* instance = s_data->EntityClass.Instantiate();

		// Call method
		MonoMethod* printMessageFunc = s_data->EntityClass.GetMethod("PrintMessage", 0);
		s_data->EntityClass.InvokeMethod(instance, printMessageFunc);

		// Call method with param
		MonoMethod* printIntFunc = s_data->EntityClass.GetMethod("PrintInt", 1);

		int value = 5;
		void* params = &value;

		s_data->EntityClass.InvokeMethod(instance, printIntFunc, &params);

		// Call method with multiple params
		MonoMethod* printIntsFunc = s_data->EntityClass.GetMethod("PrintInts", 2);

		int value1 = 7, value2 = 8;
		void* params2[2] = { &value1, &value2 };

		s_data->EntityClass.InvokeMethod(instance, printIntsFunc, params2);

		// Call method with string param
		MonoMethod* printCustomMessageFunc = s_data->EntityClass.GetMethod("PrintCustomMessage", 1);
		MonoString* str = mono_string_new(s_data->AppDomain, "test string");
		void* params3 = str;

		s_data->EntityClass.InvokeMethod(instance, printCustomMessageFunc, &params3);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// Setup app domain
		s_data->AppDomain = mono_domain_create_appdomain("EpEngineScriptRuntime", nullptr);
		mono_domain_set(s_data->AppDomain, true);

		// Setup core assembly
		s_data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		s_data->CoreAssemblyImage = mono_assembly_get_image(s_data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		s_data->AppAssemblyImage = mono_assembly_get_image(s_data->AppAssembly);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_data->SceneContext = nullptr;
		s_data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_data->EntityClasses.find(fullClassName) != s_data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			UUID uuid = entity.GetUUID();

			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_data->EntityClasses.at(sc.ClassName), entity);

			s_data->EntityInstances.insert_or_assign(uuid, instance);
			auto& test1 = s_data->EntityScriptFields;
			auto& test2 = s_data->EntityInstances;

			auto it = s_data->EntityScriptFields.find(uuid);
			if (it != s_data->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = it->second;
				for (const auto& [name, fieldInstance] : fieldMap)
					instance->SetFieldValueInternal(name, fieldInstance.m_buffer);
			}

			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float ts)
	{
		UUID entityUUID = entity.GetUUID();
		EP_CORE_ASSERT(s_data->EntityInstances.find(entityUUID) != s_data->EntityInstances.end());

		Ref<ScriptInstance> instance = s_data->EntityInstances.at(entityUUID);
		instance->InvokeOnUpdate(ts);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_data->SceneContext;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID uuid)
	{
		auto it = s_data->EntityInstances.find(uuid);
		if (it == s_data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		auto it = s_data->EntityClasses.find(name);
		if (it == s_data->EntityClasses.end())
			return nullptr;

		return it->second;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_data->EntityClasses;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(UUID uuid)
	{
		//EP_CORE_ASSERT(it != s_data->EntityScriptFields.end())

		return s_data->EntityScriptFields[uuid];
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_data->CoreAssemblyImage;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Setup root domain
		MonoDomain* rootDomain = mono_jit_init("EpJITRuntime");
		EP_CORE_ASSERT(rootDomain, "RootDomain is null!");

		s_data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		// TODO: This is not the correct way but "Mono is pain".
		s_data->AppDomain = nullptr;
		s_data->RootDomain = nullptr;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_data->CoreAssemblyImage, "EpEngine", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_data->AppAssemblyImage, nameSpace, name);


			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(monoClass);
			s_data->EntityClasses.insert_or_assign(fullName, scriptClass);

			uint32_t numFields = mono_class_num_fields(monoClass);
			EP_CORE_TRACE("{} has {} fields:", fullName, numFields);

			void* gpointer{ nullptr };
			while(MonoClassField* field = mono_class_get_fields(monoClass, &gpointer))
			{
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* fieldType = mono_field_get_type(field);
					std::string fieldName = mono_field_get_name(field);

					ScriptField scriptField;
					scriptField.ClassField = field;
					scriptField.Type = Utils::MonoTypeToScriptFieldType(fieldType);
					scriptField.Name = fieldName;

					EP_CORE_TRACE("{}::{}", Utils::ScriptFieldTypeToString(scriptField.Type), scriptField.Name);

					scriptClass->m_fields.insert_or_assign(fieldName, scriptField);
				}
			}
		}
	}

	/************************************************************************/
	/* ScriptClass                                                          */
	/************************************************************************/
	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_classNamespace(classNamespace), m_className(className)
	{
		if (isCore)
			m_monoClass = mono_class_from_name(s_data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
		else
			m_monoClass = mono_class_from_name(s_data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	ScriptClass::ScriptClass(MonoClass* monoClass)
		: m_monoClass(monoClass)
	{
		m_classNamespace = mono_class_get_namespace(monoClass);
		m_className = mono_class_get_name(monoClass);
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_monoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int paramCount)
	{
		return mono_class_get_method_from_name(m_monoClass, name.c_str(), paramCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	/************************************************************************/
	/* ScriptInstance                                                       */
	/************************************************************************/
	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_scriptClass(scriptClass)
	{
		m_instance = scriptClass->Instantiate();

		m_constructor = s_data->EntityClass.GetMethod(".ctor", 1);
		m_onCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_onUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		UUID uuid = entity.GetUUID();
		void* param = &uuid;
		m_scriptClass->InvokeMethod(m_instance, m_constructor, &param);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_onCreateMethod)
			m_scriptClass->InvokeMethod(m_instance, m_onCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_onUpdateMethod)
		{
			void* params = &ts;
			m_scriptClass->InvokeMethod(m_instance, m_onUpdateMethod, &params);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_scriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_instance, field.ClassField, s_fieldValueBuffer);

		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_scriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_instance, field.ClassField, (void*) value);
		
		return true;
	}
}
