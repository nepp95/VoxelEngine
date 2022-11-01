#pragma once

#include "Asset/Asset.h"
#include "Core/UUID.h"
#include "Renderer/Camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace VoxelEngine
{
	struct IDComponent
	{
		UUID uuid;

		IDComponent() = default;
		IDComponent(const IDComponent& other) = default;
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
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
		// TODO: Add rotation

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		Camera Camera{ { 0.0f, 0.0f, 3.0f } };

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct SpriteComponent
	{
		AssetHandle TextureHandle;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
	};
}