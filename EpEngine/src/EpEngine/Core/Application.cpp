#include "pch.h"
#include "Application.h"

#include "EpEngine/Renderer/Renderer.h"
#include "EpEngine/Scripting/ScriptEngine.h"
#include "EpEngine/Utility/Filesystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EpEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_specification{specification}
	{
		// Set instance if not set. If it is, abort. We can only have one instance!
		EP_CORE_ASSERT(!s_instance, "Application instance already exists!");
		s_instance = this;

		// Set working directory
		if (!m_specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_specification.WorkingDirectory);

		// Profiler
		m_profiler = new Profiler();

		// Create window
		WindowSpecification spec{m_specification.Name, m_specification.WindowWidth, m_specification.WindowHeight};
		m_window = CreateScope<Window>(spec);
		m_window->SetEventCallback([this](Event& e) { Application::OnEvent(e); });

		// Initialize systems
		Filesystem::Init();
		Renderer::Init();
		ScriptEngine::Init();

		// Add ImGui layer
		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);

		#ifdef WRITE_PROFILES
			// Profiler
			m_profiler->BeginSession("Profile.json");
		#endif
	}

	Application::~Application()
	{
		EP_CORE_INFO("Shutting down...");

		ScriptEngine::Shutdown();
		Renderer::Shutdown();
		Filesystem::Shutdown();

		m_profiler->EndSession();
	}

	void Application::Close()
	{
		m_isRunning = false;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			m_profiler->Clear();

			auto time = static_cast<float>(glfwGetTime());
			float ts = time - m_lastFrameTime;
			m_lastFrameTime = time;
			m_accumulatedTs += ts;

			ExecuteMainThreadQueue();

			if (!m_isMinimized)
			{
				EP_PROFILE_SCOPE_CAT("Application", "Frametime");

				{
					EP_PROFILE_SCOPE_CAT("Application", "Update");

					for (Layer* layer : m_layerStack)
						layer->Update(ts);
				}

				{
					EP_PROFILE_SCOPE_CAT("Application", "Render")

					if (m_window->IsVSync())
					{
						if (m_accumulatedTs > 1.0f / m_targetFps)
						{
							for (Layer* layer : m_layerStack)
								layer->Render();

							m_accumulatedTs = 0.0f;
						}
					} else
					{
						for (Layer* layer : m_layerStack)
							layer->Render();
					}
				}

			}

			// Render gui
			RenderGui();

			// Update window
			m_window->Update();
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

	void Application::RenderGui()
	{
		// TODO: Fix profiler
		EP_PROFILE_SCOPE("Application::RenderImGui");

		m_imGuiLayer->Begin();

		for (Layer* layer : m_layerStack)
			layer->RenderGui();

		m_imGuiLayer->End();
	}

	void Application::SubmitToMainThread(const std::function<void()>& func)
	{
		std::scoped_lock<std::mutex> lock(m_mainThreadQueueMutex);

		m_mainThreadQueue.emplace_back(func);
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

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_mainThreadQueueMutex);

		for (auto& func : m_mainThreadQueue)
			func();

		m_mainThreadQueue.clear();
	}
}
