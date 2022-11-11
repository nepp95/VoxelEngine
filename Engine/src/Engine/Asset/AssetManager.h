#pragma once

#include "Engine/Asset/Asset.h"

#include <filesystem>

namespace VoxelEngine
{
	class AssetManager
	{
	public:
		static void Init();
		static void LoadAssetsFromDirectory(const std::filesystem::path& directoryPath);

		template<typename T>
		static Ref<T> CreateAsset(const std::filesystem::path& filepath)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

			if (!std::filesystem::exists(filepath))
				return nullptr;
			
			Ref<T> asset = nullptr;
			
			if (LoadData(filepath, asset))
			{
				m_assets.insert_or_assign(asset->GetHandle(), asset);
				m_assetHandles.insert_or_assign(filepath, asset->GetHandle());
				return asset;
			}
			
			return nullptr;
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

			if (m_assets.find(handle) != m_assets.end())
			{
				Ref<T> asset = std::dynamic_pointer_cast<T>(m_assets.at(handle));

				if (asset)
					return asset;
			}

			return nullptr;
		}

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& filepath)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

			if (m_assetHandles.find(filepath) != m_assetHandles.end())
				return GetAsset<T>(m_assetHandles.at(filepath));

			return nullptr;
		}

		static AssetHandle GetAssetHandle(const std::filesystem::path& filepath);

	private:
		template<typename T>
		static bool LoadData(const std::filesystem::path& filepath, Ref<T>& asset);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> m_assets;
		static std::unordered_map<std::filesystem::path, AssetHandle> m_assetHandles;
	};
}