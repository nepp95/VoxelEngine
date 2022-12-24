#pragma once

#include <EpEngine.h>

using namespace EpEngine;

class SandboxLayer : public Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox")
	{}
	SandboxLayer(const SandboxLayer& other) = delete;

	~SandboxLayer() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void Update(float ts) override;
	void Render() override;
	void RenderGui() override;

	void OnEvent(Event& e) override;

private:
	Ref<Scene> m_activeScene;
};
