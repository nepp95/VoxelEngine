#pragma once

#include "Engine/Asset/AssetTypes.h"

#include <filesystem>

namespace VoxelEngine
{
	struct AssetMetadata
	{
		AssetHandle Handle{ 0 };
		AssetType Type;
		std::filesystem::path Filepath;

		bool IsValid() const
		{
			return Handle && std::filesystem::exists(Filepath);
		}
	};
}