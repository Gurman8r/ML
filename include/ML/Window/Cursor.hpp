#ifndef _ML_CURSOR_HPP_
#define _ML_CURSOR_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Cursor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Mode : int32_t
		{
			Normal = 0x34001,	// The arrow cursor is used and motion is not limited.
			Hidden,				// Hide the cursor, but it behaves normally otherwise.
			Disabled,			// Hide the cursor and lock it to the window.

			MAX_MODE,
			MIN_MODE = Normal - 1,
			NUM_MODE = 3
		};

		static constexpr C_String Mode_names[(size_t)Mode::NUM_MODE] = {
			"Normal",
			"Hidden",
			"Disabled",
		};

		static constexpr Mode Mode_values[(size_t)Mode::NUM_MODE] = {
			Mode::Normal,
			Mode::Hidden,
			Mode::Disabled
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class Shape : int32_t
		{
			Arrow = 0x36001,	// Regular arrow cursor shape
			TextInput,			// Text input I-beam cursor shape
			Crosshair,			// Crosshair cursor shape
			Hand,				// Hand cursor shape
			ResizeEW,			// Horizontal resize cursor shape
			ResizeNS,			// Vertical resize cursor shape
			
			// FIXME: GLFW doesn't have these
			ResizeNESW = Arrow,	// The bottom-left resize cursor shape
			ResizeNWSE = Arrow,	// The bottom-right resize cursor shape

			MAX_SHAPE,
			MIN_SHAPE = Arrow - 1,
			NUM_SHAPE = 8
		};

		static constexpr C_String Shape_names[(size_t)Shape::NUM_SHAPE] = {
			"Arrow",
			"Text Input",
			"Cross Hair",
			"Hand",
			"Resize EW",
			"Resize NS",
			"Resize NE/SW",
			"Resize NW/SE",
		};

		static constexpr Shape Shape_values[(size_t)Shape::NUM_SHAPE] = {
			Shape::Arrow,
			Shape::TextInput,
			Shape::Crosshair,
			Shape::Hand,
			Shape::ResizeEW,
			Shape::ResizeNS,
			Shape::ResizeNESW,
			Shape::ResizeNWSE,
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool value_at(int32_t i, Cursor::Mode & value)
		{
			return alg::value_at(i, value, Cursor::Mode_values);
		}

		static constexpr int32_t index_of(Cursor::Mode const & value)
		{
			return alg::index_of(value, Cursor::Mode_values);
		}

		static constexpr C_String name_of(Cursor::Mode const & value)
		{
			const int32_t i = index_of(value);
			return (i >= 0) ? Cursor::Mode_names[i] : "";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool value_at(int32_t i, Cursor::Shape & value)
		{
			return alg::value_at(i, value, Cursor::Shape_values);
		}

		static constexpr int32_t index_of(Cursor::Shape const & value)
		{
			return alg::index_of(value, Cursor::Shape_values);
		}

		static constexpr C_String name_of(Cursor::Shape const & value)
		{
			const int32_t i = index_of(value);
			return (i >= 0) ? Cursor::Shape_names[i] : "";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, Cursor::Mode const & value)
		{
			return out << Cursor::name_of(value);
		}

		inline friend ML_SERIALIZE(std::ostream & out, Cursor::Shape const & value)
		{
			return out << Cursor::name_of(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CURSOR_HPP_