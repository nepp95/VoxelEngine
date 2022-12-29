#pragma once

#include "EpEngine/Scene/Entity.h"
#include "EpEngine/Scene/Scene.h"

// Forward declaration
namespace YAML
{
	class Emitter;
}

namespace EpEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);

	private:
		Ref<Scene> m_scene;
	};
}