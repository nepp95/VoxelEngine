#pragma once

#include "Core/UUID.h"

#include <entt/entt.hpp>

namespace VoxelEngine
{
	class Entity;

	class Level : public RefCounted
	{
	public:
		Level() = default;
		~Level() = default;

		void OnUpdate(float ts);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void DuplicateEntity(Entity entity);

		Entity GetCameraEntity();

	private:
		entt::registry m_registry;

		friend class Entity;
	};
}