#include "pch.h"
#include "AssetManager.h"

#include "Renderer/Texture.h"

namespace VoxelEngine
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_loadedAssets;
	std::unordered_map<AssetHandle, AssetMetadata> AssetManager::s_assetRegistry;

	std::filesystem::path AssetManager::s_assetsDir;
	std::filesystem::path AssetManager::s_assetRegistryFilepath;
	std::filesystem::path AssetManager::s_texturesDir;

	void AssetManager::Init()
	{
		// Load all files in assets dir into registry
		// todo: ONLY IF NOT IN REGISTRY FILE
		s_assetsDir = "assets";
		s_texturesDir = s_assetsDir / "textures";
		s_assetRegistryFilepath = s_assetsDir / "registry.yaml";

		// Load assets from registry file
		if (std::filesystem::exists(s_assetRegistryFilepath))
		{
			std::ifstream in(s_assetRegistryFilepath.string());
		
			if (in.is_open())
			{
				VE_CORE_INFO("Asset registry file found at: %", s_assetRegistryFilepath);
			}

			in.close();
		}

		VE_CORE_WARN("Asset registry file not found or can't be opened");

		// Load assets to registry from recursive file dir search
		VE_CORE_INFO("Loading textures from: %", s_texturesDir.string());

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(s_texturesDir))
		{
			VE_CORE_INFO("Checking texture: %", dirEntry.path().string());

			Ref<Asset> asset = CreateAsset<Texture>(dirEntry.path());

			// Create new metadata for file
			//AssetMetadata metadata;
			//metadata.Type = AssetType::Texture;
			//metadata.FilePath = dirEntry.path();
			//metadata.Handle = AssetHandle();
		}
	}

	void AssetManager::Shutdown()
	{
		// Write registry to file
		WriteRegistry();

		// Clear assets
		s_loadedAssets.clear();
		s_assetRegistry.clear();
	}

	AssetMetadata& AssetManager::GetMetadataInternal(AssetHandle handle)
	{
		if (s_assetRegistry.find(handle) != s_assetRegistry.end())
			return s_assetRegistry.at(handle);

		return AssetMetadata();
	}

	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		if (s_assetRegistry.find(handle) != s_assetRegistry.end())
			return s_assetRegistry.at(handle);

		return AssetMetadata();
	}

	const AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& filepath)
	{
		for (auto& [handle, metadata] : s_assetRegistry)
		{
			VE_CORE_INFO(filepath.parent_path());
			if (metadata.FilePath == filepath)
				return metadata;
		}

		return AssetMetadata();
	}

	bool AssetManager::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		// Create asset
		switch (metadata.Type)
		{
			case AssetType::None:
			{
				return false;
			}

			case AssetType::Texture:
			{
				asset = Ref<Texture>::Create(metadata.FilePath.string());
				asset->Handle = metadata.Handle;

				return asset.As<Texture>()->IsLoaded();
			}
		}
		
		return false;
	}

	void AssetManager::WriteRegistry()
	{

	}
}