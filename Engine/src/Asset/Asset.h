#pragma once

#include "Asset/AssetTypes.h"
#include "Core/UUID.h"

namespace VoxelEngine
{
	using AssetHandle = UUID;

	class Asset
	{
	public:
		Asset() = default;
		virtual ~Asset() = default;

		const AssetHandle& GetHandle() const { return m_handle; }

		template<typename T>
		Ref<T> As() const
		{
			return Ref<T>(*this);
		}

	private:
		AssetHandle m_handle;
	};
}