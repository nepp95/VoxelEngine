#pragma once

#include "EpEngine/Asset/AssetTypes.h"
#include "EpEngine/Core/UUID.h"

namespace EpEngine
{
	using AssetHandle = UUID;

	class Asset
	{
	public:
		virtual ~Asset() = default;

		void SetHandle(AssetHandle handle) { m_handle = handle; }
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