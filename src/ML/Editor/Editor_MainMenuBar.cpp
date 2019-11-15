#include <ML/Editor/Editor_MainMenuBar.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_MainMenuBar::Editor_MainMenuBar()
		: EditorComponent{ "Main Menu", "", true }
		, m_menus{}
	{
	}

	bool Editor_MainMenuBar::dispose()
	{
		m_menus.clear();
		return m_menus.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_MainMenuBar::update()
	{
	}

	bool Editor_MainMenuBar::beginDraw(int32_t flags)
	{
		ImGui::PushID((int32_t)typeof<>(*this).hash);
		ImGui::PushID(ML_ADDRESSOF(this));
		ImGui::PushID(getTitle());
		return m_good = ImGui::BeginMainMenuBar();
	}

	bool Editor_MainMenuBar::draw()
	{
		if (beginDraw(0))
		{
			for (auto it = m_menus.begin(); 
				it != m_menus.end(); 
				it = m_menus.upper_bound(it->first))
			{
				ImGui::PushID(it->first.c_str());
				if (ImGui::BeginMenu(it->first.c_str()))
				{
					auto funcs{ m_menus.equal_range(it->first) };
					for (auto cur = funcs.first; cur != funcs.second; ++cur)
					{
						ImGui::PushID(ML_ADDRESSOF(&cur->second));
						cur->second();
						ImGui::PopID();
					}
					ImGui::EndMenu();
				}
				ImGui::PopID();
			}
		}
		return endDraw();
	}

	bool Editor_MainMenuBar::endDraw()
	{
		ImGui::EndMainMenuBar();
		ImGui::PopID();
		ImGui::PopID();
		ImGui::PopID();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}