// VoxelEngine - Engine
// Vertex.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 20-09-2022 15:15
// Last update: 20-09-2022 18:43

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace VoxelEngine
{
	struct Vertex
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	};
}