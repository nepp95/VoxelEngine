#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(float ts)
{
	VE::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	VE::RenderCommand::Clear();
}

void EditorLayer::OnImGuiRender() {}

void EditorLayer::OnEvent(VoxelEngine::Event& e) {}
