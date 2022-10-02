#pragma once

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace VoxelEngine
{
	// Camera logic used is from OpenGL Camera tutorial
	class Camera
	{
	public:
		Camera(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3& up = { 0.0f, 1.0f, 0.0f }, float yaw = -90.0f, float pitch = 0.0f);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		void Init();
		void OnUpdate(float ts);
		void OnEvent(Event& e);

		glm::mat4 GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

		glm::vec3 GetPosition() const { return m_position; }
		void SetPosition(const glm::vec3& position);

		float GetYaw() const { return m_yaw; }
		void SetYaw(float yaw);

		float GetPitch() const { return m_pitch; }
		void SetPitch(float pitch);

	private:
		void UpdateCameraVectors();

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		glm::vec3 m_position;
		glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		float m_movementSpeed{ 5.0f };
		float m_mouseSensitivity{ 0.1f };
		float m_zoom{ 45.0f };

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		float m_lastX, m_lastY;
		bool m_firstMouse{ true };
	};
}