#include "pch.h"
#include "Renderer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Vertex.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace VoxelEngine
{
	struct RendererData
	{
		// Batch limits
		const uint32_t MaxQuads{ 20000 };
		const uint32_t MaxVertices{ MaxQuads * 4 };
		const uint32_t MaxIndices{ MaxQuads * 6 };
		const uint32_t MaxTextureSlots{ 32 };

		// Quad Data
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Vertex* QuadVertexBufferBase{ nullptr };
		Vertex* QuadVertexBufferPtr{ nullptr };
		glm::vec4 QuadVertexPositions[4];
		uint32_t QuadIndexCount{ 0 };

		// Shader
		Ref<Shader> BaseShader;

		// Camera
		glm::mat4 ViewProjection;

		// Stats
		Renderer::Statistics Stats;
	};

	static RendererData s_data;

	void Renderer::Init()
	{
		RenderCommand::Init();

		// Create vertex array and buffers
		s_data.QuadVertexArray = CreateRef<VertexArray>();
		s_data.QuadVertexBuffer = CreateRef<VertexBuffer>(s_data.MaxVertices * sizeof(Vertex));

		s_data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" }
		});

		s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);
		s_data.QuadVertexBufferBase = new Vertex[s_data.MaxVertices];

		// Index buffer
		uint32_t* indices = new uint32_t[s_data.MaxIndices];
		uint32_t offset{ 0 };

		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>(indices, s_data.MaxIndices);
		s_data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		// Set quad vertex positions
		s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Load shader
		s_data.BaseShader = CreateRef<Shader>("assets/shaders/quad.glsl");
	}

	void Renderer::Shutdown()
	{ 
		delete[] s_data.QuadVertexBufferBase;
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		s_data.ViewProjection = camera.GetViewProjectionMatrix();

		StartBatch();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_data.QuadIndexCount >= s_data.MaxIndices)
			NextBatch();

		const int quadVertexCount{ 4 };

		for (int i = 0; i < quadVertexCount; i++)
		{
			s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[i];
			s_data.QuadVertexBufferPtr->Color = color;
			s_data.QuadVertexBufferPtr++;
		}

		s_data.QuadIndexCount += 6;
		s_data.Stats.AddQuad();
	}

	void Renderer::ResetStats()
	{
		// Because we want to reset each value of the struct to zero, we can do this by setting the entire memory block sizeof(Statistics) to zero.
		memset(&s_data.Stats, 0, sizeof(Statistics));
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_data.Stats;
	}

	void Renderer::Flush()
	{
		if (s_data.QuadIndexCount)
		{
			// Calculate data size
			uint32_t dataSize = (uint32_t) ((uint8_t*) s_data.QuadVertexBufferPtr - (uint8_t*) s_data.QuadVertexBufferBase);
			s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);

			// Create draw call
			s_data.BaseShader->Bind();
			s_data.BaseShader->SetMat4("uViewProjection", s_data.ViewProjection);
			RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
			s_data.Stats.DrawCalls++;
		}
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer::StartBatch()
	{
		s_data.QuadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
	}
}
