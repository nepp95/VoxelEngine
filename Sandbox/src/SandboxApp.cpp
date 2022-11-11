#include <Engine.h>
#include <Engine/Core/Entrypoint.h>

#include "SandboxLayer.h"

using namespace VoxelEngine;

class Sandbox : public Application
{
public:
	Sandbox(const ApplicationSpecification& specification)
		: Application(specification)
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox() = default;
};

Application* VoxelEngine::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}