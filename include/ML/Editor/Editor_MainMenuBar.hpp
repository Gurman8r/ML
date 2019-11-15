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
		decltype(m_menus)::iterator addMenu(const String & name, std::function<void()> && fun);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_MAIN_MENU_HPP_