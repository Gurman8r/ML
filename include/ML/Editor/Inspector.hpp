#ifndef _ML_INSPECTOR_HPP_
#define _ML_INSPECTOR_HPP_

#include <ML/Editor/BaseWidget.hpp>

namespace ml
{
	class ML_EDITOR_API Inspector final
		: public BaseWidget
	{
		friend class Editor;

	private:
		Inspector(Editor & editor, bool open = true);
		~Inspector();

	public:
		bool drawGui(const GuiEvent & ev) override;

	public:
		template <
			class Fun
		> inline bool drawFun(const GuiEvent & ev, Fun && fun)
		{
			if (drawGui(ev))
			{
				fun();
			}
			return endDraw();
		}
	};
}

#endif // !_ML_INSPECTOR_HPP_