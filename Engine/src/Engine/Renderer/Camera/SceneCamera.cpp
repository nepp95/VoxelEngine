#include "pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace VoxelEngine
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Perspective;

		m_perspectiveFov = fov;
		m_perspectiveNearClip = nearClip;
		m_perspectiveFarClip = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Orthographic;
		
		m_orthographicSize = size;
		m_orthographicNearClip = nearClip;
		m_orthographicFarClip = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		SetViewportSize((float)width, (float)height);
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		VE_CORE_ASSERT(width > 0 && height > 0, "Width/height must be higher than 0!");

		m_aspectRatio = width / height;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFov(float fov)
	{
		m_perspectiveFov = fov;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		m_perspectiveNearClip = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		m_perspectiveFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		m_orthographicSize = size;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicNearClip(float nearClip)
	{
		m_orthographicNearClip = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicFarClip(float farClip)
	{
		m_orthographicFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_projectionType = type;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			m_projectionMatrix = glm::perspective(m_perspectiveFov, m_aspectRatio, m_perspectiveNearClip, m_perspectiveFarClip);
		} else if (m_projectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoBottom = -m_orthographicSize * 0.5f;
			float orthoTop = m_orthographicSize * 0.5f;

			m_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNearClip, m_orthographicFarClip);
		}
	}
}