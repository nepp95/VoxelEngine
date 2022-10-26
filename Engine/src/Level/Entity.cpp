#include "pch.h"
#include "Entity.h"

namespace VoxelEngine
{
	Entity::Entity(EntityHandle handle, Level* level, Chunk* chunk)
		: m_entityHandle(handle), m_level(level), m_chunk(chunk)
	{}

	Entity::Entity(EntityHandle handle, Level* level)
		: m_entityHandle(handle), m_level(level)
	{}
}