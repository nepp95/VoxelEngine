// VoxelEngine - Engine
// Application.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 25-08-2022 15:21
// Last update: 31-08-2022 21:43

#include "pch.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VoxelEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_specification{specification}
	{
		// Set instance if not set. If it is, abort. We can only have one instance!
		// Todo: assert s_instance
		CORE_ASSERT(!s_instance, "Application instance already exists!");
		s_instance = this;

		// Set working directory
		if (!m_specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_specification.WorkingDirectory);

		// Create window
		WindowSpecification spec{m_specification.Name, m_specification.WindowWidth, m_specification.WindowHeight};
		m_window = CreateScope<Window>(spec);
		m_window->SetEventCallback([this](Event& e) { Application::OnEvent(e); });
	}

	Application::~Application() { }

	void Application::Close()
	{
		m_isRunning = false;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			auto time = static_cast<float>(glfwGetTime());
			float ts = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_isMinimized)
				for (Layer* layer : m_layerStack)
					layer->OnUpdate(ts);

			// Render gui

			// Update window
			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		// Check if window is minimized
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_isMinimized = true;
			return false;
		}

		m_isMinimized = false;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return true;
	}
}
