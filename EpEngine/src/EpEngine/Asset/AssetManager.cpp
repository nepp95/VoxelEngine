#include "pch.h"
#include "AssetManager.h"

#include "EpEngine/Renderer/Texture.h"

#include <yaml-cpp/yaml.h>

namespace EpEngine
{
	std::filesystem::path AssetManager::m_assetRegistryFilepath("assets/assetregistry.vereg");
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::m_assets;
	std::unordered_map<AssetHandle, AssetMetadata> AssetManager::m_assetHandles;

	static AssetMetadata s_NullMetadata;

	void AssetManager::Init()
	{
		// We first load all assets that are in our registry file.
		// The registry file only contains AssetMetadata structures.
		LoadAssetsFromRegistry();

		// Then we do a recursive search and add any other assets that were
		// not in our asset registry file.
		LoadAssetsFromDirectory("assets/textures");

		// Finally we rewrite the registry file to add potential missing assets
		// that were found in the recursive directory search.
		WriteRegistry();

		EP_CORE_INFO("{} Assets loaded.", m_assetHandles.size());
	}

	void AssetManager::LoadAssetsFromRegistry()
	{
		if (!std::filesystem::exists(m_assetRegistryFilepath))
		{
			EP_CORE_WARN("Asset registry file does not exist!");
			return;
		}

		EP_CORE_INFO("Loading assets from registry...");

		YAML::Node data;

		try
		{
			data = YAML::LoadFile(m_assetRegistryFilepath.string());
		} catch (YAML::ParserException e)
		{
			EP_CORE_ERROR("Failed to load .vereg file '{}'!\nError: {}", m_assetRegistryFilepath.string(), e.what());
			return;
		}

		// If the root node is not "Assets", the file is most likely invalid
		if (!data["Assets"])
			return;

		auto assets = data["Assets"];

		for (auto asset : assets)
		{
			AssetMetadata metadata;

			metadata.Handle = asset["AssetHandle"].as<uint64_t>();
			metadata.Type = Utils::AssetTypeFromString(asset["AssetType"].as<std::string>());
			metadata.Filepath = asset["Filepath"].as<std::string>();

			m_assetHandles.insert_or_assign(metadata.Handle, metadata);
		}
	}

	void AssetManager::LoadAssetsFromDirectory(const std::filesystem::path& directoryPath)
	{
		EP_CORE_INFO("Loading assets from recursive directory search in '{}'...", directoryPath.string());

		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (GetMetadata(dirEntry.path()).IsValid())
				continue;

			if (dirEntry.is_directory())
				continue;

			std::string path = dirEntry.path().string();

			// TODO: This is very error prone! Come up with an other way.
			// Get asset type from path
			AssetType type = AssetType::None;

			if (path.find("Textures") != std::string::npos)
				type = AssetType::Texture;

			switch (type)
			{
				case AssetType::None:
				{
					EP_CORE_ERROR("Asset type could not be deduced from path: {}", path);
					break;
				}

				case AssetType::Texture:
				{
					CreateAsset<Texture>(dirEntry.path());
					break;
				}
			}
		}
		WriteRegistry();
	}

	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		if (m_assetHandles.find(handle) != m_assetHandles.end())
			return m_assetHandles.at(handle);

		return s_NullMetadata;
	}

	const AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& filepath)
	{
		for (auto& [handle, metadata] : m_assetHandles)
		{
			if (metadata.Filepath == filepath)
				return metadata;
		}

		return s_NullMetadata;
	}

	bool AssetManager::LoadData(AssetMetadata metadata, Ref<Asset>& asset)
	{
		switch (metadata.Type)
		{
			case AssetType::None:
			{
				return false;
				break;
			}

			case AssetType::Texture:
			{
				asset = CreateRef<Texture>(metadata.Filepath);
				return std::dynamic_pointer_cast<Texture>(asset)->IsLoaded();
			}
		}

		return false;
	}

	void AssetManager::WriteRegistry()
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value;
		out << YAML::BeginSeq;

		for (auto& asset : m_assetHandles)
		{
			AssetMetadata& metadata = asset.second;

			out << YAML::BeginMap;

			out << YAML::Key << "AssetHandle" << YAML::Value << metadata.Handle;
			out << YAML::Key << "AssetType" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
			out << YAML::Key << "Filepath" << YAML::Value << metadata.Filepath.string();
		
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(m_assetRegistryFilepath);
		fout << out.c_str();
	}
}
