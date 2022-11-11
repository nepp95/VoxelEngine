#include "pch.h"
#include "Input.h"

#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace VoxelEngine
{
	bool Input::IsKeyPressed(KeyCode key)
	{
		auto* window = &Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetKey(window, key);

		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto* window = &Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto* window = &Application::Get().GetWindow().GetNativeWindow();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}