#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace VoxelEngine
{
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_context = context;
	}

	void SceneHierarchyPanel::RenderGui()
	{
		ImGui::Begin("Scene Hierarchy");

		// For each entity in our registry
		m_context->m_registry.each([&] (auto entityId)
		{
			// TODO: Copy entity necessary?
			Entity entity{ entityId, m_context.Raw() };
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = (m_selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow ;

		if (ImGui::TreeNodeEx(tag.c_str(), flags))
		{
			ImGui::TreePop();
		}

		if (ImGui::IsItemClicked())
			m_selectedEntity = entity;

		bool entityDeleted{ false };
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (m_selectedEntity == entity)
				m_selectedEntity = {};

			m_context->DestroyEntity(entity);
		}
	}
}
