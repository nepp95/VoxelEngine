#pragma once

#include "Level/Block.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"

namespace VoxelEngine
{
	enum class QuadSide
	{
		Front = 0,
		Right,
		Back,
		Left,
		Bottom,
		Top,

		Default = Front
	};

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		// Scene
		static void BeginScene(const Camera& camera);
		static void EndScene();

		// Rendering
		// Render Quad
		static void DrawQuad(const glm::vec2& position, const glm::vec4& color = glm::vec4(1.0f), QuadSide side = QuadSide::Default);
		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4(1.0f), QuadSide side = QuadSide::Default);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, QuadSide side = QuadSide::Default);
		// Render Quad with Texture
		static void DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), QuadSide side = QuadSide::Default);
		static void DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), QuadSide side = QuadSide::Default);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), QuadSide side = QuadSide::Default);
		// Render Cube
		static void DrawCube(const glm::vec2& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawCube(const glm::vec3& position, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		// Render Cube with texture
		static void DrawCube(const glm::vec2& position, const std::vector<Ref<Texture>>& textures, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawCube(const glm::vec3& position, const std::vector<Ref<Texture>>& textures, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		// Render Block
		static void RenderBlock(const glm::vec2& position, const Ref<Block>& block);
		static void RenderBlock(const glm::vec3& position, const Ref<Block>& block);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls{ 0 };

			uint32_t CubeCount{ 0 };
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
