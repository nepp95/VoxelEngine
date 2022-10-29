#include "pch.h"
#include "Chunk.h"

#include "Asset/AssetManager.h"
#include "Level/Entity.h"
#include "Level/Level.h"
#include "Renderer/Renderer.h"

namespace VoxelEngine
{
	Chunk::Chunk(Level* level, const glm::vec3& position)
		: m_level(level), m_position(position)
	{
		VE_PROFILE_FUNCTION();

		// Generate chunk
		const int xSize = 16;
		const int ySize = 32;
		const int zSize = 16;

		for (int x = 0; x < xSize; x++)
		{
			for (int z = 0; z < zSize; z++)
			{
				for (int y = 0; y < ySize; y++)
				{
					auto entity = CreateEntity();

					auto& tc = entity.GetComponent<TransformComponent>();
					tc.Translation = { x * 1.0f, y * 1.0f, z * 1.0f };

					auto& bc = entity.AddComponent<BlockComponent>();
					bc.Data.Type = BlockType::Grass;
					bc.Data.TextureHandle = AssetManager::GetMetadata("assets/textures/grass.png").Handle;
				}
			}
		}

		// Set AABB
		m_aabb = {
			{ m_position.x * 16.0f, 0.0f, m_position.z * 16.0f },
			{ m_position.x * 16.0f + xSize, (float)ySize, m_position.z * 16.0f + zSize }
		};

		m_isGenerated = true;
	}

	Chunk::~Chunk()
	{
		m_blocks.clear();
	}

	bool Chunk::IsBlockAtPosition(const glm::vec3& position) const
	{
		if (m_blocks.empty())
			return false;

		if (m_blocks.find(position) != m_blocks.end())
			return true;
		return false;
	}

	Entity Chunk::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Chunk::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		// Create new entity
		Entity entity{ m_registry.create(), m_level, this };

		// Add mandatory components
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		// Add name from param
		auto& tc = entity.AddComponent<TagComponent>();
		tc.Tag = name.empty() ? "Entity" : name;

		// Return newly created entity
		return entity;
	}

	void Chunk::DestroyEntity(Entity entity)
	{
		// Remove entity and all its components from registry
		m_registry.destroy(entity);
	}

	void Chunk::DuplicateEntity(Entity entity)
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

	void Chunk::Update()
	{
		VE_PROFILE_FUNCTION();
	}

	void Chunk::Render()
	{
		VE_PROFILE_FUNCTION();

		auto view = m_registry.view<TransformComponent, BlockComponent>();

		for (auto entity : view)
		{
			auto [transform, block] = view.get<TransformComponent, BlockComponent>(entity);

			// Neighbour check
			auto t = transform.Translation;

			// todo: Cube drawing, replace with face drawing
			bool drawCube{ false };

			if (m_blocks.find(glm::vec3(t.x, t.y, t.z - 1.0f)) == m_blocks.end())
			{
				if (t.z == 0.0f)
				{
					// If at border of chunk, check neighbouring chunk
					const auto& chunk = m_level->GetChunk({ m_position.x, m_position.y, m_position.z - 1.0f });
					if (chunk)
						if (!chunk->IsBlockAtPosition({ t.x, t.y, 15.0f }))
							drawCube = true;
				} else
					drawCube = true;
			}

			if (m_blocks.find(glm::vec3(t.x + 1.0f, t.y, t.z)) == m_blocks.end())
			{
				if (t.x == 15.0f)
				{
					// If at border of chunk, check neighbouring chunk
					const auto& chunk = m_level->GetChunk({ m_position.x + 1.0f, m_position.y, m_position.z });
					if (chunk)
						if (!chunk->IsBlockAtPosition({ 0.0f, t.y, t.z }))
							drawCube = true;
				}
				else
					drawCube = true;
			}

			if (m_blocks.find(glm::vec3(t.x, t.y, t.z + 1.0f)) == m_blocks.end())
			{
				if (t.z == 15.0f)
				{
					// If at border of chunk, check neighbouring chunk
					const auto& chunk = m_level->GetChunk({ m_position.x, m_position.y, m_position.z + 1.0f });
					if (chunk)
						if (!chunk->IsBlockAtPosition({ t.x, t.y, 0.0f }))
							drawCube = true;
				}
				else
					drawCube = true;
			}

			if (m_blocks.find(glm::vec3(t.x - 1.0f, t.y, t.z)) == m_blocks.end())
			{
				if (t.x == 0.0f)
				{
					// If at border of chunk, check neighbouring chunk
					const auto& chunk = m_level->GetChunk({ m_position.x - 1.0f, m_position.y, m_position.z });
					if (chunk)
						if (!chunk->IsBlockAtPosition({ 15.0f, t.y, t.z }))
							drawCube = true;
				}
				else
					drawCube = true;
			}

			if (m_blocks.find(glm::vec3(t.x, t.y - 1.0f, t.z)) == m_blocks.end())
				drawCube = true;
			if (m_blocks.find(glm::vec3(t.x, t.y + 1.0f, t.z)) == m_blocks.end())
				drawCube = true;

			if (drawCube)
				Renderer::DrawEntity(transform.Translation + (m_position * 16.0f), block);
		}
	}

	template<typename T>
	void Chunk::OnComponentAdded(Entity entity, T& component)
	{
		// When no component specified, we do not want to compile
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Chunk::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{}

	template<>
	void Chunk::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{}

	template<>
	void Chunk::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}

	template<>
	void Chunk::OnComponentAdded<BlockComponent>(Entity entity, BlockComponent& component)
	{
		VE_CORE_ASSERT(entity.HasComponent<TransformComponent>(), "Block needs a transform!");

		auto& tc = entity.GetComponent<TransformComponent>();
		m_blocks[tc.Translation] = &component.Data.Type;
	}

	template<>
	void Chunk::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{}
}