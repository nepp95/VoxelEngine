#pragma once

#include "Level/Block.h"
#include "Level/Level.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace VoxelEngine
{
	class Entity;
	class Level;

	class Chunk : public RefCounted
	{
	public:
		Chunk(Level* level, const glm::vec3& position);
		~Chunk();

		Level* GetLevel() const { return m_level; }

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);

		void Update();
		void Render();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		Level* m_level;
		glm::vec3 m_position;

		// Every entity component is stored in the registry
		entt::registry m_registry;
		// Lookup map for block types at a specific. Commonly used for neighbour checks
		std::unordered_map<glm::vec3, BlockType*> m_blocks; // TODO: What if position changes?

		friend class Entity;
	};
}