#include "DebugPanel.h"

#include <imgui.h>

namespace EpEngine
{
	void DebugPanel::RenderGui()
	{
		ImGui::Begin("Debug");

		// Render
		auto stats = Renderer::GetStats();

		ImGui::Text("Renderer Statistics");
		ImGui::Text("\tDraw calls: %d", stats.DrawCalls);
		ImGui::Text("\tQuads: %d", stats.QuadCount);
		ImGui::Text("\tVertices: %d", stats.VertexCount);
		ImGui::Text("\tIndices: %d", stats.IndexCount);

		// Performance
		const auto categorizedProfileData = Application::Get().GetProfiler()->GetCategorizedProfileTimerData();

		ImGui::Separator();
		ImGui::Text("Performance");

		for (auto& category : categorizedProfileData)
		{
			ImGui::Text("\t%s", category.first.c_str());

			for (auto& categoryData : category.second)
			{
				ImGui::Text("\t\t%s: %.3fms", categoryData.Name.c_str(), categoryData.ElapsedTime.count() / 1000.0f);
			}
		}

		ImGui::End(); // Statistics
	}
}
