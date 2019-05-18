#ifndef _ML_MOUSE_BUTTON_HPP_
#define _ML_MOUSE_BUTTON_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API MouseButton final
	{
		enum : int32_t
		{
			Button0,
			Button1,
			Button2,
			Button3,
			Button4,
			Button5,
			Button6,
			Button7,

			MAX_MOUSE_BUTTON,
			MIN_MOUSE_BUTTON = Button0,
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MOUSE_BUTTON_HPP_