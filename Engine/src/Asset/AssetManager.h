#pragma once

#include "Asset/Asset.h"
#include "Asset/AssetTypes.h"
#include "Asset/AssetMetadata.h"

#include <unordered_map>

namespace VoxelEngine
{
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		template<typename T, typename... Args>
		static Ref<T> CreateAsset(const std::string& filepath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "CreateAsset only works for types derived from Asset!");

			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			metadata.FilePath = filepath;
			metadata.IsDataLoaded = true;
			metadata.Type = T::GetStaticType();

			// Check if file exists
			

			m_assetRegistry[metadata.Handle] = metadata;
			// todo: save registry to file, load on startup

			Ref<T> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = metadata.Handle;
			m_loadedAssets[asset->Handle] = asset;

			return asset;
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle assetHandle)
		{
			if (m_memoryAssets.find(assetHandle) != m_memoryAssets.end())
				return m_memoryAssets[assetHandle].As<T>();

			AssetMetadata& metadata;
			if (m_assetRegistry.find(assetHandle) != m_assetRegistry.end())
				metadata = m_assetRegistry.find(assetHandle);

			Ref<T> asset{ nullptr };
			if (!metadata.IsDataLoaded)
			{
				// Create asset
				//asset = //
				asset->Handle = metadata.Handle;

				// Test is asset can be loaded
				// metadata.IsDataLoaded = asset.As<???>()->Loaded();
				//if (!metadata.IsDataLoaded);
				//	return nullptr;

				// Asset is now loaded
				m_loadedAssets[assetHandle] = asset;
			}
				
			asset = m_loadedAssets[assetHandle];

			return asset.As<T>();
		}
		
	private:
		std::unordered_map<AssetHandle, Ref<Asset>> m_loadedAssets;
		std::unordered_map<AssetHandle, Ref<Asset>> m_memoryAssets; // Assets which have been loaded
		std::unordered_map<AssetHandle, AssetMetadata> m_assetRegistry; // Registry with assethandles corresponding with their metadata
	};
}