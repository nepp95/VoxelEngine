#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace VoxelEngine
{
	struct Vertex
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};
}