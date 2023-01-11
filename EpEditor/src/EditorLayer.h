#pragma once

#include <EpEngine.h>
#include "Panels/DebugPanel.h"
#include "Panels/PanelManager.h"
#include "Panels/SceneHierarchyPanel.h"

namespace EpEngine {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		EditorLayer(const EditorLayer& other) = delete;

		~EditorLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;

		void Update(float ts) override;
		void Render() override;
		void RenderGui() override;

		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		// Scene management
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& filepath);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		// UI
		void UIToolbar();

	private:
		// Panels
		Scope<PanelManager> m_panelManager;

		// Timing
		float m_timestep{ 0.0f };

		// Camera
		EditorCamera m_editorCamera{ 30.0f, 1.778f, 0.1f, 1000.0f };

		// Scene
		std::filesystem::path m_scenePath;
		Ref<Scene> m_activeScene; // runtime
		Ref<Scene> m_editorScene; // editor

		enum class SceneState
		{
			Edit = 0, Play
		};
		SceneState m_sceneState{ SceneState::Edit };

		// Viewport
		bool m_viewportFocused{false}, m_viewportHovered{false};
		glm::vec2 m_viewportBounds[2];
		glm::vec2 m_viewportSize{ 1280.0f, 720.0f };

		// Framebuffer
		Ref<Framebuffer> m_framebuffer;

		// Resources
		Ref<Texture> m_iconPlay;
		Ref<Texture> m_iconStop;
	};
}
