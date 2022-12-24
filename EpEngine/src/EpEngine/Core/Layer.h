#pragma once

#include "EpEngine/Events/Event.h"

namespace EpEngine
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

		virtual void RenderGui() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_debugName; }

	private:
		std::string m_debugName;
	};
}
