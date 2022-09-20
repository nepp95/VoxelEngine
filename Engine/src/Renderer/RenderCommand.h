// VoxelEngine - Engine
// RenderCommand.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 03-09-2022 17:26
// Last update: 20-09-2022 18:44

#pragma once

#include <glm/glm.hpp>

namespace VoxelEngine
{
	class RenderCommand
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		//static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
	};
}
