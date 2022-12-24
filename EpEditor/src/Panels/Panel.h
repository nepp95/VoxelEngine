#pragma once

#include <EpEngine.h>

namespace EpEngine
{
	class Panel
	{
	public:
		virtual ~Panel() = default;

		virtual void RenderGui() = 0;
		virtual void SetSceneContext(const Ref<Scene>& scene) {};
	};
}