#pragma once

#include "EpEngine/Asset/AssetTypes.h"

#include <filesystem>

namespace EpEngine
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