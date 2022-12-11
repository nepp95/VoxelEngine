#include "pch.h"
#include "EditorCamera.h"

#include "Engine/Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace VoxelEngine
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)), m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
	{
		UpdateCameraView();
	}

	void EditorCamera::OnUpdate(float ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateCameraView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		
		// Update projection
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateCameraView();

		return false;
	}

	void EditorCamera::UpdateCameraView()
	{
		m_position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		float xSpeed, ySpeed;
		float x, y;

		x = std::min(m_viewportWidth / 1000.0f, 2.4f);
		xSpeed = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		y = std::min(m_viewportHeight / 1000.0f, 2.4f);
		ySpeed = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		m_focalPoint += GetRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * m_rotationSpeed;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		float distance = m_distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		m_distance -= delta * speed;

		if (m_distance < 1.0f)
		{
			m_focalPoint += GetForwardDirection();
			m_distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3( 0.0f, 1.0f, 0.0f ));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_focalPoint - GetForwardDirection() * m_distance;
	}
}