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
		static void Init();
		static void Shutdown();

		template<typename T, typename... Args>
		static Ref<T> CreateAsset(const std::string& filepath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "CreateAsset only works for types derived from Asset!");

			// Create metadata
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
			// Get asset metadata from registry
			AssetMetadata& metadata = AssetMetadata();
			if (m_assetRegistry.find(assetHandle) != m_assetRegistry.end())
				metadata = m_assetRegistry.at(assetHandle); // asset metadata

			Ref<T> asset{ nullptr };
			if (!metadata.IsDataLoaded)
			{
				// Create asset
				asset = Ref<Texture>::Create(metadata.FilePath.string());
				asset->Handle = metadata.Handle;

				// Test is asset can be loaded
				metadata.IsDataLoaded = asset.As<Texture>()->IsLoaded();
				if (!metadata.IsDataLoaded)
					return nullptr;

				// Asset is now loaded
				m_loadedAssets[assetHandle] = asset;
			}
				
			asset = m_loadedAssets[assetHandle];

			return asset.As<T>();
		}

		template<typename T>
		static Ref<T> GetAsset(const std::string& filepath)
		{
			// todo: Relative paths conversion
			
			// Find handle in registry
			for (auto& [handle, metadata] : m_assetRegistry)
				if (metadata.FilePath == filepath)
					return GetAsset<T>(metadata.Handle);

			// Return invalid metadata
			return GetAsset<T>(AssetMetadata().Handle);
		}

	private:
		bool TryLoadData(const std::string& filepath, Ref<Asset>& asset);
		
	private:
		// Assets
		static std::unordered_map<AssetHandle, Ref<Asset>> m_loadedAssets; // Assets which are loaded
		static std::unordered_map<AssetHandle, AssetMetadata> m_assetRegistry; // Registry with assethandles corresponding with their metadata

		// Serialization
		//std::unordered_map<AssetType, AssetSerializer> m_serializers;
	};
}