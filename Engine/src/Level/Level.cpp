#include "pch.h"
#include "Level.h"

#include "Asset/AssetManager.h"
#include "LEvel/Chunk.h"
#include "Level/Components.h"
#include "Level/Entity.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	Level::Level()
	{
		VE_PROFILE_FUNCTION();

		GenerateLevel();
	}

	Level::~Level()
	{
		VE_PROFILE_FUNCTION();
		
		m_chunks.clear();
	}

	void Level::GenerateLevel(uint64_t seed)
	{
		VE_PROFILE_FUNCTION();

		// Amount of chunks!
		const int xSize = 8;
		const int zSize = 8;

		for (int x = -xSize / 2; x < xSize / 2; x++)
		{
			for (int z = -zSize / 2; z < zSize / 2; z++)
			{
				glm::vec3 position = { x, 1.0f, z };

				Ref<Chunk> chunk = Ref<Chunk>::Create(this, position);
				m_chunks[position] = chunk;
			}
		}
	}

	void Level::Update(float ts)
	{
		VE_PROFILE_FUNCTION();
	}

	void Level::Render()
	{
		VE_PROFILE_FUNCTION();

		// Get camera entity
		Entity cameraEntity = GetCameraEntity();
		Camera* camera = &cameraEntity.GetComponent<CameraComponent>().Camera;

		// Begin render
		Renderer::BeginScene(*camera);

		// Get all chunks within range
		auto cameraPosition = camera->GetPosition();

		for (auto& chunk : m_chunks)
		{
			auto chunkPosition = chunk.first * 16.0f;
			if (glm::length(chunkPosition - cameraPosition) < 64.0f)
				chunk.second->Render();
		}

		// End render
		Renderer::EndScene();
	}

	Entity Level::CreateEntity(const std::string& name, Chunk* chunk)
	{
		if (chunk)
			return chunk->CreateEntity(name);
		else
			return CreateEntityWithUUID(UUID(), name);
	}

	Entity Level::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		// Create new entity
		Entity entity{ m_registry.create(), this };
		
		// Add mandatory components
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		// Add name from param
		auto& tc = entity.AddComponent<TagComponent>();
		tc.Tag = name.empty() ? "Entity" : name;

		// Return newly created entity
		return entity;
	}

	void Level::DestroyEntity(Entity entity, Chunk* chunk)
	{
		// Remove entity and all its components from registry
		if (chunk)
			chunk->DestroyEntity(entity);
		else
			m_registry.destroy(entity);
	}

	void Level::DuplicateEntity(Entity entity, Chunk* chunk)
	{
		if (chunk)
			chunk->DuplicateEntity(entity);
		else
		{
			std::string name = entity.GetName();
			Entity newEntity = CreateEntity(name);

			if (entity.HasComponent<TransformComponent>())
				newEntity.AddOrReplaceComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
			if (entity.HasComponent<BlockComponent>())
				newEntity.AddOrReplaceComponent<BlockComponent>(entity.GetComponent<BlockComponent>());
			if (entity.HasComponent<CameraComponent>())
				newEntity.AddOrReplaceComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
		}
	}

	Entity Level::GetCameraEntity()
	{
		// Get all entities with a camera component
		auto view = m_registry.view<CameraComponent>();
		
		// Return the first entity found with a camera component, typically there is only one
		for (auto entity : view)
			return Entity{ entity, this };

		// Return empty entity when no entities with a camera component are found
		return {};
	}

	template<typename T>
	void Level::OnComponentAdded(Entity entity, T& component)
	{
		// When no component specified, we do not want to compile
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Level::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{}

	template<>
	void Level::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{}

	template<>
	void Level::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}

	template<>
	void Level::OnComponentAdded<BlockComponent>(Entity entity, BlockComponent& component)
	{}

	template<>
	void Level::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{}
}