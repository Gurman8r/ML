#include <ML/Editor/Editor_MainMenuBar.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_MainMenuBar::Editor_MainMenuBar()
		: Editor_Widget{ "Main Menu Bar", "", ML_Engine.prefs().get_bool("Editor", "show_main_menu", true) }
		, m_menus{}
	{
		ML_EventSystem.addListener<UnloadEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_MainMenuBar::onEvent(Event const & value)
	{
		Editor_Widget::onEvent(value);

		switch (*value)
		{
		case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			m_menus.clear();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	bool Editor_MainMenuBar::beginDraw(int32_t flags)
	{
		if (m_good = m_open)
		{
			ImGui::PushID((int32_t)typeof<>(*this).hash);
			ImGui::PushID(ML_ADDRESSOF(this));
			ImGui::PushID(getTitle());
			m_good = ImGui::BeginMainMenuBar();
		}
		return m_good;
	}

	bool Editor_MainMenuBar::draw()
	{
		if (beginDraw(0))
		{
			for (auto & pair : m_menus)
			{
				if (!pair.second.empty() && ImGui::BeginMenu(pair.first.c_str()))
				{
					for (auto & func : pair.second)
					{
						if (func) { func(); }
					}
					ImGui::EndMenu();
				}
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