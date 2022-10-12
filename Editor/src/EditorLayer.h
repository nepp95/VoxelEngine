#pragma once

#include <Engine.h>

using namespace VoxelEngine;

class EditorLayer : public Layer
{
public:
	EditorLayer(const std::string& name = std::string())
		: Layer(name.empty() ? name : "EditorLayer")
	{}

	~EditorLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(float ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

private:
	// Test data
	Ref<Texture> m_grassTexture;

	// Camera
	Camera* m_camera;
	//Camera m_camera{ { 0.0f, 0.0f, 3.0f } };

	// Level
	Ref<Level> m_level;

	// Viewport
	bool m_viewportFocused{false}, m_viewportHovered{false};
	glm::vec2 m_viewportBounds[2];
	glm::vec2 m_viewportSize{ 0.0f, 0.0f };

	// Framebuffer
	Ref<Framebuffer> m_framebuffer;

	// Settings
	bool m_enableVSync;
};
