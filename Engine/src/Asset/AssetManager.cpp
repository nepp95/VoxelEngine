#include "pch.h"
#include "AssetManager.h"

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
			VE_CORE_INFO(dirEntry.path().string());
		}
	}

	template<typename T>
	Ref<T> AssetManager::GetAsset(const std::filesystem::path& filepath)
	{
		static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");
	}

	template<typename T>
	Ref<T> AssetManager::GetAsset(AssetHandle handle)
	{
		static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

		if (m_assets.find(handle) != m_assets.end())
			return m_assets.at(handle);
	}

	AssetHandle AssetManager::GetAssetHandle(const std::filesystem::path& filepath)
	{
		if (m_assetHandles.find(filepath) != m_assetHandles.end())
			return m_assetHandles.at(filepath);

		return AssetHandle{ 0 }; // invalid handle
	}
}