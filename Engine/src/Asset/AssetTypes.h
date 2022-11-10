#pragma once

#include "Core/Base.h"

#include <string>

namespace VoxelEngine
{
	enum class AssetType
	{
		None = 0,
		Texture
	};

	namespace Utils
	{
		inline AssetType AssetTypeFromString(const std::string& assetType)
		{
			if (assetType == "None")				return AssetType::None;
			if (assetType == "Texture")				return AssetType::Texture;

			VE_CORE_ASSERT(false, "Unknown AssetType!");
			return AssetType::None;
		}

		inline std::string AssetTypeToString(AssetType assetType)
		{
			if (assetType == AssetType::None)		return "None";
			if (assetType == AssetType::Texture)	return "Texture";

			VE_CORE_ASSERT(false, "Unknown AssetType!");
			return "None";
		}
	}
}