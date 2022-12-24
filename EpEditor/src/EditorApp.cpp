#include <EpEngine.h>
#include <EpEngine/Core/Entrypoint.h>

#include "EditorLayer.h"

namespace EpEngine
{
	class Editor : public Application
	{
	public:
		Editor(const ApplicationSpecification& specification)
			: Application{specification}
		{
			PushLayer(new EditorLayer());
		}

		~Editor() = default;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Voxel Engine Editor";
		spec.CommandLineArgs = args;

		return new Editor(spec);
	}
}
