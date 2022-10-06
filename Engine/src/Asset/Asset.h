#pragma once

#include "Asset/AssetTypes.h"
#include "Core/UUID.h"

namespace VoxelEngine
{
	using AssetHandle = UUID;

	class Asset : public RefCounted
	{
	public:
		AssetHandle Handle{ 0 };

		virtual ~Asset() = default;

		static AssetType GetStaticType() { return AssetType::None; }
		virtual AssetType GetAssetType() const { return AssetType::None; }

		virtual bool operator==(const Asset& other) const
		{
			return Handle == other.Handle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}
	};
}