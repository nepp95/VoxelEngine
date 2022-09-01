// VoxelEngine - Engine
// ApplicationEvent.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 20:56
// Last update: 31-08-2022 20:58

#pragma once

#include "Events/Event.h"

namespace VoxelEngine
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose);
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_width(width), m_height(height)
		{
		}

		EVENT_CLASS_TYPE(WindowResize);

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	private:
		uint32_t m_width, m_height;
	};
}
