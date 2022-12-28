#include "pch.h"
#include "Scene.h"

#include "EpEngine/Asset/AssetManager.h"
#include "EpEngine/Scene/Components.h"
#include "EpEngine/Scene/Entity.h"
#include "EpEngine/Renderer/Renderer.h"
#include "EpEngine/Scripting/ScriptEngine.h"
#include "EpEngine/Utility/Converter.h"

#include <box2d/box2d.h>

namespace EpEngine
{
	Scene::Scene()
	{
		EP_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
		EP_PROFILE_FUNCTION();
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

		// Add entity to entity map
		m_entityMap.insert_or_assign(uuid, entity);

		// Return newly created entity
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
		m_entityMap.erase(entity.GetUUID());
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		if (entity.HasComponent<TransformComponent>())
			newEntity.AddOrReplaceComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		if (entity.HasComponent<CameraComponent>())
			newEntity.AddOrReplaceComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
		if (entity.HasComponent<SpriteComponent>())
			newEntity.AddOrReplaceComponent<SpriteComponent>(entity.GetComponent<SpriteComponent>());
		if (entity.HasComponent<RigidBodyComponent>())
			newEntity.AddOrReplaceComponent<RigidBodyComponent>(entity.GetComponent<RigidBodyComponent>());
		if (entity.HasComponent<BoxColliderComponent>())
			newEntity.AddOrReplaceComponent<BoxColliderComponent>(entity.GetComponent<BoxColliderComponent>());
		if (entity.HasComponent<ScriptComponent>())
			newEntity.AddOrReplaceComponent<ScriptComponent>(entity.GetComponent<ScriptComponent>());
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
		CopyComponent<RigidBodyComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<BoxColliderComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<ScriptComponent>(dstRegistry, srcRegistry, enttMap);

		return newScene;
	}

	void Scene::OnUpdateRuntime(float ts)
	{
		/************************************************************************/
		/* Scripting                                                            */
		/************************************************************************/
		{
			auto view = m_registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity{ e, this };
				ScriptEngine::OnUpdateEntity(entity, ts);
			}
		}

		/************************************************************************/
		/* Physics                                                              */
		/************************************************************************/
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_physicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_registry.view<RigidBodyComponent>();
			for (auto e : view)
			{
				Entity entity{ e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

				b2Body* body = (b2Body*)rigidbody.RuntimeBody;
				const auto& position = body->GetPosition();

				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}
	}

	void Scene::OnRenderRuntime()
	{
		// Get the main camera
		SceneCamera* sceneCamera{ nullptr };
		glm::mat4 cameraTransform;

		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					sceneCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();

					break;
				}
			}
		}

		if (sceneCamera)
		{
			Renderer::BeginScene(*sceneCamera, cameraTransform);
			
			auto view = m_registry.view<TransformComponent, SpriteComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
				Renderer::DrawEntity(transform.GetTransform(), sprite);
			}

			Renderer::EndScene();
		}
	}

	void Scene::OnRenderEditor(EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnRuntimeStart()
	{
		OnPhysicsStart();
		ScriptEngine::OnRuntimeStart(this);

		auto view = m_registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity{ e, this };
			ScriptEngine::OnCreateEntity(entity);
		}
	}

	void Scene::OnRuntimeStop()
	{
		OnPhysicsStop();
		ScriptEngine::OnRuntimeStop();
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

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_entityMap.find(uuid) != m_entityMap.end())
			return Entity{ m_entityMap.at(uuid), this };

		return {};
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		EP_PROFILE_FUNCTION();

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

	void Scene::OnPhysicsStart()
	{
		m_physicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_registry.view<RigidBodyComponent>();
		for (auto e : view)
		{
			Entity entity{ e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Converter::RigidBodyTypeToBox2DBodyType(rigidbody.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody.FixedRotation);
			rigidbody.RuntimeBody = body;

			if (entity.HasComponent<BoxColliderComponent>())
			{
				auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(boxCollider.Size.x * transform.Scale.x, boxCollider.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.Density;
				fixtureDef.friction = boxCollider.Friction;
				fixtureDef.restitution = boxCollider.Restitution;
				fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysicsStop()
	{
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
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

	template<>
	void Scene::OnComponentAdded<RigidBodyComponent>(Entity entity, RigidBodyComponent& component)
	{}

	template<>
	void Scene::OnComponentAdded<BoxColliderComponent>(Entity entity, BoxColliderComponent& component)
	{}
	
	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{}
}