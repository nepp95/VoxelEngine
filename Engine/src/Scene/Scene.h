#pragma once

#include "Core/UUID.h"
#include <entt/entt.hpp>

namespace VoxelEngine
{
	class Entity;

	class Scene : public RefCounted
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

		Entity GetCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_registry;

		friend class Entity;
	};
}