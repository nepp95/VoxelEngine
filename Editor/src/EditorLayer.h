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
	VE::Camera m_camera{ { 0.0f, 0.0f, 3.0f } };
	bool m_viewportFocused{false}, m_viewportHovered{false};
};
