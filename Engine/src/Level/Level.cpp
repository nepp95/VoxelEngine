#include "pch.h"
#include "Level.h"

#include "Asset/AssetManager.h"
#include "Level/Components.h"
#include "Level/Entity.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	Level::Level()
	{
		GenerateLevel();
	}

	Level::~Level()
	{
		// Clear raw pointers
		m_blocks.clear();
	}

	void Level::GenerateLevel(uint64_t seed)
	{
		for (int x = 0; x < 25; x++)
		{
			for (int z = 0; z < 25; z++)
			{
				for (int y = 0; y < 5; y++)
				{
					auto entity = CreateEntity("TestEntity");

					auto& tc = entity.GetComponent<TransformComponent>();
					tc.Translation = { x * 1.0f, y * 1.0f, z * 1.0f };

					auto& bc = entity.AddComponent<BlockComponent>();
					bc.Data.Type = BlockType::Grass;
					bc.Data.TextureHandle = AssetManager::GetMetadata("assets/textures/grass.png").Handle;

				}
			}
		}
	}

	void Level::OnUpdate(float ts)
	{
		// Get camera entity
		Entity cameraEntity = GetCameraEntity();
		Camera* camera = &cameraEntity.GetComponent<CameraComponent>().Camera;

		// Begin render
		Renderer::BeginScene(*camera);

		// Get all components with both a transform component and a block component
		auto view = m_registry.view<TransformComponent, BlockComponent>();

		for (auto entity : view)
		{
			auto [transform, block] = view.get<TransformComponent, BlockComponent>(entity);

			// Neighbour check
			auto t = transform.Translation;
			bool drawSides[6] { true, true, true, true, true, true };

			if (m_blocks.find(glm::vec3( t.x, t.y, t.z - 1.0f )) != m_blocks.end())
				drawSides[(int) QuadSide::Front] = false;
			if (m_blocks.find(glm::vec3( t.x + 1.0f, t.y, t.z )) != m_blocks.end())
				drawSides[(int) QuadSide::Right] = false;
			if (m_blocks.find(glm::vec3( t.x, t.y, t.z + 1.0f )) != m_blocks.end())
				drawSides[(int) QuadSide::Back] = false;
			if (m_blocks.find(glm::vec3( t.x - 1.0f, t.y, t.z )) != m_blocks.end())
				drawSides[(int) QuadSide::Left] = false;
			if (m_blocks.find(glm::vec3( t.x, t.y - 1.0f, t.z )) != m_blocks.end())
				drawSides[(int) QuadSide::Bottom] = false;
			if (m_blocks.find(glm::vec3( t.x, t.y + 1.0f, t.z )) != m_blocks.end())
				drawSides[(int) QuadSide::Top] = false;

			Renderer::DrawEntity(transform.GetTransform(), block, drawSides);
		}

		// End render
		Renderer::EndScene();
	}

	Entity Level::CreateEntity(const std::string& name)
	{
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

	void Level::DestroyEntity(Entity entity)
	{
		// Remove entity and all its components from registry
		m_registry.destroy(entity);
	}

	void Level::DuplicateEntity(Entity entity)
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
	{
		VE_CORE_ASSERT(entity.HasComponent<TransformComponent>(), "Block needs a transform!");
		
		auto& tc = entity.GetComponent<TransformComponent>();
		m_blocks[tc.Translation] = &component.Data.Type;
	}

	template<>
	void Level::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{}
}