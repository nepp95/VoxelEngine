#pragma once

#include "Level/Components.h"
#include "Level/Level.h"

#include <entt/entt.hpp>

namespace VoxelEngine
{
	using EntityHandle = entt::entity;

	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityHandle handle, Level* level);
		Entity(const Entity& other) = default;

		// Component logic
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			VE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_level->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			//m_level->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_level->m_registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
			//m_level->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			VE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_level->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_level->m_registry.all_of<T>(m_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			VE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_level->m_registry.remove<T>(m_entityHandle);
		}
		//

		operator bool() const { return m_entityHandle != entt::null; }
		operator EntityHandle() const { return m_entityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_level == other.m_level;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		EntityHandle m_entityHandle{ entt::null };
		Level* m_level{ nullptr };
	};
}