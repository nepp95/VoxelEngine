#pragma once

#include <EpEngine.h>
#include "Panels/Panel.h"

namespace EpEngine
{
	struct PanelData
	{
		Ref<Panel> Panel = nullptr;
		bool IsOpen{ false };
	};

	class PanelManager
	{
	public:
		PanelManager() = default;
		~PanelManager();

		void RenderGui();
		void SetSceneContext(const Ref<Scene>& scene);

		template<typename T, typename... Args>
		void AddPanel(const std::string& name, bool isOpen, Args&&... args)
		{
			static_assert(std::is_base_of<Panel, T>::value, "PanelManager::AddPanel requires type to derive from Panel!");

			if (m_panelData.find(name) != m_panelData.end())
				return;

			PanelData panelData;
			panelData.IsOpen = isOpen;
			panelData.Panel = CreateRef<T>(std::forward<Args>(args)...);

			m_panelData.insert({ name, panelData });
		}
		
		void RemovePanel(const std::string& name);

		PanelData* GetPanelData(const std::string& name);

	private:
		std::unordered_map<std::string, PanelData> m_panelData;
	};
}