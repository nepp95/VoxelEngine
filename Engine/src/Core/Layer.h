// VoxelEngine - Engine
// Layer.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:02
// Last update: 31-08-2022 21:07

#pragma once

#include "Events/Event.h"

namespace VoxelEngine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetName() const { return m_debugName; }

	private:
		std::string m_debugName;
	};
}
