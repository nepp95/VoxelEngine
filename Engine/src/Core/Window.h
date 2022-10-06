#pragma once

#include <functional>

#include "Events/Event.h"

// Forward declare
struct GLFWwindow;

namespace VoxelEngine
{
	struct WindowSpecification
	{
		std::string Title;

		uint32_t Width;
		uint32_t Height;

		bool VSync;

		WindowSpecification(const std::string& title = "Voxel Engine", uint32_t width = 1280, uint32_t height = 720,
		                    bool vsync = true)
			: Title(title), Width(width), Height(height), VSync(vsync) { }
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowSpecification& specification);
		~Window();

		void Shutdown();

		void OnUpdate();

		uint32_t GetWidth() const { return m_specification.Width; }
		uint32_t GetHeight() const { return m_specification.Height; }

		void SetVSync(bool enabled);
		bool IsVSync() const { return m_specification.VSync; }

		void SetEventCallback(const EventCallbackFn& callback) { m_callback = callback; }

		GLFWwindow& GetNativeWindow() const { return *m_window; }

	private:
		WindowSpecification m_specification;
		GLFWwindow* m_window;

		EventCallbackFn m_callback;
	};
}
