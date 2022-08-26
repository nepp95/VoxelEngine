#include <Engine.h>
#include <Core/Entrypoint.h>

namespace VoxelEngine
{
	class Editor : public Application
	{
	public:
		Editor(const ApplicationSpecification& specification)
			: Application{ specification }
		{}

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