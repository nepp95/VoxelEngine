#include "pch.h"
#include "Application.h"

namespace VoxelEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_specification{ specification }
	{
		// Todo: assert s_instance
		s_instance = this;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
	}
}