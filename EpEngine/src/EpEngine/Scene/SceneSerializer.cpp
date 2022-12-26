#include "pch.h"
#include "SceneSerializer.h"

#include "EpEngine/Utility/Converter.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
		
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);

			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

	// Operator overloading
	Emitter& operator<<(Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;

		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}
}

namespace EpEngine
{

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{}

	bool SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		EP_CORE_INFO("Serializing scene '{}'", "Untitled");

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
		} catch (YAML::ParserException e)
		{
			EP_CORE_ERROR("Failed to load .scene file '{}'!\nError: {}", filepath, e.what());
			return false;
		}

		// If the root node is not "Scene", the file is most likely invalid
		if (!data["Scene"])
			return false;

		std::string name = data["Scene"].as<std::string>();
		EP_CORE_INFO("Deserializing scene '{}'", name);

		auto entities = data["Entities"];

		if (!entities)
		{
			EP_CORE_WARN("Scene has no entities, are you sure this is correct?");
			return true;
		}

		for (auto entity : entities)
		{
			UUID uuid = entity["Entity"].as<uint64_t>();
			std::string name;

			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
				name = tagComponent["Tag"].as<std::string>();

			EP_CORE_INFO("Deserializing entity '{}' ({})", name, uuid);

			Entity newEntity = m_scene->CreateEntityWithUUID(uuid, name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				auto& tc = newEntity.GetComponent<TransformComponent>();
				tc.Translation = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& cc = newEntity.AddComponent<CameraComponent>();
				auto& camera = cc.Camera;
				auto& properties = cameraComponent["Camera"];

				camera.SetProjectionType((SceneCamera::ProjectionType)properties["ProjectionType"].as<int>());
				camera.SetPerspectiveFov(properties["PerspectiveFov"].as<float>());
				camera.SetPerspectiveNearClip(properties["PerspectiveNear"].as<float>());
				camera.SetPerspectiveFarClip(properties["PerspectiveFar"].as<float>());
				camera.SetOrthographicSize(properties["OrthographicSize"].as<float>());
				camera.SetOrthographicNearClip(properties["OrthographicNear"].as<float>());
				camera.SetOrthographicFarClip(properties["OrthographicFar"].as<float>());
				
				cc.Primary = cameraComponent["Primary"].as<bool>();
			}

			auto spriteComponent = entity["SpriteComponent"];
			if (spriteComponent)
			{
				auto& sc = newEntity.AddComponent<SpriteComponent>();
				sc.TextureHandle = spriteComponent["TextureHandle"].as<uint64_t>();
				sc.Color = spriteComponent["Color"].as<glm::vec4>();
			}

			auto rigidBodyComponent = entity["RigidBodyComponent"];
			if (rigidBodyComponent)
			{
				auto& rbc = newEntity.AddComponent<RigidBodyComponent>();
				rbc.Type = Converter::RigidBodyTypeFromString(rigidBodyComponent["BodyType"].as<std::string>());
				rbc.FixedRotation = rigidBodyComponent["FixedRotation"].as<bool>();
			}

			auto boxColliderComponent = entity["BoxColliderComponent"];
			if (boxColliderComponent)
			{
				auto& bcc = newEntity.AddComponent<BoxColliderComponent>();
				bcc.Offset = boxColliderComponent["Offset"].as<glm::vec2>();
				bcc.Size = boxColliderComponent["Size"].as<glm::vec2>();
				bcc.Density = boxColliderComponent["Density"].as<float>();
				bcc.Friction = boxColliderComponent["Friction"].as<float>();
				bcc.Restitution = boxColliderComponent["Restitution"].as<float>();
				bcc.RestitutionThreshold = boxColliderComponent["RestitutionThreshold"].as<float>();
			}
		}

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		EP_CORE_ASSERT(entity.HasComponent<IDComponent>() && entity.HasComponent<TagComponent>(), "Trying to serialize invalid entity!");
		EP_CORE_INFO("Serializing entity '{}' ({})", entity.GetName(), entity.GetUUID());

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent" << YAML::Value;
			out << YAML::BeginMap;

			out << YAML::Key << "Tag" << YAML::Value << entity.GetName();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent" << YAML::Value;
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << c.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << c.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << c.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent" << YAML::Value;
			out << YAML::BeginMap;

			// TODO: What do we serialize?
			auto& c = entity.GetComponent<CameraComponent>();
			auto& camera = c.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;

			out << YAML::Key << "ProjectionType" << YAML::Value << (int) camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFov" << YAML::Value << camera.GetPerspectiveFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();

			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << c.Primary;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent" << YAML::Value;
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "TextureHandle" << YAML::Value << c.TextureHandle;
			out << YAML::Key << "Color" << YAML::Value << c.Color;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBodyComponent>())
		{
			out << YAML::Key << "RigidBodyComponent" << YAML::Value;
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<RigidBodyComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << Converter::RigidBodyTypeToString(c.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << c.FixedRotation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxColliderComponent>())
		{
			out << YAML::Key << "BoxColliderComponent" << YAML::Value;
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<BoxColliderComponent>();
			out << YAML::Key << "Offset" << YAML::Value << c.Offset;
			out << YAML::Key << "Size" << YAML::Value << c.Size;
			out << YAML::Key << "Density" << YAML::Value << c.Density;
			out << YAML::Key << "Friction" << YAML::Value << c.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << c.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << c.RestitutionThreshold;
		
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}
}