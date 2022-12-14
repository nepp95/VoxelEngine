#include "EditorLayer.h"

#include <imgui/imgui.h>

namespace VoxelEngine
{
	#define DEBUG_PANEL "DebugPanel"
	#define SCENE_HIERARCHY_PANEL "SceneHierarchyPanel"

	void EditorLayer::OnAttach()
	{
		VE_PROFILE_FUNCTION();

		m_editorScene = CreateRef<Scene>();
		m_activeScene = m_editorScene;

		//auto entity = m_editorScene->CreateEntity();
		//auto& sc = entity.AddComponent<SpriteComponent>();
		//sc.TextureHandle = AssetManager::GetMetadata("assets/textures/grass.png").Handle;

		// Camera
		//auto cameraEntity = m_editorScene->CreateEntity("Camera");
		//auto& cc = cameraEntity.AddComponent<CameraComponent>();
		//cc.Camera.SetPitch(0.0f);
		//cc.Camera.SetYaw(-90.0f);
		//cc.Camera.SetPosition({ 0.0f, 0.0f, 2.0f });
		//m_camera = &cc.Camera;

		// Create framebuffer
		FramebufferSpecification fbSpecification;
		fbSpecification.Width = 1280;
		fbSpecification.Height = 720;
		fbSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		m_framebuffer = CreateRef<Framebuffer>(fbSpecification);

		// Panels
		m_panelManager = CreateScope<PanelManager>();
		m_panelManager->AddPanel<DebugPanel>(DEBUG_PANEL, true);
		m_panelManager->AddPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL, true);

		m_panelManager->SetSceneContext(m_editorScene);

		OpenScene();
	}

	void EditorLayer::OnDetach()
	{
		VE_PROFILE_FUNCTION();
	}

	void EditorLayer::Update(float ts)
	{
		VE_PROFILE_FUNCTION();

		// Reset render stats
		Renderer::ResetStats();
		m_timestep = ts;

		// Handle resize
		bool resizeEvent{ false };
		if (FramebufferSpecification specification = m_framebuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(specification.Width != m_viewportSize.x || specification.Height != m_viewportSize.y))
			resizeEvent = true;
		else if (glm::vec2 viewportSize = m_editorScene->GetViewportSize();
			m_sceneState == SceneState::Edit &&
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(viewportSize.x != m_viewportSize.x || viewportSize.y != m_viewportSize.y))
			resizeEvent = true;
		else if (glm::vec2 viewportSize = m_activeScene->GetViewportSize();
			m_sceneState == SceneState::Play &&
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(viewportSize.x != m_viewportSize.x || viewportSize.y != m_viewportSize.y))
			resizeEvent = true;

		if (resizeEvent)
		{
			m_framebuffer->Resize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
			m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_editorScene->OnViewportResize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
			m_activeScene->OnViewportResize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
		}

		// Update
		switch (m_sceneState)
		{
			case SceneState::Edit:
			{
				m_editorCamera.OnUpdate(ts);
				break;
			}

			case SceneState::Play:
			{
				m_activeScene->OnUpdateRuntime(ts);
				break;
			}
		}
	}

	void EditorLayer::Render()
	{
		VE_PROFILE_FUNCTION();

		// Prepare for render
		m_framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();

		// Render scene
		switch (m_sceneState)
		{
			case SceneState::Edit:
			{
				m_editorScene->OnRenderEditor(m_editorCamera);
				break;
			}

			case SceneState::Play:
			{
				m_activeScene->OnRenderRuntime();
				break;
			}
		}

		// Unbind framebuffer
		m_framebuffer->Unbind();
	}
	
	void EditorLayer::RenderGui()
	{
		VE_PROFILE_FUNCTION();

		// Check ImGui examples for more information

		static bool dockspaceOpen{ true };
		static bool optFullscreenPersistance{ true };
		bool optFullscreen{ optFullscreenPersistance };
		static ImGuiDockNodeFlags dockspaceFlags{ ImGuiDockNodeFlags_None };

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (optFullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar(); // ImGuiStyleVar_WindowPadding

		if (optFullscreen)
			ImGui::PopStyleVar(2); // ImGuiStyleVar_WindowBorderSize, ImGuiStyleVar_WindowRounding

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		style.WindowMinSize.x = minWinSizeX;

		// Menubar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New scene", "CTRL+N"))
					NewScene();

				if (ImGui::MenuItem("Open scene", "CTRL+O"))
					OpenScene();

				if (ImGui::MenuItem("Save scene", "CTRL+S"))
					SaveScene();

				if (ImGui::MenuItem("Save scene as...", "CTRL+SHIFT+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

				ImGui::EndMenu(); // File
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Start scene"))
					OnScenePlay();
				
				if (ImGui::MenuItem("End scene"))
					OnSceneStop();
				
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_panelManager->RenderGui();

		// -----------------------------------------
		//
		//    Viewport
		//
		// -----------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End(); // Viewport
		ImGui::PopStyleVar(); // ImGuiStyleVar_WindowPadding

		UIToolbar();

		ImGui::End(); // DockSpace Demo
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_sceneState == SceneState::Edit)
			m_editorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}

			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}

			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
	}

	void EditorLayer::OpenScene()
	{
		OpenScene("assets/scenes/test.scene");
	}

	void EditorLayer::OpenScene(const std::filesystem::path& filepath)
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> newScene = CreateRef<Scene>();

		SceneSerializer serializer(newScene);
		
		if (serializer.Deserialize(filepath))
		{
			m_editorScene = newScene;
			m_panelManager->SetSceneContext(m_editorScene);
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();

		SceneSerializer serializer(m_editorScene);
		serializer.Serialize("assets/scenes/test.scene");
	}

	void EditorLayer::SaveSceneAs()
	{
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();

		m_sceneState = SceneState::Play;

		m_activeScene = Scene::Copy(m_editorScene);
		m_activeScene->OnRuntimeStart();

		m_panelManager->SetSceneContext(m_activeScene);
	}

	void EditorLayer::OnSceneStop()
	{
		VE_CORE_ASSERT(m_sceneState == SceneState::Play, "Tried to stop a runtime scene which wasn't running!");
		m_sceneState = SceneState::Edit;

		m_activeScene->OnRuntimeStop();
		m_activeScene = Scene::Copy(m_editorScene);

		m_panelManager->SetSceneContext(m_editorScene);
	}

	void EditorLayer::UIToolbar()
	{

	}
}
