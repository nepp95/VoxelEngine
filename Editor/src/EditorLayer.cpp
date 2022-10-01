#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(float ts)
{
	VE::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	VE::RenderCommand::Clear();

	VE::Renderer::BeginScene(m_camera);
	VE::Renderer::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f });
	VE::Renderer::EndScene();
}

void EditorLayer::OnImGuiRender() {}

void EditorLayer::OnEvent(VoxelEngine::Event& e) {}
