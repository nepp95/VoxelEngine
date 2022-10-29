#pragma once

#include <glm/vec3.hpp>

namespace VoxelEngine
{
	struct AABB
	{
		glm::vec3 Min;
		glm::vec3 Max;
	};
}