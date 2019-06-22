#ifndef _ML_FMT_HPP_
#define _ML_FMT_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class FG : uint16_t
	{
		Black,
		DarkBlue	= (1 << 0),
		DarkGreen	= (1 << 1),
		DarkCyan	= DarkGreen | DarkBlue,
		DarkRed		= (1 << 2),
		DarkMagenta = DarkRed | DarkBlue,
		DarkYellow	= DarkRed | DarkGreen,
		Normal		= DarkRed | DarkGreen | DarkBlue,
		Gray		= (1 << 3),
		Blue		= Gray | DarkBlue,
		Green		= Gray | DarkGreen,
		Cyan		= Gray | DarkGreen | DarkBlue,
		Red			= Gray | DarkRed,
		Magenta		= Gray | DarkRed | DarkBlue,
		Yellow		= Gray | DarkRed | DarkGreen,
		White		= Gray | DarkRed | DarkGreen | DarkBlue,

		None = static_cast<uint16_t>(-1)
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class BG : uint16_t
	{
		Black,
		DarkBlue	= (1 << 4),
		DarkGreen	= (1 << 5),
		DarkCyan	= DarkGreen | DarkBlue,
		DarkRed		= (1 << 6),
		DarkMagenta = DarkRed | DarkBlue,
		DarkYellow	= DarkRed | DarkGreen,
		Gray		= DarkRed | DarkGreen | DarkBlue,
		DarkGray	= (1 << 7),
		Blue		= DarkGray | DarkBlue,
		Green		= DarkGray | DarkGreen,
		Cyan		= DarkGray | DarkGreen | DarkBlue,
		Red			= DarkGray | DarkRed,
		Magenta		= DarkGray | DarkRed | DarkBlue,
		Yellow		= DarkGray | DarkRed | DarkGreen,
		White		= DarkGray | DarkRed | DarkGreen | DarkBlue,
		
		None = static_cast<uint16_t>(-1)
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API FMT final
	{
		static bool setTextAttrib(const uint16_t value);

	public:
		FG fg;
		BG bg;

		constexpr FMT(const FG fg, const BG bg)
			: fg(fg)
			, bg(bg)
		{
		}

		constexpr FMT()
			: FMT { FG::Normal, BG::Black }
		{
		}

		constexpr FMT(const FG fg)
			: FMT { fg, BG::Black }
		{
		}

		constexpr FMT(const BG bg)
			: FMT { FG::Normal, bg }
		{
		}

		constexpr FMT(const FMT & copy)
			: FMT { copy.fg, copy.bg }
		{
		}

		friend ML_SERIALIZE(ostream &, const FMT &);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr FMT operator|(const BG & bg, const FG & fg)
	{
		return FMT { fg, bg };
	};

	constexpr FMT operator|(const FG & fg, const BG & bg)
	{
		return FMT { fg, bg };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(ostream & out, const FG & value)
	{
		return out << FMT { value, BG::None };
	}

	inline ML_SERIALIZE(ostream & out, const BG & value)
	{
		return out << FMT { FG::None, value };
	}

	inline ML_SERIALIZE(ostream & out, const FMT & value)
	{
		if ((value.fg != FG::None) && (value.bg != BG::None))
		{
			FMT::setTextAttrib((uint16_t)value.fg | (uint16_t)value.bg);
		}
		else if (value.fg != FG::None)
		{
			FMT::setTextAttrib((uint16_t)value.fg);
		}
		else if (value.bg != BG::None)
		{
			FMT::setTextAttrib((uint16_t)value.bg);
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FMT_HPP_