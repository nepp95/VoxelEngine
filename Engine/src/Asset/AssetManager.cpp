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
				CORE_INFO("Asset registry file found");
			}

			in.close();
		}

		CORE_WARN("Asset registry file not found or can't be opened");

		// Load assets to registry from recursive file dir search
		std::stringstream ss;
		ss << "Loading textures from: " << s_texturesDir.string();
		CORE_INFO(ss.str());

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(s_texturesDir))
		{
			ss = std::stringstream();
			ss << "Checking texture: " << dirEntry.path().string();
			CORE_INFO(ss.str());

			{
				auto& metadata = GetMetadata(dirEntry.path());
				if (metadata.IsValid())
					continue;
			}

			// Create new metadata for file
			AssetMetadata metadata;
			metadata.Type = AssetType::Texture;
			metadata.FilePath = dirEntry.path();
			metadata.Handle = AssetHandle();

			s_assetRegistry.insert({ metadata.Handle, metadata });
		}

		// Load assets
		for (auto& [handle, metadata] : s_assetRegistry)
		{
			CreateAsset<Asset>(metadata.FilePath);
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
			if (metadata.FilePath == filepath)
				return metadata;
		}

		return AssetMetadata();
	}

	bool AssetManager::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		// Create asset
		asset = Ref<Texture>::Create(metadata.FilePath.string());
		asset->Handle = metadata.Handle;

		return asset.As<Texture>()->IsLoaded();
	}

	void AssetManager::WriteRegistry()
	{

	}
}