#include "pch.h"
#include "Level.h"

#include "Level/Components.h"
#include "Level/Entity.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	void Level::OnUpdate(float ts)
	{
		Entity cameraEntity = GetCameraEntity();
		Camera* camera = &cameraEntity.GetComponent<CameraComponent>().Camera;

		Renderer::BeginScene(*camera);

		auto view = m_registry.view<TransformComponent, SpriteComponent>();

		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
			Renderer::DrawSprite(transform.GetTransform(), sprite);
		}
		
		Renderer::DrawCube({ 0.0f, 1.0f, 0.0f });

		Renderer::EndScene();
	}

	Entity Level::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Level::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity{ m_registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Level::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Level::DuplicateEntity(Entity entity)
	{
	}

	Entity Level::GetCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		
		for (auto entity : view)
		{
			return Entity{ entity, this };
		}

		return {};
	}
}