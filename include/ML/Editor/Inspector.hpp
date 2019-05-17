#ifndef _ML_INSPECTOR_HPP_
#define _ML_INSPECTOR_HPP_

#include <ML/Editor/GUI_Window.hpp>

#define ML_Inspector ml::Inspector::getInstance()

namespace ml
{
	class ML_EDITOR_API Inspector final
		: public GUI_Window
		, public ISingleton<Inspector>
	{
		friend class ISingleton<Inspector>;

	private:
		Inspector();
		~Inspector();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	public:
		template <
			typename _Fun
		> inline bool drawFun(bool * p_open, _Fun fun)
		{
			if (drawGui(p_open))
			{
				fun();
			}
			return endDraw();
		}
	};
}

#endif // !_ML_INSPECTOR_HPP_