#ifndef _ML_CONSOLE_HPP_
#define _ML_CONSOLE_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/ISingleton.hpp>

#define ML_Console ml::Console::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Console final
	{
		static bool enableMenuItem(const uint32_t item, const uint32_t enable);
		static bool setTextAttribute(const uint16_t value);
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FG final
	{
		enum Color : uint16_t
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

			MAX_COLOR	= 16,
			DEFAULT		= Normal
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API BG final
	{
		enum Color : uint16_t
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

			MAX_COLOR	= 16,
			DEFAULT		= Black
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FMT final
	{
		FG::Color fg;
		BG::Color bg;

		FMT(const FG::Color fg, const BG::Color bg)
			: fg(fg)
			, bg(bg)
		{
		}
		FMT(const FG::Color fg)
			: FMT(fg, BG::DEFAULT)
		{
		}
		FMT(const BG::Color bg)
			: FMT(FG::DEFAULT, bg)
		{
		}
		FMT(const FMT & copy)
			: FMT(copy.fg, copy.bg)
		{
		}
		FMT()
			: FMT(FG::DEFAULT, BG::DEFAULT)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline FMT operator|(const BG::Color & bg, const FG::Color & fg) { return FMT(fg, bg); };
	inline FMT operator|(const FG::Color & fg, const BG::Color & bg) { return FMT(fg, bg); };

	/* * * * * * * * * * * * * * * * * * * * */

	inline std::ostream & operator<<(std::ostream & out, const FG::Color & value)
	{
		Console::setTextAttribute((uint16_t)(value));
		return out;
	};

	inline std::ostream & operator<<(std::ostream & out, const BG::Color & value)
	{
		Console::setTextAttribute((uint16_t)(value));
		return out;
	};

	inline std::ostream & operator<<(std::ostream & out, const FMT & value)
	{
		Console::setTextAttribute((uint16_t)(value.fg) | (uint16_t)(value.bg));
		return out;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONSOLE_HPP_