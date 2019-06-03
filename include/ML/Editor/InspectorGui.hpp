#ifndef _ML_INSPECTOR_GUI_HPP_
#define _ML_INSPECTOR_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API InspectorGui final 
		: public EditorGui
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit InspectorGui(EventSystem & eventSystem);
		~InspectorGui();

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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INSPECTOR_GUI_HPP_