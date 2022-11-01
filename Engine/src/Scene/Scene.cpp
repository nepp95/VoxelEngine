#include "pch.h"
#include "Scene.h"

#include "Asset/AssetManager.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/Renderer.h"

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

	void Scene::Update(float ts)
	{
		VE_PROFILE_FUNCTION();
	}

	void Scene::Render()
	{
		VE_PROFILE_FUNCTION();

		// Get camera entity
		Entity cameraEntity = GetCameraEntity();
		Camera* camera = &cameraEntity.GetComponent<CameraComponent>().Camera;

		// Begin render
		Renderer::BeginScene(*camera);

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