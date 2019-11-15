#include <ML/Editor/Editor_MainMenuBar.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Engine/Engine.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_MainMenuBar::Editor_MainMenuBar()
		: EditorComponent{ "Main Menu", "", ML_Engine.prefs().get_bool("Editor", "show_main_menu", true) }
		, m_menus{}
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor_MainMenuBar::dispose()
	{
		m_menus.clear();
		return m_menus.empty();
	}

	void Editor_MainMenuBar::update()
	{
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
				if (ImGui::BeginMenu(pair.first.c_str()))
				{
					for (auto & func : pair.second)
					{
						func();
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

	auto Editor_MainMenuBar::addMenu(const String & name, std::function<void()> && fun)
		-> decltype(m_menus)::iterator
	{
		auto it{ std::find_if(m_menus.begin(), m_menus.end(), [&](auto elem)
		{
			return elem.first == name;
		}) };
		if (it == m_menus.end())
		{
			m_menus.push_back({ name, {} });
			it = (m_menus.end() - 1);
		}
		it->second.push_back(fun);
		return it;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}