#include "pch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	Emitter& operator<<(Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		out << v.x << v.y << v.z;
		out << YAML::EndSeq;

		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		out << v.x << v.y << v.z << v.w;
		out << YAML::EndSeq;

		return out;
	}
}

namespace VoxelEngine
{

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{}

	bool SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled"; // TODO: Add scene name
		out << YAML::Key << "Entities" << YAML::Value;
		out << YAML::BeginSeq;

		m_scene->m_registry.each([&] (auto entityID)
		{
			Entity entity = { entityID, m_scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;

		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			VE_CORE_ERROR("Failed to load .scene file '%'!\nError: %", filepath, e.what())
			return false;
		}

		// If the root node is not "Scene", the file is most likely invalid
		if (!data["Scene"])
			return false;

		std::string name = data["Scene"].as<std::string>();
		VE_CORE_INFO("Deserializing scene '%'", name);

		auto entities = data["Entities"];

		if (!entities)
		{
			VE_CORE_WARN("Scene has no entities, are you sure this is correct?");
			return true;
		}

		for (auto entity : entities)
		{

			return false;
		}
		// entities = data[entities]
		// loop entities

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		VE_CORE_ASSERT(entity.HasComponent<IDComponent>() && entity.HasComponent<TagComponent>(), "Trying to serialize invalid entity!");

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value;

		out << YAML::BeginMap;
		out << YAML::Key << "UUID" << YAML::Value << entity.GetUUID();
		out << YAML::Key << "Tag" << YAML::Value << entity.GetName();

		out << YAML::Key << "Components" << YAML::Value;
		out << YAML::BeginSeq;

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "TransformComponent" << YAML::Value;
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << c.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << c.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << c.Scale;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "CameraComponent" << YAML::Value;
			out << YAML::BeginMap;

			// TODO: What do we serialize?
			auto& c = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "Position" << YAML::Value << c.Camera.GetPosition();

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "SpriteComponent" << YAML::Value;
			out << YAML::BeginMap;

			// TODO: What do we serialize?
			auto& c = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "TextureHandle" << YAML::Value << c.TextureHandle;
			out << YAML::Key << "Color" << YAML::Value << c.Color;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
}