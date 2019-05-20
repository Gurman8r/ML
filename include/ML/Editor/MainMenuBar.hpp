#ifndef _ML_MAIN_MENU_BAR_HPP_
#define _ML_MAIN_MENU_BAR_HPP_

#include <ML/Editor/BaseWidget.hpp>

namespace ml
{
	class ML_EDITOR_API MainMenuBar
		: public BaseWidget
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		MainMenuBar(Editor & editor, bool open);
		~MainMenuBar();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;
		bool beginDraw(int32_t flags = 0) override;
		bool endDraw() override;
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_