#pragma once

#include "Core/UUID.h"
#include "Level/Block.h"
#include "Utility/Random.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <xhash>

// Comparison for std::map and glm::vec3
namespace std
{
	// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
	template<>
	struct hash<glm::vec3>
	{
		std::size_t operator()(const glm::vec3& v) const
		{
			using std::hash;

			return ((hash<float>()(v.x)
				^ (hash<float>()(v.y) << 1)) >> 1)
				^ (hash<float>()(v.z) << 1);
		}
	};
}

namespace VoxelEngine
{
	class Entity;

	class Level : public RefCounted
	{
	public:
		Level();
		~Level();

		void GenerateLevel(uint64_t seed = Utility::GenerateRandomUInt64());
		void OnUpdate(float ts);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);

		Entity GetCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		// Every entity component is stored in the registry
		entt::registry m_registry;
		// Lookup map for block types at a specific. Commonly used for neighbour checks
		std::unordered_map<glm::vec3, BlockType*> m_blocks; // TODO: What if position changes?

		friend class Entity;
	};
}