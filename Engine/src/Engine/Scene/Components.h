#pragma once

#include "Engine/Asset/Asset.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/Camera/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

enum b2BodyType;

namespace VoxelEngine
{
	struct IDComponent
	{
		UUID uuid;

		IDComponent() = default;
		IDComponent(const IDComponent& other) = default;

		operator UUID&() { return uuid; }
		operator const UUID&() const { return uuid; }
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{}

		operator std::string&() { return Tag; }
		operator const std::string&() const { return Tag; }
	};
	
	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{}

		glm::mat4 GetTransform() const
		{
			const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary{ true };

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct SpriteComponent
	{
		AssetHandle TextureHandle{ 0 };
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
	};

	struct RigidBodyComponent
	{
		enum class BodyType { Static, Dynamic, Kinematic };
		BodyType Type{ BodyType::Static };
		bool FixedRotation{ false };

		void* RuntimeBody{ nullptr };

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;
	};

	struct BoxColliderComponent
	{
		glm::vec2 Offset{ 0.0f, 0.0f };
		glm::vec2 Size{ 0.5f, 0.5f };

		float Density{ 1.0f };
		float Friction{ 0.5f };
		float Restitution{ 0.0f };
		float RestitutionThreshold{ 0.5f };

		void* RuntimeFixture{ nullptr };

		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent&) = default;
	};
}