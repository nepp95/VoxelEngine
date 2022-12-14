#pragma once

#include <glm/glm.hpp>

namespace VoxelEngine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera::Camera(const glm::mat4& projection)
			: m_projection(projection)
		{}
		
		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_projection; }

	protected:
		glm::mat4 m_projection{ glm::mat4(1.0f) };
	};
}