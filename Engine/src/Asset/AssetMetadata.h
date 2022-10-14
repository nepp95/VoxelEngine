#pragma once

#include "Asset/Asset.h"

#include <filesystem>

namespace VoxelEngine
{
	struct AssetMetadata
	{
		AssetHandle Handle{ 0 };
		AssetType Type;

		std::filesystem::path FilePath;

		bool IsDataLoaded{ false };

		bool IsValid() const { return Handle != 0; }
	};
}