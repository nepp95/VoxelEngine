#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	m_activeScene = CreateRef<Scene>();
}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::Update(float ts)
{
	m_activeScene->OnUpdateRuntime(ts);
}

void SandboxLayer::Render()
{
	RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.1f, 1.0f });
	RenderCommand::Clear();
	Renderer::ResetStats();

	m_activeScene->OnRenderRuntime();
}

void SandboxLayer::RenderGui()
{

}

void SandboxLayer::OnEvent(Event& e)
{

}
