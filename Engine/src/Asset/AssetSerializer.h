#pragma once

#include "Asset/AssetMetadata.h"

namespace YAML
{
	class Emitter;
}

namespace VoxelEngine
{
	class AssetSerializer
	{
	public:
		static void SerializeAsset(YAML::Emitter& out, const AssetMetadata& metadata);
	};
}