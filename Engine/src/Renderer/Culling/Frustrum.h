#pragma once

#include "Renderer/AABB.h"
#include "Renderer/Camera.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	class Frustrum
	{
	public:
		static bool Intersects(AABB aabb);
		static void SetPlanes(const glm::mat4& vp);
	};
}