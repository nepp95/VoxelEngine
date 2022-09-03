// VoxelEngine - Editor
// EditorApp.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 25-08-2022 15:21
// Last update: 31-08-2022 21:47

#include <Engine.h>
#include <Core/Entrypoint.h>

#include "EditorLayer.h"

namespace VoxelEngine
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
