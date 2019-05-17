#include <ML/Editor/NetworkHUD.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Network/NetClient.hpp>
#include <ML/Network/NetServer.hpp>

namespace ml
{
	NetworkHUD::NetworkHUD()
		: GUI_Window("Network")
	{
	}

	NetworkHUD::~NetworkHUD()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void NetworkHUD::onEvent(const IEvent * value)
	{
	}

	bool NetworkHUD::drawGui(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", title());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}