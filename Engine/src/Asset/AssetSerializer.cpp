#include "pch.h"
#include "AssetSerializer.h"

#include <yaml-cpp/yaml.h>

namespace VoxelEngine
{
	void AssetSerializer::SerializeAsset(YAML::Emitter& out, const AssetMetadata& metadata)
	{
		VE_CORE_ASSERT(metadata.IsValid(), "Trying to serialize invalid asset!");

		out << YAML::BeginMap;

		out << YAML::Key << "Handle" << YAML::Value << metadata.Handle;
		out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
		out << YAML::Key << "Filepath" << YAML::Value << metadata.FilePath.string();
		out << YAML::Key << "IsDataLoaded" << YAML::Value << metadata.IsDataLoaded;

		out << YAML::EndMap;
	}
}