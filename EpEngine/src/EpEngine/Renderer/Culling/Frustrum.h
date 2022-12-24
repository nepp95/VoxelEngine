#pragma once

#include "EpEngine/Renderer/AABB.h"

#include <glm/glm.hpp>

namespace EpEngine
{
	class Frustrum
	{
	public:
		static bool Intersects(AABB aabb);
		static void SetPlanes(const glm::mat4& vp);
	};
}