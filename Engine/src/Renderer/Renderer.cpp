#include "pch.h"
#include "Renderer.h"

#include "Asset/AssetManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace VoxelEngine
{
	struct QuadVertex
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct RendererData
	{
		// Batch limits
		static const uint32_t MaxQuads{ 20000 };
		static const uint32_t MaxVertices{ MaxQuads * 4 };
		static const uint32_t MaxIndices{ MaxQuads * 6 };
		static const uint32_t MaxTextureSlots{ 32 };

		// Quad Data
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		QuadVertex* QuadVertexBufferBase{ nullptr };
		QuadVertex* QuadVertexBufferPtr{ nullptr };
		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadVertexTextureCoords[4];
		uint32_t QuadIndexCount{ 0 };

		// Shader
		Ref<Shader> QuadShader;

		// Textures
		Ref<Texture> WhiteTexture;
		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex{ 1 };

		// Camera
		glm::mat4 ViewProjection;

		// Stats
		Renderer::Statistics Stats;
	};

	static RendererData s_data;

	void Renderer::Init()
	{
		VE_PROFILE_FUNCTION();

		RenderCommand::Init();
		AssetManager::Init();

		// Create vertex array and buffers
		s_data.QuadVertexArray = CreateRef<VertexArray>();
		s_data.QuadVertexBuffer = CreateRef<VertexBuffer>(RendererData::MaxVertices * sizeof(QuadVertex));

		s_data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"aPosition"		},
			{ ShaderDataType::Float4,	"aColor"			},
			{ ShaderDataType::Float2,	"aTexCoord"		},
			{ ShaderDataType::Float,	"aTexIndex"		}
		});

		s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);
		s_data.QuadVertexBufferBase = new QuadVertex[RendererData::MaxVertices];

		// Index buffer
		uint32_t* quadIndices = new uint32_t[RendererData::MaxIndices];
		uint32_t offset{ 0 };

		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6) // 6 indices
		{
			quadIndices[i + 0]	= offset + 0;
			quadIndices[i + 1]	= offset + 1;
			quadIndices[i + 2]	= offset + 2;

			quadIndices[i + 3]	= offset + 2;
			quadIndices[i + 4]	= offset + 3;
			quadIndices[i + 5]	= offset + 0;

			offset += 4; // 4 vertices
		}

		Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>(quadIndices, RendererData::MaxIndices);
		s_data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] quadIndices;

		// Set quad vertex positions
		s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Set quad vertex texture coordinates
		s_data.QuadVertexTextureCoords[0] = { 0.0f, 0.0f };
		s_data.QuadVertexTextureCoords[1] = { 1.0f, 0.0f };
		s_data.QuadVertexTextureCoords[2] = { 1.0f, 1.0f };
		s_data.QuadVertexTextureCoords[3] = { 0.0f, 1.0f };

		// Load shader
		s_data.QuadShader = CreateRef<Shader>("assets/shaders/quad.glsl");

		// Set white texture slot (invalid texture)
		s_data.WhiteTexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Setup texture samplers
		int32_t samplers[RendererData::MaxTextureSlots];
		for (uint32_t i = 0; i < RendererData::MaxTextureSlots; i++)
			samplers[i] = i;

		s_data.TextureSlots[0] = s_data.WhiteTexture;
	}

	void Renderer::Shutdown()
	{ 
		VE_PROFILE_FUNCTION();

		delete[] s_data.QuadVertexBufferBase;
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		VE_PROFILE_FUNCTION();

		s_data.ViewProjection = camera.GetViewProjectionMatrix();

		StartBatch();
	}

	void Renderer::EndScene()
	{
		VE_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec4& color)
	{
		VE_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, color);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec4& color)
	{
		VE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawQuad(transform, color);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		VE_PROFILE_FUNCTION();

		// If batch is full (max indices)
		if (s_data.QuadIndexCount >= RendererData::MaxIndices)
			NextBatch();

		const int quadVertexCount{ 4 };

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[i];
			s_data.QuadVertexBufferPtr->Color = color;
			s_data.QuadVertexBufferPtr->TexCoord = s_data.QuadVertexTextureCoords[i];
			s_data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_data.QuadVertexBufferPtr++;
		}

		s_data.QuadIndexCount += 6;
		s_data.Stats.AddQuad();
	}

	void Renderer::DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		VE_PROFILE_FUNCTION();

		DrawQuad({ position.x, position.y, 0.0f }, texture, tintColor);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		VE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawQuad(transform, texture, tintColor);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		VE_PROFILE_FUNCTION();

		// If batch is full (max indices)
		if (s_data.QuadIndexCount >= RendererData::MaxIndices)
			NextBatch();

		const int quadVertexCount{ 4 };

		// Check if texture is loaded into the GPU and set texture index to it if so
		float textureIndex{ 0.0f };
		for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
			if (*s_data.TextureSlots[i] == *texture)
			{
				textureIndex = (float) i;
				break;
			}

		// If texture is not loaded into the GPU, set texture index to next index
		if (textureIndex == 0.0f)
		{
			// If batch is full (max textures)
			if (s_data.TextureSlotIndex >= RendererData::MaxTextureSlots)
				NextBatch();

			// Set texture index
			textureIndex = (float) s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[i];
			s_data.QuadVertexBufferPtr->Color = tintColor;
			s_data.QuadVertexBufferPtr->TexCoord = s_data.QuadVertexTextureCoords[i];
			s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_data.QuadVertexBufferPtr++;
		}

		s_data.QuadIndexCount += 6;
		s_data.Stats.AddQuad();
	}

	void Renderer::DrawEntity(const glm::vec2& position, const SpriteComponent& sc)
	{
		VE_PROFILE_FUNCTION();

		DrawEntity({ position.x, position.y, 0.0f }, sc);
	}

	void Renderer::DrawEntity(const glm::vec3& position, const SpriteComponent& sc)
	{
		VE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawEntity(transform, sc);
	}

	void Renderer::DrawEntity(const glm::mat4& transform, const SpriteComponent& sc)
	{
		VE_PROFILE_FUNCTION();

		if (sc.TextureHandle)
		{
			// Draw cube with texture
			Ref<Texture> texture = AssetManager::GetAsset<Texture>(sc.TextureHandle);
			DrawQuad(transform, texture);
		} else 
		{
			// Draw colored cube (no texture)
			DrawQuad(transform, sc.Color);
		}
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
		VE_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount)
		{
			// Calculate data size
			uint32_t dataSize = (uint32_t) ((uint8_t*) s_data.QuadVertexBufferPtr - (uint8_t*) s_data.QuadVertexBufferBase);
			s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
				s_data.TextureSlots[i]->Bind(i);

			// Create draw call
			s_data.QuadShader->Bind();
			s_data.QuadShader->SetMat4("uViewProjection", s_data.ViewProjection); // TODO: Move to begin scene
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

		s_data.TextureSlotIndex = 1;
	}
}
