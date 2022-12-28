#pragma once

#include <filesystem>

namespace EpEngine
{
	class FileDialog
	{
	public:
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);
	};
}