// VoxelEngine - Editor
// EditorLayer.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:38
// Last update: 20-09-2022 15:08

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
