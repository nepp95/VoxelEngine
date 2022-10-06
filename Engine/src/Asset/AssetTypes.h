#pragma once

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
			if (assetType == "None")			return AssetType::None;
			if (assetType == "Texture")			return AssetType::Texture;

			CORE_ASSERT(false, "Unknown AssetType!");
			return AssetType::None;
		}

		inline std::string AssetTypeToString(AssetType assetType)
		{
			switch (assetType)
			{
				case AssetType::None:			return "None";
				case AssetType::Texture:		return "Texture";
			}

			CORE_ASSERT(false, "Unknown AssetType!");
			return "None";
		}
	}
}