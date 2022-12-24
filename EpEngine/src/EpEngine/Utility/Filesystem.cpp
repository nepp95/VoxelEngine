#include "pch.h"
#include "Filesystem.h"

namespace EpEngine
{
	struct FilesystemData
	{
		std::filesystem::path RootPath;
		std::filesystem::path AssetPath;
	};

	FilesystemData* s_data;

	void Filesystem::Init()
	{
		s_data = new FilesystemData();

		s_data->RootPath = std::filesystem::current_path();
		s_data->AssetPath = s_data->RootPath / "assets";
	}

	void Filesystem::Shutdown()
	{
		delete s_data;
	}

	char* Filesystem::ReadBytes(const std::filesystem::path& filepath)
	{
		// TODO: Fix end line char
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		EP_CORE_ASSERT(stream, "Failed to read file: %", filepath);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t fileSize = end - stream.tellg();
		EP_CORE_ASSERT(fileSize != 0, "File '%' is empty!", filepath);

		char* buffer = new char[fileSize];
		stream.read((char*)buffer, fileSize);
		stream.close();

		return buffer;
	}

	void Filesystem::WriteBytes(const std::filesystem::path& filepath, const char* bytes, bool overwrite /*= true*/)
	{

	}
}