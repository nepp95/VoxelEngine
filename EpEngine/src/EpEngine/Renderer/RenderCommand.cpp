#include "pch.h"
#include "RenderCommand.h"

#include "EpEngine/Core/Application.h"

#include <glad/glad.h>

namespace EpEngine
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
			{
				EP_CORE_ERROR(message);
				return;
			}

			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				EP_CORE_ERROR(message);
				return;
			}

			case GL_DEBUG_SEVERITY_LOW:
			{
				EP_CORE_WARN(message);
				return;
			}

			case GL_DEBUG_SEVERITY_NOTIFICATION:
			{
				EP_CORE_INFO(message);
				return;
			}
		}
	}

	void RenderCommand::Init()
	{
		EP_PROFILE_FUNCTION();

		#ifdef V_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false);
		#endif

		// Blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// Depth testing
		glEnable(GL_DEPTH_TEST);
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		EP_PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		EP_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear()
	{
		EP_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		EP_PROFILE_FUNCTION();

		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}
