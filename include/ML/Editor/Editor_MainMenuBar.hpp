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

	public:
		using Callback = std::function<void()>;

		using Menus = Multimap<String, Callback>;

		template <class Fun>
		inline Menus::iterator addMenu(const String & name, Fun && fun)
		{
			return m_menus.insert({ name, fun });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Menus m_menus;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_MAIN_MENU_HPP_