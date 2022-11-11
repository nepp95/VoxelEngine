#pragma once

#include <Engine.h>
#include "Panels/Panel.h"

namespace VoxelEngine
{
	class DebugPanel : public Panel
	{
	public:
		DebugPanel() = default;

		void RenderGui() override;
	};
}