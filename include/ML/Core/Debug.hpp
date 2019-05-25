#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <ML/Core/String.hpp>

#define ML_WARNING -1 // -1
#define ML_FAILURE	0 //  0
#define ML_SUCCESS	1 // +1

#define ML_MSG_LOG "LOG"
#define ML_MSG_WRN "WRN"
#define ML_MSG_ERR "ERR"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Debug final
	{
	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static void		fatal(const String & message);
		static int32_t	pause(int32_t exitCode);
		static bool		setTextAttrib(const uint16_t value);
		static int32_t	system(CString cmd);
		static int32_t	system(CString cmd, std::ostream & out);

	public:
		static int32_t logWarning(const String & message); // - 1 true
		static int32_t logError(const String & message);   //   0 false
		static int32_t log(const String & message);		   // + 1 true

	public:
		template <
			class T,
			class ... Args
		> inline static int32_t logWarning(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::logWarning(fmt.format(arg0, (args)...));
		}

		template <
			class T,
			class ... Args
		> inline static int32_t logError(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::logError(fmt.format(arg0, (args)...));
		}

		template <
			class T, 
			class ... Args
		> inline static int32_t log(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::log(fmt.format(arg0, (args)...));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FG final
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
			White		= Gray | DarkRed | DarkGreen | DarkBlue
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct BG final
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
			White		= DarkGray | DarkRed | DarkGreen | DarkBlue
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct FMT final
	{
		FG::Color fg;
		BG::Color bg;

		constexpr FMT(const FG::Color fg, const BG::Color bg)
			: fg(fg)
			, bg(bg)
		{
		}

		constexpr FMT() 
			: FMT { FG::Normal, BG::Black }
		{
		}

		constexpr FMT(const FG::Color fg) 
			: FMT { fg, BG::Black }
		{
		}

		constexpr FMT(const BG::Color bg) 
			: FMT { FG::Normal, bg }
		{
		}

		constexpr FMT(const FMT & copy) 
			: FMT { copy.fg, copy.bg } 
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr FMT operator|(const BG::Color & bg, const FG::Color & fg)
	{
		return { fg, bg };
	};

	constexpr FMT operator|(const FG::Color & fg, const BG::Color & bg)
	{
		return { fg, bg };
	};

	inline std::ostream & operator<<(std::ostream & out, const FG::Color & value)
	{
		Debug::setTextAttrib((uint16_t)value);
		return out;
	}

	inline std::ostream & operator<<(std::ostream & out, const BG::Color & value)
	{
		Debug::setTextAttrib((uint16_t)value);
		return out;
	}

	inline std::ostream & operator<<(std::ostream & out, const FMT & value)
	{
		Debug::setTextAttrib((uint16_t)value.fg | (uint16_t)value.bg);
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_