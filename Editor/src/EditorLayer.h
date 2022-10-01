#pragma once

#include <Engine.h>

namespace VE = VoxelEngine;
using VE::Ref, VE::Scope, VE::CreateRef, VE::CreateScope;

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
	// Camera
	VE::Camera m_camera{ { 0.0f, 0.0f, 3.0f } };

	// Viewport
	bool m_viewportFocused{false}, m_viewportHovered{false};
	glm::vec2 m_viewportBounds[2];
	glm::vec2 m_viewportSize{ 0.0f, 0.0f };

	// Framebuffer
	Ref<VE::Framebuffer> m_framebuffer;

	// Settings
	bool m_enableVSync;
};
