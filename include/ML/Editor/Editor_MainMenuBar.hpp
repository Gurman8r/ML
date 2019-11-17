#ifndef _ML_EDITOR_MAIN_MENU_HPP_
#define _ML_EDITOR_MAIN_MENU_HPP_

#include <ML/Editor/EditorComponent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_MainMenuBar final : public EditorComponent
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_MainMenuBar();

		~Editor_MainMenuBar() { this->dispose(); }

		bool dispose() override;

		void update() override;

		bool beginDraw(int32_t flags) override;

		bool draw() override;

		bool endDraw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		List<std::pair<
			String,
			List<std::function<void()>>
		>> m_menus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		inline decltype(m_menus)::iterator addMenu(const String & name, std::function<void()> && fun)
		{
			auto it{ std::find_if(m_menus.begin(), m_menus.end(), [&](auto elem)
			{
				return (elem.first == name);
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
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_MAIN_MENU_HPP_