#pragma once

#include "Asset/Asset.h"

#include <filesystem>

namespace VoxelEngine
{
	class AssetManager
	{
	public:
		static void Init();
		static void LoadAssetsFromDirectory(const std::filesystem::path& directoryPath);

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle);

		template<typename T>
		static Ref<T> GetAsset(const std::filesystem::path& filepath);

		static AssetHandle GetAssetHandle(const std::filesystem::path& filepath);

	private:
		static std::unordered_map<AssetHandle, Ref<Asset>> m_assets;
		static std::unordered_map<std::filesystem::path, AssetHandle> m_assetHandles;
	};
}