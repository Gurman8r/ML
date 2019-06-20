#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

#include <ML/Window/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Cursor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Mode : int32_t
		{
			Normal = 0x34001,	// The arrow cursor is used and motion is not limited.
			Hidden,				// Hide the cursor, but it behaves normally otherwise.
			Disabled,			// Hide the cursor and lock it to the window.

			MAX_MODE,
			MIN_MODE = Normal - 1,
			NUM_MODE = 3
		};

		static constexpr CString Mode_names[NUM_MODE] = {
			"Normal",
			"Hidden",
			"Disabled",
		};

		static constexpr Mode Mode_values[NUM_MODE] = {
			Mode::Normal,
			Mode::Hidden,
			Mode::Disabled
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Shape : int32_t
		{
			Arrow = 0x36001,	// The regular arrow cursor shape
			TextInput,			// The text input I-beam cursor shape.
			Crosshair,			// The crosshair shape.
			Hand,				// The hand shape.
			ResizeEW,			// The horizontal resize arrow shape.
			ResizeNS,			// The vertical resize arrow shape.

			MAX_SHAPE,
			MIN_SHAPE = Arrow - 1,
			NUM_SHAPE = 6
		};

		static constexpr CString Shape_names[NUM_SHAPE] = {
			"Arrow",
			"Text Input",
			"Cross Hair",
			"Hand",
			"Resize EW",
			"Resize NS",
		};

		static constexpr Shape Shape_values[NUM_SHAPE] = {
			Shape::Arrow,
			Shape::TextInput,
			Shape::Crosshair,
			Shape::Hand,
			Shape::ResizeEW,
			Shape::ResizeNS,
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CURSOR_HPP_