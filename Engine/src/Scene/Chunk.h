#pragma once

#include "Scene/Block.h"
#include "Scene/Scene.h"
#include "Renderer/AABB.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace VoxelEngine
{
	class Entity;
	class Scene;

	class Chunk : public RefCounted
	{
	public:
		Chunk(Scene* scene, const glm::vec3& position);
		~Chunk();

		Scene* GetScene() const { return m_scene; }
		bool IsBlockAtPosition(const glm::vec3& position) const;
		bool IsGenerated() const { return m_isGenerated; }

		AABB GetAABB() const { return m_aabb; }

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
		Scene* m_scene;
		glm::vec3 m_position;
		AABB m_aabb;
		bool m_isGenerated{ false };

		// Every entity component is stored in the registry
		entt::registry m_registry;
		// Lookup map for block types at a specific. Commonly used for neighbour checks
		std::unordered_map<glm::vec3, BlockType*> m_blocks; // TODO: What if position changes?

		friend class Entity;
	};
}