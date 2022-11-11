#pragma once

#include <Engine.h>
#include "Panels/DebugPanel.h"
#include "Panels/PanelManager.h"
#include "Panels/SceneHierarchyPanel.h"

namespace VoxelEngine {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer()
			: Layer("EditorLayer")
		{}
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
		void OpenScene(const std::filesystem::path& path);
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
		Camera* m_camera;

		// Scene
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
		glm::vec2 m_viewportSize{ 0.0f, 0.0f };

		// Framebuffer
		Ref<Framebuffer> m_framebuffer;
	};
}
