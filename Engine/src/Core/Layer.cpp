// VoxelEngine - Engine
// Layer.cpp
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 21:02
// Last update: 31-08-2022 21:06

#include "pch.h"
#include "Layer.h"

namespace VoxelEngine
{
	Layer::Layer(const std::string& name)
		: m_debugName(name) {}

	Layer::~Layer() {}
}
