#include "pch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

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
		static MonoAssembly* LoadCSharpAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize{ 0 };
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// Load mono image
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			VE_CORE_ASSERT(status == MONO_IMAGE_OK, "Failed to load mono assembly: %\n%", assemblyPath, mono_image_strerror(status));

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
	};

	static ScriptEngineData* s_data;

	void ScriptEngine::Init()
	{
		s_data = new ScriptEngineData();

		InitMono();

		// 1. Create instance and call constructor
		MonoClass* testClass = Utils::GetClassInAssembly(s_data->CoreAssembly, "VoxelEngine", "Main");
		MonoObject* testInstance = mono_object_new(s_data->AppDomain, testClass);
		mono_runtime_object_init(testInstance);

		// 2. Call method without params
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(testClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFunc, testInstance, nullptr, nullptr);

		// 3. Call method with param
		MonoMethod* printIntFunc = mono_class_get_method_from_name(testClass, "PrintInt", 1);
		int value = 5;
		void* params = &value;

		mono_runtime_invoke(printIntFunc, testInstance, &params, nullptr);

		// 4. Call method with multiple params
		MonoMethod* printIntsFunc = mono_class_get_method_from_name(testClass, "PrintInts", 2);
		int value1 = 7, value2 = 8;
		void* params2[2] = { &value1, &value2 };

		mono_runtime_invoke(printIntsFunc, testInstance, params2, nullptr);

		// 5. Call method with string param
		MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(testClass, "PrintCustomMessage", 1);
		MonoString* str = mono_string_new(s_data->AppDomain, "test string");
		void* params3 = str;

		mono_runtime_invoke(printCustomMessageFunc, testInstance, &params3, nullptr);
	}

	void ScriptEngine::Shutdown()
	{
		delete s_data;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Setup root domain
		MonoDomain* rootDomain = mono_jit_init("VoxelEngineJITRuntime");
		VE_CORE_ASSERT(rootDomain, "RootDomain is null!");
		s_data->RootDomain = rootDomain;
	
		// Setup app domain
		s_data->AppDomain = mono_domain_create_appdomain("VoxelEngineScriptRuntime", nullptr);
		mono_domain_set(s_data->AppDomain, true);

		// Setup core assembly
		s_data->CoreAssembly = Utils::LoadCSharpAssembly("Resources/Scripts/ScriptCore.dll");
		Utils::PrintAssemblyTypes(s_data->CoreAssembly);
	}

	void ScriptEngine::ShutdownMono()
	{
	}
}
