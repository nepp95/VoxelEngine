#include <EpEngine.h>
#include <EpEngine/Core/Entrypoint.h>

#include "SandboxLayer.h"

using namespace EpEngine;

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

Application* EpEngine::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}