#pragma once

#include <Engine.h>
#include "Panels/Panel.h"

namespace VoxelEngine
{
	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel() = default;

		void RenderGui() override;
		void SetSceneContext(const Ref<Scene>& scene) override;

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