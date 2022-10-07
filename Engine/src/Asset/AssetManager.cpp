#include "pch.h"
#include "AssetManager.h"

namespace VoxelEngine
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::m_loadedAssets;
	std::unordered_map<AssetHandle, AssetMetadata> AssetManager::m_assetRegistry;

	void AssetManager::Init()
	{
		// Load all files in assets dir into registry
		// todo: ONLY IF NOT IN REGISTRY FILE
		std::filesystem::path assetsDir("assets");
		std::filesystem::path texturesDir(assetsDir / "textures");

		std::stringstream ss;
		ss << "Loading textures from: " << texturesDir.string();
		CORE_INFO(ss.str());

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(texturesDir))
		{
			ss = std::stringstream();
			ss << "Loading texture to registry: " << dirEntry.path().string();
			CORE_INFO(ss.str());


		}
	}

	void AssetManager::Shutdown()
	{
		// Write registry to file

		// Clear assets
		m_loadedAssets.clear();
		m_assetRegistry.clear();
	}

	bool AssetManager::TryLoadData(const std::string& filepath, Ref<Asset>& asset)
	{
		return false;
	}
}