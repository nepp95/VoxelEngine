// VoxelEngine - Engine
// Camera.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 20-09-2022 15:43
// Last update: 20-09-2022 15:49

#pragma once

#include "Events/Event.h"

namespace VoxelEngine
{
	class Camera
	{
	public:
		Camera(const Vec3& position = { 0.0f, 0.0f, 0.0f }, const Vec3& up = { 0.0f, 1.0f, 0.0f }, float yaw = -90.0f, float pitch = 0.0f);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		void Init();
		void OnUpdate(float ts);
		void OnEvent(Event& e);

		// View matrix
		// View projection matrix

		Vec3 GetPosition() const { return m_position; }
		void SetPosition(const Vec3& position);

		float GetYaw() const { return m_yaw; }
		void SetYaw(float yaw);

		float GetPitch() const { return m_pitch; }
		void SetPitch(float pitch);

	private:
		void UpdateCameraVectors();

		//bool OnMouseMoved(MouseMovedEvent& e);
		//bool OnKeyPressed(KeyPressedEvent& e);

	private:
		Vec3 m_position;
		Vec3 m_front;
		Vec3 m_up;
		Vec3 m_right;
		Vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		float m_movementSpeed;
		float m_mouseSensitivity;
		float m_zoom;

		// viewMatrix
		// projectionMatrix

		float m_lastX, m_lastY;
		bool m_firstMouse{ true };
	};
}