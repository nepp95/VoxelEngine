#pragma once

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h";

// Forward declaration
namespace YAML
{
	class Emitter;
}

namespace VoxelEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::filesystem::path& filepath);
		void Deserialize(Ref<Scene>& scene);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);

	private:
		Ref<Scene> m_scene;
	};
}