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
			None		= (0 << 0), // 0
			Resizable	= (1 << 0), // 1
			Visible		= (1 << 1), // 2
			Decorated	= (1 << 2), // 4
			Focused		= (1 << 3), // 8
			AutoIconify = (1 << 4), // 16
			Floating	= (1 << 5), // 32
			Maximized	= (1 << 6), // 64
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

		static constexpr C_String Flag_names[] =
		{
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
		)	: resizable		{ resizable		}
			, visible		{ visible		}
			, decorated		{ decorated		}
			, focused		{ focused		}
			, autoIconify	{ autoIconify	}
			, floating		{ floating		}
			, maximized		{ maximized		}
		{
		}

		constexpr WindowStyle(uint32_t value) : WindowStyle {
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
			true, true, true, true, true, false, false
		}
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr uint32_t flags() const
		{
			return Flag::None
				| (resizable	? Flag::Resizable	: 0)
				| (visible		? Flag::Visible		: 0)
				| (decorated	? Flag::Decorated	: 0)
				| (focused		? Flag::Focused		: 0)
				| (autoIconify	? Flag::AutoIconify : 0)
				| (floating		? Flag::Floating	: 0)
				| (maximized	? Flag::Maximized	: 0);
		}

		constexpr bool operator[](Flag i) const
		{
			return this->flags() & static_cast<uint32_t>(i);
		}

		constexpr bool operator==(const WindowStyle & other) const
		{
			return this->flags() == other.flags();
		}

		constexpr bool operator!=(const WindowStyle & other) const
		{
			return !((*this) == other);
		}

		constexpr bool operator<(const WindowStyle & other) const
		{
			return this->flags() < other.flags();
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

	inline ML_SERIALIZE(std::ostream & out, const WindowStyle & value)
	{
		return out
			<< value.resizable << " "
			<< value.visible << " "
			<< value.decorated << " "
			<< value.focused << " "
			<< value.autoIconify << " "
			<< value.floating << " "
			<< value.maximized << " ";
	}

	inline ML_DESERIALIZE(std::istream & in, WindowStyle & value)
	{
		return in
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

	inline ML_SERIALIZE(std::ostream & out, const WindowStyle::Flag & value)
	{
		return out << name_of(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_
