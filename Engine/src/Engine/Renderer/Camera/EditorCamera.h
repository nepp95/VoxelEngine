#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Renderer/Camera/Camera.h"

namespace VoxelEngine
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float ts);
		void OnEvent(Event& e);

		void SetViewportSize(float width, float height);

		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }

	private:
		// Events
		bool OnMouseScroll(MouseScrolledEvent& e);

		// Update matrices
		void UpdateCameraView();

		// Camera movement
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		// Directions
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		// Orientation
		glm::quat GetOrientation() const;

		// Position
		glm::vec3 CalculatePosition() const;

	private:
		glm::mat4 m_viewMatrix;
		glm::vec3 m_position{ 0.0f };
		glm::vec3 m_focalPoint{ 0.0f };

		float m_fov{ 45.0f };
		float m_aspectRatio{ 1.778f }; // 1920x1080
		float m_nearClip{ 0.1f };
		float m_farClip{ 1000.0f };
		float m_distance{ 10.0f };

		float m_pitch{ 0.0f };
		float m_yaw{ 0.0f };

		float m_rotationSpeed{ 0.8f };

		glm::vec2 m_initialMousePosition{ 0.0f, 0.0f };

		uint32_t m_viewportWidth{ 1280 };
		uint32_t m_viewportHeight{ 720 };
	};
}