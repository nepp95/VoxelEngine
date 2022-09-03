// VoxelEngine - Engine
// RenderCommand.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 22:04
// Last update: 03-09-2022 17:18

#include "pch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace VoxelEngine
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
				CORE_ERROR(message);
				return;
			}

			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				CORE_ERROR(message);
				return;
			}

			case GL_DEBUG_SEVERITY_LOW:
			{
				CORE_WARN(message);
				return;
			}

			case GL_DEBUG_SEVERITY_NOTIFICATION:
			{
				CORE_INFO(message);
				return;
			}
		}
	}

	void RenderCommand::Init()
	{
		#ifdef V_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false);
		#endif
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const Vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
