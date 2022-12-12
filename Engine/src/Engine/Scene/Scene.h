#pragma once

#include "Engine/Core/UUID.h"
#include "Engine/Renderer/Camera/EditorCamera.h"

#include <entt/entt.hpp>

namespace VoxelEngine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);

		template<typename T>
		static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap);
		template<typename T>
		static void CopyComponentIfExists(Entity dst, Entity src);
		static Ref<Scene> Copy(Ref<Scene> scene);

		void OnUpdateRuntime(float ts);

		void OnRenderRuntime();
		void OnRenderEditor(EditorCamera& camera);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetCameraEntity();

	private:
		void RenderScene(EditorCamera& camera);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth{ 0 };
		uint32_t m_viewportHeight{ 0 };

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}