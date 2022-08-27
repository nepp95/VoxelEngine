// VoxelEngine - Engine
// Entrypoint.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 25-08-2022 15:21
// Last update: 26-08-2022 22:07

#pragma once

#include "Core/Application.h"

int main(int argc, char** argv)
{
	auto app = VoxelEngine::CreateApplication({argc, argv});
	app->Run();
	delete app;
}
