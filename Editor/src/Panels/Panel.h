#pragma once

#include <Engine.h>

namespace VoxelEngine
{
	class Panel
	{
	public:
		virtual ~Panel() = default;

		virtual void RenderGui() = 0;
		virtual void SetSceneContext(const Ref<Scene>& scene) {};
	};
}