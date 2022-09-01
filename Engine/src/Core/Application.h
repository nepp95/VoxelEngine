// VoxelEngine - Engine
// Application.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 25-08-2022 15:21
// Last update: 31-08-2022 21:43

#pragma once

#include <string>

#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"

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

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Window& GetWindow() { return *m_window; }

		const ApplicationSpecification& GetSpecification() const { return m_specification; }
		static Application& Get() { return *s_instance; }

	private:
		ApplicationSpecification m_specification;
		LayerStack m_layerStack;

		Scope<Window> m_window;

		bool m_isRunning{true};
		bool m_isMinimized{false};
		float m_lastFrameTime{0.0f};

		static Application* s_instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
