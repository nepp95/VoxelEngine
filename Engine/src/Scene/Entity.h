#pragma once

#include "Scene/Components.h"
#include "Scene/Scene.h"

#include <entt/entt.hpp>

namespace VoxelEngine
{
	using EntityHandle = entt::entity;

	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityHandle handle, Scene* scene);
		Entity(const Entity& other) = default;

		const std::string& GetName() { return GetComponent<TagComponent>(); }

		// Component logic
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			VE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_scene->m_registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			VE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			VE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_scene->m_registry.remove<T>(m_entityHandle);
		}
		//

		// Operators
		operator bool() const { return m_entityHandle != entt::null; }
		operator EntityHandle() const { return m_entityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		EntityHandle m_entityHandle{ entt::null };
		Scene* m_scene{ nullptr };
	};
}