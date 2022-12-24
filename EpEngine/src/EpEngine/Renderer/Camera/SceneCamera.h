#pragma once

#include "EpEngine/Renderer/Camera/Camera.h"

namespace EpEngine
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		// Set camera parameters
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		void SetViewportSize(float width, float height);

		float GetPerspectiveFov() const { return m_perspectiveFov; }
		float GetPerspectiveNearClip() const { return m_perspectiveNearClip; }
		float GetPerspectiveFarClip() const { return m_perspectiveFarClip; }
		void SetPerspectiveFov(float fov);
		void SetPerspectiveNearClip(float nearClip);
		void SetPerspectiveFarClip(float farClip);

		float GetOrthographicSize() const { return m_orthographicSize; }
		float GetOrthographicNearClip() const { return m_orthographicNearClip; }
		float GetOrthographicFarClip() const { return m_orthographicFarClip; }
		void SetOrthographicSize(float size);
		void SetOrthographicNearClip(float nearClip);
		void SetOrthographicFarClip(float farClip);

		// Projection type
		// Prefer usage of SetPerspective / SetOrthographic. If you don't, old params that were still in the variables will be used.
		ProjectionType GetProjectionType() const { return m_projectionType; }
		void SetProjectionType(ProjectionType type);

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_projectionType{ ProjectionType::Orthographic };

		float m_perspectiveFov{ glm::radians(45.0f) };
		float m_perspectiveNearClip{ 0.01f };
		float m_perspectiveFarClip{ 1000.0f };
		
		float m_orthographicSize{ 10.0f };
		float m_orthographicNearClip{ -1.0f };
		float m_orthographicFarClip{ 1.0f };

		float m_aspectRatio{ 0.0f };
	};
}
