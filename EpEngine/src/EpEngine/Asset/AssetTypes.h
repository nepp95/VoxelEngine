#pragma once

#include "EpEngine/Core/Base.h"

#include <string>

namespace EpEngine
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
			// AssetType string
			if (assetType == "None")				return AssetType::None;
			if (assetType == "Texture")				return AssetType::Texture;

			// Asset folder string
			if (assetType == "textures")			return AssetType::Texture;

			EP_CORE_ASSERT(false, "Unknown AssetType!");
			return AssetType::None;
		}

		inline std::string AssetTypeToString(AssetType assetType)
		{
			if (assetType == AssetType::None)		return "None";
			if (assetType == AssetType::Texture)	return "Texture";

			EP_CORE_ASSERT(false, "Unknown AssetType!");
			return "None";
		}
	}
}