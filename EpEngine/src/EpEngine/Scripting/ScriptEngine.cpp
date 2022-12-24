#include "pch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace EpEngine
{
	namespace Utils
	{
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* size)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
			EP_CORE_ASSERT(stream, "Failed to read file: %", filepath);

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t fileSize = end - stream.tellg();
			EP_CORE_ASSERT(size != 0, "File '%' is empty!", filepath);

			char* buffer = new char[fileSize];
			stream.read((char*)buffer, fileSize);
			stream.close();

			*size = fileSize;

			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize{ 0 };
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// Load mono image
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			EP_CORE_ASSERT(status == MONO_IMAGE_OK, "Failed to load mono assembly: %\n%", assemblyPath, mono_image_strerror(status));

			// Load mono assembly
			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Cleanup pointers
			delete[] fileData;

			return assembly;
		}

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
	}

	void ScriptEngine::Shutdown()
	{
		delete s_data;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Setup root domain
		MonoDomain* rootDomain = mono_jit_init("EpJITRuntime");
		EP_CORE_ASSERT(rootDomain, "RootDomain is null!");

		s_data->RootDomain = rootDomain;
	
		// Setup app domain
		s_data->AppDomain = mono_domain_create_appdomain("EpEngineScriptRuntime", nullptr);
		mono_domain_set(s_data->AppDomain, true);

		// Setup core assembly
		s_data->CoreAssembly = Utils::LoadCSharpAssembly("Resources/Scripts/ScriptCore.dll");
		Utils::PrintAssemblyTypes(s_data->CoreAssembly);
	}

	void ScriptEngine::ShutdownMono()
	{
	}
}
