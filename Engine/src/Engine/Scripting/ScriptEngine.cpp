#include "pch.h"
#include "ScriptEngine.h"

#include "Engine/Scripting/ScriptGlue.h"

#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace VoxelEngine
{
	namespace Utils
	{
		/************************************************************************/
		/* Read binary file and return a byte pointer                           */
		/************************************************************************/
		// TODO: Move to filesystem utils? When we create that...
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* size)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
			VE_CORE_ASSERT(stream, "Failed to read file: %", filepath);

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t fileSize = end - stream.tellg();
			VE_CORE_ASSERT(size != 0, "File '%' is empty!", filepath);

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

				VE_CORE_INFO("%.%", nameSpace, name);
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
				VE_CORE_ERROR("Class '%' not found in namespace '%'!", className, namespaceName);
				return nullptr;
			}

			return monoClass;
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain{ nullptr };
		MonoDomain* AppDomain{ nullptr };

		MonoAssembly* CoreAssembly{ nullptr };
		MonoImage* CoreAssemblyImage{ nullptr };

		ScriptClass EntityClass;
	};

	static ScriptEngineData* s_data;

	void ScriptEngine::Init()
	{
		s_data = new ScriptEngineData();

		InitMono();
		LoadAssembly("Resources/Scripts/ScriptCore.dll");

		// Add internal calls
		ScriptGlue::RegisterFunctions();

		s_data->EntityClass = ScriptClass("VoxelEngine", "Entity");
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
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// Setup app domain
		s_data->AppDomain = mono_domain_create_appdomain("VoxelEngineScriptRuntime", nullptr);
		mono_domain_set(s_data->AppDomain, true);

		// Setup core assembly
		s_data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		s_data->CoreAssemblyImage = mono_assembly_get_image(s_data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_data->CoreAssembly);
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Setup root domain
		MonoDomain* rootDomain = mono_jit_init("VoxelEngineJITRuntime");
		VE_CORE_ASSERT(rootDomain, "RootDomain is null!");

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

	/************************************************************************/
	/* ScriptClass                                                          */
	/************************************************************************/
	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_classNamespace(classNamespace), m_className(className)
	{
		m_monoClass = mono_class_from_name(s_data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
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
}
