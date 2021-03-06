#ifndef _ML_MOUSE_BUTTON_HPP_
#define _ML_MOUSE_BUTTON_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct MouseButton final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : int32_t
		{
			INVALID_MOUSE_BUTTON = -1,

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MOUSE_BUTTON_HPP_