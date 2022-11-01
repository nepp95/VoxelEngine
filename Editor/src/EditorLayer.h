#pragma once

#include <Engine.h>
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace VoxelEngine {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::string& name = std::string())
			: Layer(name.empty() ? name : "EditorLayer")
		{}
		EditorLayer(const EditorLayer& other) = delete;

		~EditorLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;

		void Update(float ts) override;
		void Render() override;
		void ImGuiRender() override;

		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		// UI
		void UIToolbar();

	private:
		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;

		// Timing
		float m_timestep{ 0.0f };

		// Camera
		Camera* m_camera;

		// Scene
		Ref<Scene> m_scene;

		// Viewport
		bool m_viewportFocused{false}, m_viewportHovered{false};
		glm::vec2 m_viewportBounds[2];
		glm::vec2 m_viewportSize{ 0.0f, 0.0f };

		// Framebuffer
		Ref<Framebuffer> m_framebuffer;

		// Settings
		bool m_enableVSync;
	};
}
