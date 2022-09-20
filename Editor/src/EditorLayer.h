// VoxelEngine - Editor
// EditorLayer.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:38
// Last update: 20-09-2022 15:05

#pragma once

#include <Engine.h>

namespace VE = VoxelEngine;

class EditorLayer : public VE::Layer
{
public:
	EditorLayer(const std::string& name)
		: Layer(name)
	{}

	~EditorLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float ts) override;
	void OnImGuiRender() override;
	void OnEvent(VE::Event& e) override;

private:
	bool m_viewportFocused{false}, m_viewportHovered{false};
};
