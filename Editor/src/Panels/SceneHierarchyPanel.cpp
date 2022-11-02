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

		// Add each entity in our registry to the list
		m_context->m_registry.each([&] (auto entityId)
		{
			// TODO: Copy entity necessary?
			Entity entity{ entityId, m_context.Raw() };
			DrawEntityNode(entity);
		});

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create new entity"))
				m_context->CreateEntity("Empty entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		// Properties, since scene hierarchy and properties are so intwined, we put them in one class.
		ImGui::Begin("Properties");

		if (m_selectedEntity)
			DrawProperties(m_selectedEntity);

		ImGui::End();
	}

	template <typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_selectedEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_selectedEntity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
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

	void SceneHierarchyPanel::DrawProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>();

			// ImGui wants a char*, we use std::string
			// We could cast a c_str to char*, but we still could not write to it
			char buffer[256]{ 0 };
			// Since ImGui accounts for the buffer size AND the null termination char, we can safely use strncpy.
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			// Double quote prevents the label from showing
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("+"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteComponent>("Sprite renderer");

			ImGui::EndPopup(); // AddComponent
		}

		ImGui::PopItemWidth(); // -1
	}
}
