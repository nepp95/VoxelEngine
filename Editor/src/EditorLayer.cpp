// VoxelEngine - Editor
// EditorLayer.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:38
// Last update: 31-08-2022 21:45

#include "EditorLayer.h"

EditorLayer::EditorLayer() {}

void EditorLayer::OnAttach()
{
	CORE_INFO("EditorLayer");	
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(float ts) {}

void EditorLayer::OnImGuiRender() {}

void EditorLayer::OnEvent(VoxelEngine::Event& e) {}
