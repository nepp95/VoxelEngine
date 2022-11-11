#pragma once

#include "Engine/Core/UUID.h"

#include <entt/entt.hpp>

namespace VoxelEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Update(float ts);
		void Render();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);

		template<typename T>
		static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap);
		template<typename T>
		static void CopyComponentIfExists(Entity dst, Entity src);
		static Ref<Scene> Copy(Ref<Scene> scene);

		void OnRuntimeStart();
		void OnRuntimeStop();

		Entity GetCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}