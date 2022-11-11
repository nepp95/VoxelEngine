#include "pch.h"
#include "AssetManager.h"

#include "Engine/Renderer/Texture.h"

namespace VoxelEngine
{
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::m_assets;
	std::unordered_map<std::filesystem::path, AssetHandle> AssetManager::m_assetHandles;

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

	AssetHandle AssetManager::GetAssetHandle(const std::filesystem::path& filepath)
	{
		if (m_assetHandles.find(filepath) != m_assetHandles.end())
			return m_assetHandles.at(filepath);

		return AssetHandle{ 0 }; // invalid handle
	}

	template<typename T>
	bool AssetManager::LoadData(const std::filesystem::path& filepath, Ref<T>& asset)
	{
		static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

		switch (T::GetStaticType())
		{
			case AssetType::None:
			{
				return false;
				break;
			}

			case AssetType::Texture:
			{
				asset = CreateRef<Texture>(filepath);
				return asset->IsLoaded();
			}
		}

		return false;
	}
}