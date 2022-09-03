// VoxelEngine - Engine
// RenderCommand.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 22:04
// Last update: 03-09-2022 17:16

#pragma once

#include "Math/Vector.h"

namespace VoxelEngine
{
	class RenderCommand
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const Vec4& color);
		static void Clear();
		//static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
	};
}
