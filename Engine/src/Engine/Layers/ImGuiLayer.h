#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/ApplicationEvent.h"

namespace VoxelEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_blockEvents = block; }

	private:
		void SetDarkThemeColors();

	private:
		bool m_blockEvents{ true };
	};
}
