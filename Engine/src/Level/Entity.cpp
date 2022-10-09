#include "pch.h"
#include "Entity.h"

namespace VoxelEngine
{
	Entity::Entity(entt::entity handle, Level* level)
		: m_entityHandle(handle), m_level(level)
	{}
}