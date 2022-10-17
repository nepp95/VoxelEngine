#include "pch.h"
#include "AssetManager.h"

#include "Asset/AssetSerializer.h"
#include "Renderer/Texture.h"

#include <yaml-cpp/yaml.h>

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
			VE_CORE_INFO("Asset registry file found at: %", s_assetRegistryFilepath);
			LoadRegistry();
		} else
		{
			VE_CORE_WARN("Asset registry file not found or can't be opened");
		}

		// Load assets to registry from recursive file dir search
		VE_CORE_INFO("Loading textures from: %", s_texturesDir.string());

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(s_texturesDir))
		{
			VE_CORE_INFO("Checking texture: %", dirEntry.path().string());
			AssetMetadata metadata = GetMetadata(dirEntry.path());
			if (!metadata.IsValid())
				CreateAsset<Texture>(dirEntry.path());
			else
				GetAsset<Texture>(metadata.Handle);

		}

		WriteRegistry();
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
		std::ofstream outFile(s_assetRegistryFilepath, std::ios::binary | std::ios::out);

		if (outFile.is_open())
		{
			YAML::Emitter out;
			out << YAML::BeginMap;
			out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

			for (auto& asset : s_assetRegistry)
				AssetSerializer::SerializeAsset(out, asset.second);

			out << YAML::EndSeq;
			out << YAML::EndMap;

			outFile << out.c_str();
			outFile.close();
		} else
		{
			VE_CORE_ERROR("Something went wrong writing the asset registry!");
		}
	}

	void AssetManager::LoadRegistry()
	{
		std::ifstream inFile(s_assetRegistryFilepath);

		if (inFile.is_open())
		{
			std::stringstream ss;
			ss << inFile.rdbuf();

			YAML::Node data = YAML::Load(ss.str());
			VE_CORE_ASSERT(data["Assets"], "Invalid assets registry file!");

			VE_CORE_INFO("Deserializing asset registry file.");

			YAML::Node assets = data["Assets"];
			if (assets)
			{
				// Loop through asset entries from asset registry
				for (auto asset : assets)
				{
					// Create metadata from asset registry entry
					AssetMetadata metadata;
					metadata.Handle = asset["Handle"].as<uint64_t>();
					metadata.FilePath = asset["Filepath"].as<std::string>();
					metadata.Type = Utils::AssetTypeFromString(asset["Type"].as<std::string>());

					// Invalid asset
					if (metadata.Type == AssetType::None)
						continue;

					if (!std::filesystem::exists(metadata.FilePath))
					{
						VE_CORE_WARN("Missing asset '%' found in registry file.", metadata.FilePath);
						continue;
					}

					if (!metadata.IsValid())
					{
						VE_CORE_WARN("Trying to load an invalid asset: %", metadata.FilePath);
						continue;
					}

					s_assetRegistry.insert_or_assign(metadata.Handle, metadata);
				}
			}

			VE_CORE_INFO("Loaded % assets from asset registry file.", s_assetRegistry.size());
		} else
		{
			VE_CORE_WARN("Asset registry file not found or can't be opened");
		}
	}
}