#ifndef _ML_GUI_TOOL_HPP_
#define _ML_GUI_TOOL_HPP_

#include <ML/Editor/GUI_Base.hpp>

namespace ml
{
	// A GUI drawer for draw(void)
	class ML_EDITOR_API GUI_Tool
		: public GUI_Base
	{
	public:
		GUI_Tool();
		virtual ~GUI_Tool();

	public:
		virtual bool beginDraw() = 0;
		virtual bool endDraw() = 0;
	};
}

#endif // !_ML_GUI_TOOL_HPP_