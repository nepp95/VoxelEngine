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
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls{ 0 };

			uint32_t QuadCount{ 0 };
			uint32_t VertexCount{ 0 };
			uint32_t IndexCount{ 0 };

			void AddQuad()
			{
				QuadCount++;
				VertexCount += 4;
				IndexCount += 6;
			}
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
