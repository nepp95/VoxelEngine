#include "pch.h"
#include "Scene.h"

#include "Asset/AssetManager.h"
#include "Scene/Chunk.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/Culling/Frustrum.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	Scene::Scene()
	{
		VE_PROFILE_FUNCTION();

		GenerateScene();
	}

	Scene::~Scene()
	{
		VE_PROFILE_FUNCTION();
		
		m_chunks.clear();
	}

	void Scene::GenerateScene(uint64_t seed)
	{
		VE_PROFILE_FUNCTION();

		m_sceneData.Seed = seed;

		// Amount of chunks!
		const int xSize = 16;
		const int zSize = 16;

		for (int x = -xSize / 2; x < xSize / 2; x++)
		{
			for (int z = -zSize / 2; z < zSize / 2; z++)
			{
				glm::vec3 position = { x, 0.0f, z };

				Ref<Chunk> chunk = Ref<Chunk>::Create(this, position);
				m_chunks[position] = chunk;
			}
		}
	}

	void Scene::Update(float ts)
	{
		VE_PROFILE_FUNCTION();
	}

	void Scene::Render()
	{
		VE_PROFILE_FUNCTION();

		ResetSceneRenderStats();

		// Get camera entity
		Entity cameraEntity = GetCameraEntity();
		Camera* camera = &cameraEntity.GetComponent<CameraComponent>().Camera;

		// Begin render
		Renderer::BeginScene(*camera);

		// Get all chunks within range
		const float viewDistance{ 64.0f };
		auto cameraPosition = camera->GetPosition();
		
		for (auto& chunk : m_chunks)
		{
			auto chunkPosition = chunk.first * 16.0f;
			bool renderChunk{ true };

			// View distance
			if (glm::length(chunkPosition - cameraPosition) > viewDistance)
			{
				m_sceneRenderStats.chunksCulledByViewDistance++;
				renderChunk = false;
			}

			// Frustrum culling
			else if (!Frustrum::Intersects(chunk.second->GetAABB()))
			{
				m_sceneRenderStats.chunksCulledByFrustrum++;
				renderChunk = false;
			}

			if (renderChunk)
			{
				m_sceneRenderStats.chunksRendered++;
				chunk.second->Render();
			}
		}

#if 0
		VE_CORE_INFO("Chunks rendered: %/%. Culled total: %. Culled by view distance: %. Culled by frustrum: %",
			m_sceneRenderStats.chunksRendered,
			m_chunks.size(),
			m_chunks.size() - m_sceneRenderStats.chunksRendered,
			m_sceneRenderStats.chunksCulledByViewDistance,
			m_sceneRenderStats.chunksCulledByFrustrum
		);
#endif

		// End render
		Renderer::EndScene();
	}

	Entity Scene::CreateEntity(const std::string& name, Chunk* chunk)
	{
		if (chunk)
			return chunk->CreateEntity(name);
		else
			return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
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

	void Scene::DestroyEntity(Entity entity, Chunk* chunk)
	{
		// Remove entity and all its components from registry
		if (chunk)
			chunk->DestroyEntity(entity);
		else
			m_registry.destroy(entity);
	}

	void Scene::DuplicateEntity(Entity entity, Chunk* chunk)
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

	const Ref<Chunk>& Scene::GetChunk(const glm::vec3& position) const
	{
		if (m_chunks.find(position) != m_chunks.end())
		{
			const auto& chunk = m_chunks.at(position);
			if (chunk->IsGenerated())
				return chunk;
		}

		return nullptr;
	}

	Entity Scene::GetCameraEntity()
	{
		// Get all entities with a camera component
		auto view = m_registry.view<CameraComponent>();
		
		// Return the first entity found with a camera component, typically there is only one
		for (auto entity : view)
			return Entity{ entity, this };

		// Return empty entity when no entities with a camera component are found
		return {};
	}

	void Scene::ResetSceneRenderStats()
	{
		memset(&m_sceneRenderStats, 0, sizeof(SceneRenderStats));
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		// When no component specified, we do not want to compile
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<BlockComponent>(Entity entity, BlockComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{}
}