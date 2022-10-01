#include "EditorLayer.h"

#include <imgui/imgui.h>

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(float ts)
{
	// Reset render stats
	VE::Renderer::ResetStats();

	// Prepare for render
	VE::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	VE::RenderCommand::Clear();

	// Render scene
	VE::Renderer::BeginScene(m_camera);
	VE::Renderer::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f });
	VE::Renderer::EndScene();
}

void EditorLayer::OnImGuiRender()
{
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
	ImGui::PopStyleVar();

	if (optFullscreen)
		ImGui::PopStyleVar(2);

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

	// -----------------------------------------
	//
	//    Statistics
	//
	// -----------------------------------------
	ImGui::Begin("Statistics");

	auto stats = VE::Renderer::GetStats();
	ImGui::Text("Renderer2D Statistics:");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.VertexCount);
	ImGui::Text("Indices: %d", stats.IndexCount);

	ImGui::End();

	// -----------------------------------------
	//
	//    Settings
	//
	// -----------------------------------------
	ImGui::Begin("Settings");
	
	ImGui::End();

	ImGui::End();
}

void EditorLayer::OnEvent(VoxelEngine::Event& e) {}
