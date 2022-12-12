#include "pch.h"
#include "Scene.h"

#include "Engine/Asset/AssetManager.h"
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Renderer/Renderer.h"

namespace VoxelEngine
{
	Scene::Scene()
	{
		VE_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
		VE_PROFILE_FUNCTION();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
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

	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		if (entity.HasComponent<TransformComponent>())
			newEntity.AddOrReplaceComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		if (entity.HasComponent<CameraComponent>())
			newEntity.AddOrReplaceComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
	}

	template<typename T>
	static void Scene::CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<T>();

		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity));
			auto& srcComponent = src.get<T>(srcEntity);
			dst.emplace_or_replace<T>(dstEntity, srcComponent);
		}
	}

	template<typename T>
	static void Scene::CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<T>())
			dst.AddOrReplaceComponent<T>(src.GetComponent<T>());
	}

	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> newScene = CreateRef<Scene>();
		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcRegistry = scene->m_registry;
		auto& dstRegistry = newScene->m_registry;
		auto idView = srcRegistry.view<IDComponent>();

		for (auto entity : idView)
		{
			UUID uuid = srcRegistry.get<IDComponent>(entity);
			const auto& name = srcRegistry.get<TagComponent>(entity);
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity;
		}

		CopyComponent<TransformComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<CameraComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<SpriteComponent>(dstRegistry, srcRegistry, enttMap);

		return newScene;
	}

	void Scene::OnUpdateRuntime(float ts)
	{

	}

	void Scene::OnRenderRuntime()
	{

	}

	void Scene::OnRenderEditor(EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnRuntimeStart()
	{

	}

	void Scene::OnRuntimeStop()
	{

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		auto view = m_registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);

			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetCameraEntity()
	{
		// Get all entities with a camera component
		auto view = m_registry.view<CameraComponent>();
		
		// Return the first entity found with a primary camera
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}

		// Return empty entity when no entities with a camera component are found
		return {};
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		VE_PROFILE_FUNCTION();

		// Begin render
		Renderer::BeginScene(camera);

		// Sprite components
		auto view = m_registry.view<TransformComponent, SpriteComponent>();

		for (auto entity : view)
		{
			auto& [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
			Renderer::DrawEntity(transform.GetTransform(), sprite);
		}

		// End render
		Renderer::EndScene();
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
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
	{}
}