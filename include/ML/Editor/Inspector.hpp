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
		/* * * * * * * * * * * * * * * * * * * * */
		Inspector(Editor & editor, bool open);
		~Inspector();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class Fun
		> inline bool drawFun(const GuiEvent & ev, Fun && fun)
		{
			if (m_open)
			{
				if (drawGui(ev))
				{
					fun();
				}
				return endDraw();
			}
			return false;
		}
	};
}

#endif // !_ML_INSPECTOR_HPP_