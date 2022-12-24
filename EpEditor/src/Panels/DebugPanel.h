#pragma once

#include <EpEngine.h>
#include "Panels/Panel.h"

namespace EpEngine
{
	class DebugPanel : public Panel
	{
	public:
		DebugPanel() = default;

		void RenderGui() override;
	};
}