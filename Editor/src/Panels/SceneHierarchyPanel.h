#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Scene/Entity.h>
#include <Engine/Scene/Scene.h>

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

		template<typename T, typename UIFunction>
		void DrawProperty(const std::string& name, Entity entity, UIFunction uiFunction);

	private:
		Ref<Scene> m_context;
		Entity m_selectedEntity;
	};
}