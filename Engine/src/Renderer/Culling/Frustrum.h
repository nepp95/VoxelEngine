#pragma once

#include "Renderer/AABB.h"
#include "Renderer/Camera.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	struct FrustrumPlanes
	{
		glm::vec4 NearRight;
		glm::vec4 NearLeft;
		glm::vec4 NearTop;
		glm::vec4 NearBottom;

		glm::vec4 Near;
		glm::vec4 Far;
	};

	class Frustrum
	{
	public:
		static bool Intersects(AABB aabb);
		static void SetPlanes(const glm::mat4& vp);
	};
}