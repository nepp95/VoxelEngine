#include "pch.h"
#include "Entity.h"

namespace VoxelEngine
{
	Entity::Entity(EntityHandle handle, Scene* scene, Chunk* chunk)
		: m_entityHandle(handle), m_scene(scene), m_chunk(chunk)
	{}

	Entity::Entity(EntityHandle handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{}
}