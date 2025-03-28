#pragma once

#include "Engine/Core/Layer.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void OnImGuiRender(TimeStep ts) override;

		void Begin();
		void End();
		void InitializeDockSpace();
		void BeginDockSpace();
		void EndDockSpace();
		
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;

	private:
		bool m_BlockEvents = true;
	};
}