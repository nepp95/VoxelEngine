// VoxelEngine - Engine
// Vertex.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 20-09-2022 15:15
// Last update: 20-09-2022 15:44

#pragma once

#include "Math/Vector.h"

namespace VoxelEngine
{
	struct Vertex
	{
		Vec3 Position{ 0.0f, 0.0f, 0.0f };
		Vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	};
}