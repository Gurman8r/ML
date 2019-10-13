#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StringUtility.hpp>

// Log Codes
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_WARNING -1 // -1
#define ML_FAILURE	0 //  0
#define ML_SUCCESS	1 // +1

// Log Prefixes
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_MSG_LOG "info"
#define ML_MSG_WRN "warning"
#define ML_MSG_ERR "error"

// Breakpoint
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if not ML_DEBUG
#	define ML_BREAKPOINT
# else
#	if defined(ML_CC_MSC)
#		define ML_BREAKPOINT __debugbreak()
#	else
#		define ML_BREAKPOINT raise(SIGTRAP)
#	endif
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FMT final
	{
		FG fg;
		BG bg;

		constexpr FMT()							: FMT { FG::Normal, BG::Black } {}
		constexpr FMT(const FG fg)				: FMT { fg,			BG::Black }	{}
		constexpr FMT(const BG bg)				: FMT { FG::Normal, bg }		{}
		constexpr FMT(const FMT & copy)			: FMT { copy.fg,	copy.bg }	{}
		constexpr FMT(const FG fg, const BG bg) : fg  { fg },		bg { bg }	{}

		constexpr uint16_t operator*() const
		{
			// Foreground and Background
			return (((this->fg != FG::None) && (this->bg != BG::None))
				? ((uint16_t)this->fg | (uint16_t)this->bg) 
				// Foreground Only
				: ((this->fg != FG::None)
					? ((uint16_t)this->fg) 
					// Background Only
					: ((this->bg != BG::None)
						? ((uint16_t)this->bg) 
						// Default
						: ((uint16_t)FG::Normal | (uint16_t)BG::Black) 
						)));
		}

		std::ostream & operator()(std::ostream & out) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr FMT operator|(const BG & bg, const FG & fg)	{ return FMT { fg, bg }; }
	constexpr FMT operator|(const FG & fg, const BG & bg)	{ return FMT { fg, bg }; }

	inline ML_SERIALIZE(std::ostream & out, const FMT & value)	{ return value(out); }
	inline ML_SERIALIZE(std::ostream & out, const FG & value)	{ return out << FMT { value }; }
	inline ML_SERIALIZE(std::ostream & out, const BG & value)	{ return out << FMT { value }; }

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Debug final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Debug() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void		exit(int32_t exitCode);
		static void		fatal(const String & message);
		static int32_t	clear();
		static int32_t	pause(int32_t exitCode);
		static int32_t	system(C_String cmd);
		static int32_t	system(C_String cmd, std::ostream & out);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Logger final
		{
			Logger() = default;
			inline int32_t operator()(
				std::ostream &		out,
				const int32_t	exitCode,
				const FMT &		color,
				const String &	prefix,
				const String &	message)
			{
				out << FMT()
					<< FG::White << "[ " << color << prefix << FG::White << " ]"
					<< FMT() << " " << message
					<< endl;
				return exitCode;
			};
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline int32_t log(const String & message)
		{
			return Logger()(cout, ML_SUCCESS, FG::Green, ML_MSG_LOG, message);
		}

		static inline int32_t logError(const String & message)
		{
			return Logger()(cout, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
		}

		static inline int32_t logWarning(const String & message)
		{
			return Logger()(cout, ML_WARNING, FG::Yellow, ML_MSG_WRN, message);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, 
			class ... Args
		> static inline int32_t log(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::log(fmt.format(arg0, std::forward<Args>(args)...));
		}

		template <
			class T,
			class ... Args
		> static inline int32_t logError(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::logError(fmt.format(arg0, std::forward<Args>(args)...));
		}

		template <
			class T,
			class ... Args
		> static inline int32_t logWarning(const String & fmt, const T & arg0, Args && ... args)
		{
			return Debug::logWarning(fmt.format(arg0, std::forward<Args>(args)...));
		}
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_