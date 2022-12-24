#pragma once

#include "EpEngine/Core/KeyCodes.h"
#include "EpEngine/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace EpEngine
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}