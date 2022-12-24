#include "pch.h"
#include "Entity.h"

namespace EpEngine
{
	Entity::Entity(EntityHandle handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{}
}