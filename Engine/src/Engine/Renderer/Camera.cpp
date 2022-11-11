#include "pch.h"
#include "Camera.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Culling/Frustrum.h"

#include <glm/ext/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace VoxelEngine
{
	Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
		: m_position{ position }, m_worldUp{ up }, m_yaw{ yaw }, m_pitch{ pitch }
	{}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: m_position{ posX, posY, posZ }, m_worldUp{ upX, upY, upZ }, m_yaw{ yaw }, m_pitch{ pitch }
	{}

	void Camera::Init()
	{
		auto& app = Application::Get();

		m_lastX = app.GetWindow().GetWidth() / 2.0f;
		m_lastY = app.GetWindow().GetHeight() / 2.0f;

		UpdateCameraVectors();
	}

	void Camera::Update(float ts)
	{
		float velocity{ m_movementSpeed * ts };

		// Input
		// Forward/Backward
		if (Input::IsKeyPressed(Key::W))
			m_position += m_front * velocity;
		else if (Input::IsKeyPressed(Key::S))
			m_position -= m_front * velocity;

		// Left/Right
		if (Input::IsKeyPressed(Key::A))
			m_position -= m_right * velocity;
		else if (Input::IsKeyPressed(Key::D))
			m_position += m_right * velocity;

		// Up/Down
		if (Input::IsKeyPressed(Key::Space))
			m_position += m_up * velocity;
		else if (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl))
			m_position -= m_up * velocity;

		UpdateCameraVectors();
	}

	void Camera::OnEvent(Event& e)
	{
		//EventDispatcher dispatcher(e);
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_position = position;

		UpdateCameraVectors();
	}

	void Camera::SetYaw(float yaw)
	{
		m_yaw = yaw;

		UpdateCameraVectors();
	}

	void Camera::SetPitch(float pitch)
	{
		m_pitch = pitch;

		UpdateCameraVectors();
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 front{
			cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
			sin(glm::radians(m_pitch)),
			sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
		};

		m_front = glm::normalize(glm::vec3{ front.x, front.y, front.z });
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));

		auto& app = Application::Get();
		m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
		m_projectionMatrix = glm::perspective(glm::radians(m_zoom), static_cast<float>(app.GetWindow().GetWidth()) / static_cast<float>(app.GetWindow().GetHeight()), 0.1f, 100.0f);

		Frustrum::SetPlanes(GetViewProjectionMatrix());
	}
}
