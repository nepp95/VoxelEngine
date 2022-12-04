#include "SceneHierarchyPanel.h"

#include "Controls.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace VoxelEngine
{
	void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& context)
	{
		m_selectedEntity = {};
		m_context = context;
	}

	void SceneHierarchyPanel::RenderGui()
	{
		ImGui::Begin("Scene Hierarchy");

		// Add each entity in our registry to the list
		m_context->m_registry.each([&] (auto entityId)
		{
			// TODO: Copy entity necessary?
			Entity entity{ entityId, m_context.get() };
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

		ImGuiTreeNodeFlags flags = (m_selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

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
		// Name, tag component
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

		// Add component button
		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteComponent>("Sprite renderer");

			ImGui::EndPopup(); // AddComponent
		}

		DrawProperty<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);

			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);

			DrawVec3Control("Scale", component.Scale);
		});

		DrawProperty<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			float pitch = camera.GetPitch();
			if (ImGui::DragFloat("Pitch", &pitch))
				camera.SetPitch(pitch);

			float yaw = camera.GetYaw();
			if (ImGui::DragFloat("Yaw", &yaw))
				camera.SetYaw(yaw);
		});

		DrawProperty<SpriteComponent>("Sprite", entity, [](auto& component)
		{
			// TODO: Texture changing/removing/adding
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
	}

	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawProperty(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

			ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}
}
