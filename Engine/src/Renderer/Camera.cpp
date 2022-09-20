// VoxelEngine - Engine
// Camera.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 20-09-2022 15:43
// Last update: 20-09-2022 18:56

#include "pch.h"
#include "Camera.h"

#include "Core/Application.h"

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

	void Camera::OnUpdate(float ts)
	{
		float velocity{ m_movementSpeed * ts };

		// Input

		UpdateCameraVectors();
	}

	void Camera::OnEvent(Event& e) {}

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
	}
}
