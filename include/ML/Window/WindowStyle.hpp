#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowStyle final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : uint32_t
		{
			None		= (0 << 0), // 0
			Fullscreen	= (1 << 0), // 1
			Resizable	= (1 << 1), // 2
			Visible		= (1 << 2), // 4
			Decorated	= (1 << 3), // 8
			Focused		= (1 << 4), // 16
			AutoIconify = (1 << 5), // 32
			Floating	= (1 << 6), // 64
			Maximized	= (1 << 7), // 128
		};

		static constexpr Flag Flag_values[] = {
			Flag::None,
			Flag::Fullscreen,
			Flag::Resizable,
			Flag::Visible,
			Flag::Decorated,
			Flag::Focused,
			Flag::AutoIconify,
			Flag::Floating,
			Flag::Maximized
		};

		static constexpr C_String Flag_names[] =
		{
			"None",
			"Fullscreen",
			"Resizable",
			"Visible",
			"Decorated",
			"Focused",
			"AutoIconify",
			"Floating",
			"Maximized"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool fullscreen;	// Fullscreen
		bool resizable;		// Resizable
		bool visible;		// Visible
		bool decorated;		// Decorated
		bool focused;		// Focused
		bool autoIconify;	// AutoIconify
		bool floating;		// Floating
		bool maximized;		// Maximized

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr WindowStyle(bool fullscreen, bool resizable, bool visible, bool decorated, bool focused, bool autoIconify, bool floating, bool maximized)
			: fullscreen	{ fullscreen	}
			, resizable		{ resizable		}
			, visible		{ visible		}
			, decorated		{ decorated		}
			, focused		{ focused		}
			, autoIconify	{ autoIconify	}
			, floating		{ floating		}
			, maximized		{ maximized		}
		{
		}

		constexpr WindowStyle(uint32_t value) : WindowStyle {
			(value & Flag::Fullscreen	),
			(value & Flag::Resizable	),
			(value & Flag::Visible		),
			(value & Flag::Decorated	),
			(value & Flag::Focused		),
			(value & Flag::AutoIconify	),
			(value & Flag::Floating		),
			(value & Flag::Maximized	)
		}
		{
		}

		constexpr WindowStyle(const WindowStyle & copy) : WindowStyle {
			copy.fullscreen,
			copy.resizable,
			copy.visible,
			copy.decorated,
			copy.focused,
			copy.autoIconify,
			copy.floating,
			copy.maximized
		}
		{
		}

		constexpr WindowStyle() : WindowStyle {
			false, true, true, true, true, true, false, false
		}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool operator==(const WindowStyle & other) const
		{
			return
				this->fullscreen	== other.fullscreen		&&
				this->resizable		== other.resizable		&&
				this->visible		== other.visible		&&
				this->decorated		== other.decorated		&&
				this->focused		== other.focused		&&
				this->autoIconify	== other.autoIconify	&&
				this->floating		== other.floating		&&
				this->maximized		== other.maximized;
		}

		constexpr bool operator!=(const WindowStyle & other) const
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const WindowStyle & other) const
		{
			return
				this->fullscreen	< other.fullscreen		||
				this->resizable		< other.resizable		||
				this->visible		< other.visible			||
				this->decorated		< other.decorated		||
				this->focused		< other.focused			||
				this->autoIconify	< other.autoIconify		||
				this->floating		< other.floating		||
				this->maximized		< other.maximized;
		}

		constexpr bool operator>(const WindowStyle & other) const
		{
			return !((*this) < other);
		}

		constexpr bool operator<=(const WindowStyle & other) const
		{
			return ((*this) == other) || ((*this) < other);
		}

		constexpr bool operator>=(const WindowStyle & other) const
		{
			return ((*this) == other) || ((*this) > other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const WindowStyle & value)
	{
		return out
			<< value.fullscreen << " "
			<< value.resizable << " "
			<< value.visible << " "
			<< value.decorated << " "
			<< value.focused << " "
			<< value.autoIconify << " "
			<< value.floating << " "
			<< value.maximized << " ";
	}

	inline ML_DESERIALIZE(Istream & in, WindowStyle & value)
	{
		return in
			>> value.fullscreen
			>> value.resizable
			>> value.visible
			>> value.decorated
			>> value.focused
			>> value.autoIconify
			>> value.floating
			>> value.maximized;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	static constexpr bool value_at(int32_t i, WindowStyle::Flag & value)
	{
		return alg::value_at(i, value, WindowStyle::Flag_values);
	}

	static constexpr int32_t index_of(const WindowStyle::Flag & value)
	{
		return alg::index_of(value, WindowStyle::Flag_values);
	}

	static constexpr C_String name_of(const WindowStyle::Flag & value)
	{
		const int32_t i = index_of(value);
		return (i >= 0) ? WindowStyle::Flag_names[i] : "";
	}

	inline ML_SERIALIZE(Ostream & out, const WindowStyle::Flag & value)
	{
		return out << name_of(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_
