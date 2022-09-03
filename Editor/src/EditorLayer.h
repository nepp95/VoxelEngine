// VoxelEngine - Editor
// EditorLayer.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:38
// Last update: 31-08-2022 21:41

#pragma once

#include <Engine.h>

class EditorLayer : public VoxelEngine::Layer
{
public:
	EditorLayer();
	~EditorLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float ts) override;
	void OnImGuiRender() override;
	void OnEvent(VoxelEngine::Event& e) override;

private:
	bool m_viewportFocused{false}, m_viewportHovered{false};
};
