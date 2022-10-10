#include "pch.h"
#include "Renderer.h"

#include "Asset/AssetManager.h"
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
		static const uint32_t MaxQuads{ 30000 };
		static const uint32_t MaxVertices{ MaxQuads * 4 };
		static const uint32_t MaxIndices{ MaxQuads * 6 };
		static const uint32_t MaxTextureSlots{ 32 };

		// Quad Data
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Vertex* QuadVertexBufferBase{ nullptr };
		Vertex* QuadVertexBufferPtr{ nullptr };
		std::vector<glm::vec4> QuadVertexPositions;
		std::vector<glm::vec2> QuadVertexTextureCoords;
		uint32_t QuadIndexCount{ 0 };

		// Shader
		Ref<Shader> BaseShader;

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
		RenderCommand::Init();

		// Create vertex array and buffers
		s_data.QuadVertexArray = Ref<VertexArray>::Create();
		s_data.QuadVertexBuffer = Ref<VertexBuffer>::Create(RendererData::MaxVertices * sizeof(Vertex));

		s_data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,	"aPosition"		},
			{ ShaderDataType::Float4,	"aColor"		},
			{ ShaderDataType::Float2,	"aTexCoord"		},
			{ ShaderDataType::Float,	"aTexIndex"		},
			{ ShaderDataType::Float,	"aTilingFactor" },
		});

		s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);
		s_data.QuadVertexBufferBase = new Vertex[RendererData::MaxVertices];

		// Index buffer
		uint32_t* indices = new uint32_t[RendererData::MaxIndices];
		uint32_t offset{ 0 };

		for (uint32_t i = 0; i < s_data.MaxIndices; i += 36)
		{
			// Front
			indices[i + 0]	= offset + 0;
			indices[i + 1]	= offset + 1;
			indices[i + 2]	= offset + 2;
			indices[i + 3]	= offset + 2;
			indices[i + 4]	= offset + 3;
			indices[i + 5]	= offset + 0;

			// Right
			indices[i + 6]	= offset + 4;
			indices[i + 7]	= offset + 6;
			indices[i + 8]	= offset + 7;
			indices[i + 9]	= offset + 7;
			indices[i + 10] = offset + 5;
			indices[i + 11] = offset + 4;

			// Back
			indices[i + 12] = offset + 9;
			indices[i + 13] = offset + 8;
			indices[i + 14] = offset + 11;
			indices[i + 15] = offset + 11;
			indices[i + 16] = offset + 10;
			indices[i + 17] = offset + 9;

			// Left
			indices[i + 18] = offset + 14;
			indices[i + 19] = offset + 12;
			indices[i + 20] = offset + 13;
			indices[i + 21] = offset + 13;
			indices[i + 22] = offset + 15;
			indices[i + 23] = offset + 14;

			// Bottom
			indices[i + 24] = offset + 18;
			indices[i + 25] = offset + 19;
			indices[i + 26] = offset + 17;
			indices[i + 27] = offset + 17;
			indices[i + 28] = offset + 16;
			indices[i + 29] = offset + 18;

			// Top
			indices[i + 30] = offset + 21;
			indices[i + 31] = offset + 20;
			indices[i + 32] = offset + 22;
			indices[i + 33] = offset + 22;
			indices[i + 34] = offset + 23;
			indices[i + 35] = offset + 21;

			offset += 8;
		}

		Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>::Create(indices, RendererData::MaxIndices);
		s_data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		// Set quad vertex positions
		s_data.QuadVertexPositions = {
			{ -0.5f, -0.5f, 0.0f, 1.0f }, // FRONT
			{  0.5f, -0.5f, 0.0f, 1.0f }, // FRONT
			{  0.5f,  0.5f, 0.0f, 1.0f }, // FRONT
			{ -0.5f,  0.5f, 0.0f, 1.0f }, // FRONT

			{  0.5f, -0.5f, 0.0f, 1.0f }, // RIGHT
			{  0.5f,  0.5f, 0.0f, 1.0f }, // RIGHT
			{  0.5f, -0.5f, 1.0f, 1.0f }, // RIGHT
			{  0.5f,  0.5f, 1.0f, 1.0f }, // RIGHT

			{ -0.5f, -0.5f, 1.0f, 1.0f }, // BACK
			{  0.5f, -0.5f, 1.0f, 1.0f }, // BACK
			{  0.5f,  0.5f, 1.0f, 1.0f }, // BACK
			{ -0.5f,  0.5f, 1.0f, 1.0f }, // BACK

			{ -0.5f, -0.5f, 0.0f, 1.0f }, // LEFT
			{ -0.5f,  0.5f, 0.0f, 1.0f }, // LEFT
			{ -0.5f, -0.5f, 1.0f, 1.0f }, // LEFT
			{ -0.5f,  0.5f, 1.0f, 1.0f }, // LEFT

			{ -0.5f, -0.5f, 0.0f, 1.0f }, // BOTTOM
			{  0.5f, -0.5f, 0.0f, 1.0f }, // BOTTOM
			{ -0.5f, -0.5f, 1.0f, 1.0f }, // BOTTOM
			{  0.5f, -0.5f, 1.0f, 1.0f }, // BOTTOM

			{  0.5f,  0.5f, 0.0f, 1.0f }, // TOP
			{ -0.5f,  0.5f, 0.0f, 1.0f }, // TOP
			{  0.5f,  0.5f, 1.0f, 1.0f }, // TOP
			{ -0.5f,  0.5f, 1.0f, 1.0f }, // TOP
		};

		// Set quad vertex texture coordinates
		s_data.QuadVertexTextureCoords = {
			{ 0.0f, 0.0f }, // FRONT
			{ 1.0f, 0.0f }, // FRONT
			{ 1.0f, 1.0f }, // FRONT
			{ 0.0f, 1.0f }, // FRONT

			{ 0.0f, 0.0f }, // RIGHT
			{ 0.0f, 1.0f }, // RIGHT
			{ 1.0f, 0.0f }, // RIGHT
			{ 1.0f, 1.0f }, // RIGHT

			{ 1.0f, 0.0f }, // BACK
			{ 0.0f, 0.0f }, // BACK
			{ 0.0f, 1.0f }, // BACK
			{ 1.0f, 1.0f }, // BACK

			{ 1.0f, 0.0f }, // LEFT
			{ 1.0f, 1.0f }, // LEFT
			{ 0.0f, 0.0f }, // LEFT
			{ 0.0f, 1.0f }, // LEFT

			{ 0.0f, 1.0f }, // BOTTOM
			{ 1.0f, 1.0f }, // BOTTOM
			{ 0.0f, 0.0f }, // BOTTOM
			{ 1.0f, 0.0f }, // BOTTOM

			{ 1.0f, 0.0f }, // TOP
			{ 0.0f, 0.0f }, // TOP
			{ 1.0f, 1.0f }, // TOP
			{ 0.0f, 1.0f }, // TOP
		};

		// Load shader
		s_data.BaseShader = Ref<Shader>::Create("assets/shaders/quad.glsl");

		// Set white texture slot (invalid texture)
		s_data.WhiteTexture = Ref<Texture>::Create(1, 1);
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

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec4& color, QuadSide side)
	{
		DrawQuad({ position.x, position.y, 0.0f }, color, side);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec4& color, QuadSide side)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawQuad(transform, color, side);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, QuadSide side)
	{
		if (s_data.QuadIndexCount >= RendererData::MaxIndices)
			NextBatch();

		const int quadVertexCount{ 4 };

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			auto vertexPosition = s_data.QuadVertexPositions[i + ((int) side * 4)];
			auto vertexTextureCoord = s_data.QuadVertexTextureCoords[i + ((int) side * 4)];

			s_data.QuadVertexBufferPtr->Position = transform * vertexPosition;
			s_data.QuadVertexBufferPtr->Color = color;
			s_data.QuadVertexBufferPtr->TexCoord = vertexTextureCoord;
			s_data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_data.QuadVertexBufferPtr++;
		}

		s_data.QuadIndexCount += 6;
		s_data.Stats.AddQuad();
	}

	void Renderer::DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, QuadSide side)
	{
		DrawQuad({ position.x, position.y, 0.0f }, texture, tilingFactor, tintColor, side);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, QuadSide side)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawQuad(transform, texture, tilingFactor, tintColor, side);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, QuadSide side)
	{
		// If batch is full (max indices)
		if (s_data.QuadIndexCount >= RendererData::MaxIndices)
			NextBatch();

		const int quadVertexCount{ 4 };

		// Check if texture is loaded into the GPU and set texture index to it if so
		float textureIndex{ 0.0f };
		for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
			if (*s_data.TextureSlots[i] == *texture.Raw())
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
			auto vertexPosition = s_data.QuadVertexPositions[i + ((int)side * 4)];
			auto vertexTextureCoord = s_data.QuadVertexTextureCoords[i + ((int)side * 4)];

			s_data.QuadVertexBufferPtr->Position = transform * vertexPosition;
			s_data.QuadVertexBufferPtr->Color = tintColor;
			s_data.QuadVertexBufferPtr->TexCoord = vertexTextureCoord;
			s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_data.QuadVertexBufferPtr++;
		}

		s_data.QuadIndexCount += 6;
		s_data.Stats.AddQuad();
	}

	void Renderer::DrawSprite(const glm::mat4& transform, SpriteComponent& sc)
	{
		if (sc.Texture)
			DrawCube(transform, { sc.Texture });
		else
			DrawQuad(transform, sc.Color);
	}

	void Renderer::DrawCube(const glm::vec2& position, const glm::vec4& color)
	{
		DrawCube({ position.x, position.y, 0.0f }, color);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawCube(transform, color);
	}

	void Renderer::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		DrawQuad(transform, color, QuadSide::Front);
		DrawQuad(transform, color, QuadSide::Right);
		DrawQuad(transform, color, QuadSide::Back);
		DrawQuad(transform, color, QuadSide::Left);
		DrawQuad(transform, color, QuadSide::Bottom);
		DrawQuad(transform, color, QuadSide::Top);

		s_data.Stats.CubeCount++;
	}

	void Renderer::DrawCube(const glm::vec2& position, const std::vector<Ref<Texture>>& textures, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawCube({ position.x, position.y, 0.0f }, textures, tilingFactor, tintColor);
	}

	void Renderer::DrawCube(const glm::vec3& position, const std::vector<Ref<Texture>>& textures, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		DrawCube(transform, textures, tilingFactor, tintColor);
	}

	void Renderer::DrawCube(const glm::mat4& transform, const std::vector<Ref<Texture>>& textures, float tilingFactor, const glm::vec4& tintColor)
	{
		if (textures.size() == 1)
			for (int i = 0; i < 6; i++)
				DrawQuad(transform, textures.at(0), tilingFactor, tintColor, (QuadSide)i);
		else if (textures.size() == 6)
			for (int i = 0; i < 6; i++)
				DrawQuad(transform, textures.at(i), tilingFactor, tintColor, (QuadSide)i);
	}

	void Renderer::RenderEntity(const glm::vec2& position, EntityHandle entity)
	{
		RenderEntity({ position.x, position.y, 0.0f }, entity);
	}

	void Renderer::RenderEntity(const glm::vec3& position, EntityHandle entity)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		RenderEntity(transform, entity);
	}

	void Renderer::RenderEntity(const glm::mat4& transform, EntityHandle entity)
	{

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

			// Bind textures
			for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
				s_data.TextureSlots[i]->Bind(i);

			// Create draw call
			s_data.BaseShader->Bind();
			s_data.BaseShader->SetMat4("uViewProjection", s_data.ViewProjection); // TODO: Move to begin scene
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
