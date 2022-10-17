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

			Ref<Asset> asset{ nullptr };

			// Create metadata
			AssetMetadata metadata;
			metadata.Handle = AssetHandle();
			metadata.FilePath = filepath;
			metadata.Type = T::GetStaticType();
			metadata.IsDataLoaded = TryLoadData(metadata, asset);

			// Check if file exists
			if (!std::filesystem::exists(filepath))
				return nullptr;

			s_assetRegistry.insert_or_assign(metadata.Handle, metadata);
			
			WriteRegistry();

			asset->Handle = metadata.Handle;
			s_loadedAssets.insert_or_assign(asset->Handle, asset);

			return asset.As<T>();
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
			return GetAsset<T>(GetMetadata(filepath).Handle);
		}

		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);
		static bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset);
	
	private:
		static AssetMetadata& GetMetadataInternal(AssetHandle handle);
		static void WriteRegistry();
		static void LoadRegistry();

	private:
		// Assets
		static std::unordered_map<AssetHandle, Ref<Asset>> s_loadedAssets; // Assets which are loaded
		static std::unordered_map<AssetHandle, AssetMetadata> s_assetRegistry; // Registry with assethandles corresponding with their metadata

		static std::filesystem::path s_assetsDir;
		static std::filesystem::path s_assetRegistryFilepath;
		static std::filesystem::path s_texturesDir;

		// Serialization
		// Inherit from AssetSerializer
		//std::unordered_map<AssetType, AssetSerializer> m_serializers;
	};
}