#pragma once

#include "Asset/Asset.h"
#include "Core/UUID.h"
#include "Scene/Block.h"
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
	};
	
	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

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

	struct BlockComponent
	{
		BlockData Data{ BlockType::Air };

		BlockComponent() = default;
		BlockComponent(const BlockComponent&) = default;
	};

	struct CameraComponent
	{
		Camera Camera{ { 0.0f, 0.0f, 3.0f } };

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}