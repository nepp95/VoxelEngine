#pragma once

#include "Engine/Asset/Asset.h"
#include "Engine/Asset/AssetMetadata.h"

#include <filesystem>

namespace VoxelEngine
{
	class AssetManager
	{
	public:
		static void Init();
		static void LoadAssetsFromDirectory(const std::filesystem::path& directoryPath);

		template<typename T, typename... Args>
		static Ref<T> CreateAsset(const std::filesystem::path& filepath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

			if (!std::filesystem::exists(filepath))
				return nullptr;

			// Create asset metadata
			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			metadata.Filepath = filepath;
			metadata.Type = T::GetStaticType();

			m_assetHandles.insert_or_assign(metadata.Handle, metadata);
			
			// Load asset
			Ref<Asset> asset = nullptr;

			if (!LoadData(metadata, asset))
				return nullptr;

			asset->SetHandle(metadata.Handle);

			m_assets.insert_or_assign(metadata.Handle, asset);
			
			return std::dynamic_pointer_cast<T>(asset);
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Class is not based on Asset!");

			Ref<Asset> asset = nullptr;

			auto& metadata = GetMetadata(handle);
			if (!metadata.IsValid())
				return nullptr;

			if (m_assets.find(handle) == m_assets.end())
			{
				if (!LoadData(metadata, asset))
					return nullptr;

				m_assets.insert_or_assign(handle, asset);
			} else
			{
				asset = m_assets.at(handle);
			}


			return std::dynamic_pointer_cast<T>(asset);
		}

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& filepath)
		{
			return GetAsset<T>(GetMetadata(filepath).Handle);
		}

		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);

	private:
		static bool LoadData(AssetMetadata metadata, Ref<Asset>& asset);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> m_assets;
		static std::unordered_map<AssetHandle, AssetMetadata> m_assetHandles;
	};
}