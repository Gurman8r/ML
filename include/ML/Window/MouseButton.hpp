#ifndef _ML_MOUSE_BUTTON_HPP_
#define _ML_MOUSE_BUTTON_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct MouseButton final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t value;

		constexpr MouseButton(int32_t value)
			: value { value }
		{
		}

		constexpr MouseButton(const MouseButton & copy)
			: MouseButton { copy.value }
		{
		}

		constexpr MouseButton()
			: MouseButton { INVALID_MOUSE_BUTTON }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MOUSE_BUTTON_HPP_