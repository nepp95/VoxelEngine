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
