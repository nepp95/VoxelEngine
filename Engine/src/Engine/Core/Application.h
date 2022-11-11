#pragma once

#include <string>

#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Window.h"
#include "Engine/Debug/Profiler.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Layers/ImGuiLayer.h"

int main(int argc, char** argv);

namespace VoxelEngine
{
	struct ApplicationCommandLineArgs
	{
		int Count{0};
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			// TODO: Assert index < count
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name{"Application"};
		std::string WorkingDirectory;

		uint32_t WindowWidth = 1280;
		uint32_t WindowHeight = 720;

		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		~Application();

		void Close();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void RenderImGui();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		const ApplicationSpecification& GetSpecification() const { return m_specification; }
		static Application& Get() { return *s_instance; }

		ImGuiLayer* GetImGuiLayer() { return m_imGuiLayer; }
		Window& GetWindow() { return *m_window; }
		Profiler* GetProfiler() { return m_profiler; }

	private:
		Scope<Window> m_window;
		ApplicationSpecification m_specification;
		LayerStack m_layerStack;
		ImGuiLayer* m_imGuiLayer;

		bool m_isRunning{ true };
		bool m_isMinimized{ false };
		bool m_needRender{ false };
		float m_lastFrameTime{ 0.0f };
		float m_accumulatedTs{ 0.0f };
		float m_targetFps{ 60.0f };

		Profiler* m_profiler{ nullptr };

		static Application* s_instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
