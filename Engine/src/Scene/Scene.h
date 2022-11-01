#pragma once

#include "Core/UUID.h"
#include "Scene/Block.h"
#include "Utility/Random.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <xhash>

// Comparison for std::unordered_map and glm::vec2/3
namespace std
{
	// https://stackoverflow.com/questions/9047612/glmivec2-as-key-in-unordered-map
	template<>
	struct hash<glm::vec2>
	{
		std::size_t operator()(const glm::vec2& v) const
		{
			using std::hash;

			return hash<float>()(v.x) ^ hash<float>()(v.y);
		}
	};

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
	class Chunk;

	struct SceneData
	{
		uint32_t Seed;
	};

	struct SceneRenderStats
	{
		uint32_t chunksCulledByViewDistance{ 0 };
		uint32_t chunksCulledByFrustrum{ 0 };
		uint32_t chunksRendered{ 0 };
	};

	class Scene : public RefCounted
	{
	public:
		Scene();
		~Scene();

		void GenerateScene(uint64_t seed = Utility::GenerateRandomUInt64());

		void Update(float ts);
		void Render();

		Entity CreateEntity(const std::string& name = std::string(), Chunk* chunk = nullptr);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity, Chunk* chunk = nullptr);
		void DuplicateEntity(Entity entity, Chunk* chunk = nullptr);

		const Ref<Chunk>& GetChunk(const glm::vec3& position) const;
		Entity GetCameraEntity();

	private:
		void ResetSceneRenderStats();

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		SceneData m_sceneData;
		SceneRenderStats m_sceneRenderStats;

		// Every chunk is stored in a map
		std::unordered_map<glm::vec3, Ref<Chunk>> m_chunks;

		// Scene wide registry for things like the camera. Will probably change this later
		entt::registry m_registry;

		friend class Entity;
	};
}