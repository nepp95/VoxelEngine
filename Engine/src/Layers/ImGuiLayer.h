#pragma once

#include "Core/Layer.h"
#include "Events/ApplicationEvent.h"
//#include "Events/KeyEvent.h"
//#include "Events/MouseEvent.h"

namespace VoxelEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_blockEvents = block; }

	private:
		bool m_blockEvents{ true };
	};
}
