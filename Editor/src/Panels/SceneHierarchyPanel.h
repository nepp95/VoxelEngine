#pragma once

#include <Core/Base.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>

namespace VoxelEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;

		void SetContext(const Ref<Scene>& context);

		void RenderGui();

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawProperties(Entity entity);

	private:
		Ref<Scene> m_context;
		Entity m_selectedEntity;
	};
}