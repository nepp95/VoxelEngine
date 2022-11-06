#include "EditorLayer.h"

#include <imgui/imgui.h>

namespace VoxelEngine
{
	void EditorLayer::OnAttach()
	{
		VE_PROFILE_FUNCTION();

		m_scene = Ref<Scene>::Create();

		auto entity = m_scene->CreateEntity();
		auto& sc = entity.AddComponent<SpriteComponent>();
		sc.TextureHandle = AssetManager::GetMetadata("assets/textures/grass.png").Handle;

		// Camera
		auto cameraEntity = m_scene->CreateEntity("Camera");
		auto& cc = cameraEntity.AddComponent<CameraComponent>();
		cc.Camera.SetPitch(0.0f);
		cc.Camera.SetYaw(-90.0f);
		cc.Camera.SetPosition({ 0.0f, 0.0f, 2.0f });
		m_camera = &cc.Camera;

		// Set settings
		auto& app = Application::Get();
		m_enableVSync = app.GetWindow().IsVSync(); // TODO: Make this work?!

		// Create framebuffer
		FramebufferSpecification fbSpecification;
		fbSpecification.Width = 1280;
		fbSpecification.Height = 720;
		fbSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		m_framebuffer = Ref<Framebuffer>::Create(fbSpecification);

		// Panels
		m_sceneHierarchyPanel.SetContext(m_scene);
	}

	void EditorLayer::OnDetach()
	{
		VE_PROFILE_FUNCTION();

		m_camera = nullptr;
	}

	void EditorLayer::Update(float ts)
	{
		VE_PROFILE_FUNCTION();

		// Reset render stats
		Renderer::ResetStats();
		m_timestep = ts;

		// Handle resize
		if (FramebufferSpecification specification = m_framebuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(specification.Width != m_viewportSize.x || specification.Height != m_viewportSize.y))
		{
			m_framebuffer->Resize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
		}

		// Update
		//m_camera->Update(ts);	
		m_scene->Update(ts);
	}

	void EditorLayer::Render()
	{
		VE_PROFILE_FUNCTION();

		// Prepare for render
		m_framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();

		// Render scene
		m_scene->Render();

		// Unbind framebuffer
		m_framebuffer->Unbind();
	}

	void EditorLayer::ImGuiRender()
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

			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.RenderGui();

		// -----------------------------------------
		//
		//    Statistics
		//
		// -----------------------------------------
		ImGui::Begin("Statistics");

		// Render
		auto stats = Renderer::GetStats();

		ImGui::Text("Renderer Statistics");
		ImGui::Text("\tDraw calls: %d", stats.DrawCalls);
		ImGui::Text("\tQuads: %d", stats.QuadCount);
		ImGui::Text("\tVertices: %d", stats.VertexCount);
		ImGui::Text("\tIndices: %d", stats.IndexCount);
		
		// Camera
		ImGui::NewLine();
		ImGui::Text("Camera");
		ImGui::Text("\tPosition: %.2f/%.2f/%.2f", m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z);
		ImGui::Text("\tYaw: %.2f", m_camera->GetYaw());
		ImGui::Text("\tPitch: %.2f", m_camera->GetPitch());

		// Performance
		const auto categorizedProfileData = Application::Get().GetProfiler()->GetCategorizedProfileTimerData();

		ImGui::NewLine();
		ImGui::Text("Performance");

		for (auto& category : categorizedProfileData)
		{
			ImGui::Text("\t%s", category.first);

			for (auto& categoryData : category.second)
			{
				ImGui::Text("\t\t%s: %.3fms", categoryData.Name, categoryData.ElapsedTime.count() / 1000.0f);
			}
		}

		ImGui::End(); // Statistics

		// -----------------------------------------
		//
		//    Settings
		//
		// -----------------------------------------
		ImGui::Begin("Settings");
		
		if (ImGui::Checkbox("VSync", &m_enableVSync))
			Application::Get().GetWindow().SetVSync(m_enableVSync);

		ImGui::End(); // Settings

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

	void EditorLayer::OnEvent(VoxelEngine::Event& e)
	{
		m_camera->OnEvent(e);

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
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
	}

	void EditorLayer::SaveScene()
	{
	}

	void EditorLayer::SaveSceneAs()
	{
	}

	void EditorLayer::UIToolbar()
	{

	}
}
