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
		Inspector();
		~Inspector();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(const GuiEvent * ev, bool * p_open) override;

	public:
		template <
			class Fun
		> inline bool drawFun(const GuiEvent * ev, bool * p_open, Fun && fun)
		{
			if (drawGui(ev, p_open))
			{
				fun();
			}
			return endDraw();
		}
	};
}

#endif // !_ML_INSPECTOR_HPP_