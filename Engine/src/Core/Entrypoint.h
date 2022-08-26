#pragma once

#include "Core/Application.h"

int main(int argc, char** argv)
{
	auto app = VoxelEngine::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}