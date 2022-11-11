#pragma once

#include "Engine/Events/Event.h"

namespace VoxelEngine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void Update(float ts) {}
		virtual void Render() {}

		virtual void ImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_debugName; }

	private:
		std::string m_debugName;
	};
}
