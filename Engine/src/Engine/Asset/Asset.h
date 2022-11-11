#pragma once

#include "Engine/Asset/AssetTypes.h"
#include "Engine/Core/UUID.h"

namespace VoxelEngine
{
	using AssetHandle = UUID;

	class Asset
	{
	public:
		virtual ~Asset() = default;

		const AssetHandle& GetHandle() const { return m_handle; }
		static AssetType GetStaticType() { return AssetType::None; }

		virtual bool operator==(const Asset& other) const
		{
			return m_handle == other.m_handle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}

	private:
		AssetHandle m_handle;
	};
}