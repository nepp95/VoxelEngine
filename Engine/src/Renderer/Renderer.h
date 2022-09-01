// VoxelEngine - Engine
// Renderer.h
// 
// Niels Eppenhof
// https://github.com/nepp95
// 
// Created on: 31-08-2022 22:00
// Last update: 31-08-2022 22:03

#pragma once

namespace VoxelEngine
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		//static void BeginScene(const Camera& camera);
		//static void EndScene();

	private:
		static void Flush();
		static void NextBatch();
		static void StartBatch();
	};
}
