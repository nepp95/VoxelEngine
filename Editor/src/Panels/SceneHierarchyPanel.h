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
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_context;
		Entity m_selectedEntity;
	};
}