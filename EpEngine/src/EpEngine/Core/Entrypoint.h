#pragma once

#include "EpEngine/Core/Application.h"

int main(int argc, char** argv)
{
	auto app = EpEngine::CreateApplication({argc, argv});
	app->Run();
	delete app;
}
