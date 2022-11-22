#include "pch.h"
#include "AssetManager.h"

#include "Engine/Renderer/Texture.h"

namespace VoxelEngine
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::m_assets;
	std::unordered_map<AssetHandle, AssetMetadata> AssetManager::m_assetHandles;

	static AssetMetadata s_NullMetadata;

	void AssetManager::Init()
	{
		LoadAssetsFromDirectory("assets/textures");
	}

	void AssetManager::LoadAssetsFromDirectory(const std::filesystem::path& directoryPath)
	{
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			VE_CORE_INFO(dirEntry.path());

			// TODO: This is very error prone! Come up with an other way.
			// Get asset type from path
			std::string assetType = dirEntry.path().parent_path().string().substr(7);

			switch (Utils::AssetTypeFromString(assetType))
			{
				case AssetType::None:
				{
					break;
				}

				case AssetType::Texture:
				{
					CreateAsset<Texture>(dirEntry.path());
					break;
				}
			}
		}
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
}