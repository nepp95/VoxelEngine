#pragma once

#include "Core/Application.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"

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
		// Render Quad
		static void DrawQuad(const glm::vec2& position, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		// Render Quad with Texture
		static void DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		// Render Entity
		static void DrawEntity(const glm::vec2& position, const SpriteComponent& sc);
		static void DrawEntity(const glm::vec3& position, const SpriteComponent& sc);
		static void DrawEntity(const glm::mat4& transform, const SpriteComponent& sc);

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
