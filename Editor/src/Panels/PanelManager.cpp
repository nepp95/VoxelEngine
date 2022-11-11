#include "PanelManager.h"

namespace VoxelEngine
{
	PanelManager::~PanelManager()
	{
		m_panelData.clear();
	}

	void PanelManager::RenderGui()
	{
		for (auto& [name, panelData] : m_panelData)
			if (panelData.IsOpen)
				panelData.Panel->RenderGui();
	}

	void PanelManager::SetSceneContext(const Ref<Scene>& scene)
	{
		for (auto& [name, panelData] : m_panelData)
			panelData.Panel->SetSceneContext(scene);
	}

	void PanelManager::RemovePanel(const std::string& name)
	{
		if (m_panelData.find(name) != m_panelData.end())
			m_panelData.erase(name);
	}

	PanelData* PanelManager::GetPanelData(const std::string& name)
	{
		if (m_panelData.find(name) != m_panelData.end())
			return &m_panelData.at(name);

		return nullptr;
	}
}