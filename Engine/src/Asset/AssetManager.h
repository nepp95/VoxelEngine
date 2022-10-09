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
		static Ref<T> CreateAsset(const std::filesystem::path& filepath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "CreateAsset only works for types derived from Asset!");

			// Create metadata
			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			metadata.FilePath = filepath;
			metadata.IsDataLoaded = true;
			metadata.Type = T::GetStaticType();

			// Check if file exists
			if (!std::filesystem::exists(filepath))
				return nullptr;

			s_assetRegistry.insert_or_assign(metadata.Handle, metadata);
			
			WriteRegistry();

			Ref<T> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = metadata.Handle;
			s_loadedAssets.insert_or_assign(asset->Handle, asset);

			return asset;
		}

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			// Get asset metadata from registry
			AssetMetadata& metadata = GetMetadataInternal(handle);

			if (!metadata.IsValid())
				return nullptr;

			Ref<Asset> asset{ nullptr };
			if (!metadata.IsDataLoaded)
			{
				// Test if asset can be loaded
				metadata.IsDataLoaded = TryLoadData(metadata, asset);
				if (!metadata.IsDataLoaded)
					return nullptr;

				// Asset is now loaded
				s_loadedAssets.insert_or_assign(handle, asset);
			}
				
			asset = s_loadedAssets.at(handle);

			return asset.As<T>();
		}

		template<typename T>
		static Ref<T> GetAsset(const std::string& filepath)
		{
			// todo: Relative paths conversion
			
			// Find handle in registry
			for (auto& [handle, metadata] : s_assetRegistry)
				if (metadata.FilePath == filepath)
					return GetAsset<T>(metadata.Handle);

			// Return invalid metadata
			return GetAsset<T>(AssetMetadata().Handle);
		}

	private:
		static AssetMetadata& GetMetadataInternal(AssetHandle handle);
		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);

		static bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset);

		static void WriteRegistry();
		
	private:
		// Assets
		static std::unordered_map<AssetHandle, Ref<Asset>> s_loadedAssets; // Assets which are loaded
		static std::unordered_map<AssetHandle, AssetMetadata> s_assetRegistry; // Registry with assethandles corresponding with their metadata

		static std::filesystem::path s_assetsDir;
		static std::filesystem::path s_assetRegistryFilepath;
		static std::filesystem::path s_texturesDir;

		// Serialization
		//std::unordered_map<AssetType, AssetSerializer> m_serializers;
	};
}