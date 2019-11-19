#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowStyle final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : uint32_t
		{
			None		= (0 << 0),
			Resizable	= (1 << 0),
			Visible		= (1 << 1),
			Decorated	= (1 << 2),
			Focused		= (1 << 3),
			AutoIconify = (1 << 4),
			Floating	= (1 << 5),
			Maximized	= (1 << 6),
		};

		static constexpr Flag Flag_values[] = {
			Flag::None,
			Flag::Resizable,
			Flag::Visible,
			Flag::Decorated,
			Flag::Focused,
			Flag::AutoIconify,
			Flag::Floating,
			Flag::Maximized
		};

		static constexpr C_String Flag_names[] = {
			"None",
			"Resizable",
			"Visible",
			"Decorated",
			"Focused",
			"AutoIconify",
			"Floating",
			"Maximized"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool resizable;		// Resizable
		bool visible;		// Visible
		bool decorated;		// Decorated
		bool focused;		// Focused
		bool autoIconify;	// AutoIconify
		bool floating;		// Floating
		bool maximized;		// Maximized

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr WindowStyle(
			bool resizable,
			bool visible,
			bool decorated,
			bool focused,
			bool autoIconify,
			bool floating,
			bool maximized
		)	: resizable		{ resizable }
			, visible		{ visible }
			, decorated		{ decorated }
			, focused		{ focused }
			, autoIconify	{ autoIconify }
			, floating		{ floating }
			, maximized		{ maximized }
		{
		}

		constexpr WindowStyle(uint32_t value) 
			: resizable		{ (bool)(value & Flag::Resizable) }
			, visible		{ (bool)(value & Flag::Visible) }
			, decorated		{ (bool)(value & Flag::Decorated) }
			, focused		{ (bool)(value & Flag::Focused) }
			, autoIconify	{ (bool)(value & Flag::AutoIconify) }
			, floating		{ (bool)(value & Flag::Floating) }
			, maximized		{ (bool)(value & Flag::Maximized) }
		{
		}

		constexpr WindowStyle(WindowStyle const & copy) 
			: resizable		{ copy.resizable }
			, visible		{ copy.visible }
			, decorated		{ copy.decorated }
			, focused		{ copy.focused }
			, autoIconify	{ copy.autoIconify }
			, floating		{ copy.floating }
			, maximized		{ copy.maximized }
		{
		}

		constexpr WindowStyle() 
			: resizable		{ true }
			, visible		{ true }
			, decorated		{ true }
			, focused		{ true }
			, autoIconify	{ true }
			, floating		{ false }
			, maximized		{ false }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(WindowStyle const & other) const
		{
			return
				resizable	== other.resizable &&
				visible		== other.visible &&
				decorated	== other.decorated &&
				focused		== other.focused &&
				autoIconify	== other.autoIconify &&
				floating	== other.floating &&
				maximized	== other.maximized;
		}

		constexpr bool operator!=(WindowStyle const & other) const
		{
			return !((*this) == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_