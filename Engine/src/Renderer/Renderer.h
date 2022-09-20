// VoxelEngine - Engine
// Renderer.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 03-09-2022 17:26
// Last update: 20-09-2022 19:06

#pragma once

#include "Renderer/Camera.h"

namespace VoxelEngine
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		// Scene
		static void BeginScene(const Camera& camera);
		static void EndScene();

		// Rendering
		static void DrawTriangle(const glm::vec3& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls{ 0 };
			uint32_t VertexCount{ 0 };
			uint32_t IndexCount{ 0 };
		};

		static void ResetStats();
		static Statistics GetStats();
		//

	private:
		static void Flush();
		static void NextBatch();
		static void StartBatch();
	};
}
