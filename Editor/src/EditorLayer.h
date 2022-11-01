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

	void Update(float ts) override;
	void Render() override;
	void ImGuiRender() override;

	void OnEvent(Event& e) override;

private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

private:
	// Test data
	Ref<Texture> m_grassTexture;
	float m_timestep{ 0.0f };
	uint32_t m_frames{ 0 };

	// Camera
	Camera* m_camera;

	// Scene
	Ref<Scene> m_scene;

	// Viewport
	bool m_viewportFocused{false}, m_viewportHovered{false};
	glm::vec2 m_viewportBounds[2];
	glm::vec2 m_viewportSize{ 0.0f, 0.0f };

	// Framebuffer
	Ref<Framebuffer> m_framebuffer;

	// Settings
	bool m_enableVSync;
};
