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
		MainMenuBar(Editor & editor);
		~MainMenuBar();

	public:
		bool drawGui(const GuiEvent & ev, bool * p_open) override;

	protected:
		bool beginDraw(bool * p_open, int32_t flags = 0) override;
		bool endDraw() override;
	};
}

#endif // !_ML_MAIN_MENU_BAR_HPP_