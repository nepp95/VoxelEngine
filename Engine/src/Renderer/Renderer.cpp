// VoxelEngine - Engine
// Renderer.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 03-09-2022 17:26
// Last update: 20-09-2022 19:09

#include "pch.h"
#include "Renderer.h"

#include "Renderer/RenderCommand.h"

namespace VoxelEngine
{
	struct RendererData
	{
		// Batch limits
		const uint32_t MaxVertices{ 50000 };
		const uint32_t MaxIndices{ 50000 };

		// Shader
		//Ref<Shader> BaseShader;

		// Camera
		glm::mat4 ViewProjection;

		// Stats
		Renderer::Statistics Stats;
	};

	static RendererData s_data;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown() { }

	void Renderer::BeginScene(const Camera& camera)
	{
		s_data.ViewProjection = camera.GetViewProjectionMatrix();

		StartBatch();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::DrawTriangle(const glm::vec3& position, const glm::vec4& color)
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

	void Renderer::Flush() { }

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer::StartBatch() { }
}
