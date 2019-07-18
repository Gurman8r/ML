#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <ML/Core/String.hpp>
#include <ML/Core/FMT.hpp>

// Log Codes
#define ML_WARNING -1 // -1
#define ML_FAILURE	0 //  0
#define ML_SUCCESS	1 // +1

// Log Prefixes
#define ML_MSG_LOG "LOG"
#define ML_MSG_WRN "WRN"
#define ML_MSG_ERR "ERR"

// Breakpoint
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

	struct Debug final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Logger final
		{
			Logger() = default;
			inline int32_t operator()(
				Ostream &		out,
				int32_t			exitCode,
				const FMT &		color,
				const String &	prefix,
				const String &	message)
			{
				out << FMT()
					<< FG::White << "[ "
					<< color << prefix
					<< FG::White << " ]"
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

		static inline void exit(int32_t exitCode)
		{
			return std::exit(exitCode);
		}

		static inline void fatal(const String & message)
		{
			Logger()(cerr, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
			return std::abort();
		}

		static inline int32_t clear()
		{
#ifdef ML_SYSTEM_WINDOWS
			return std::system("cls");
#else
			return std::system("clear");
#endif
		}

		static inline int32_t pause(int32_t exitCode)
		{
#ifdef ML_SYSTEM_WINDOWS
			std::system("pause");
#else
			cin.get();
#endif
			return exitCode;
		}

		static inline int32_t system(C_String cmd)
		{
			return system(cmd, cout);
		}

		static inline int32_t system(C_String cmd, Ostream & out)
		{
			std::array<char, 128> buffer;
			std::shared_ptr<std::FILE> file(_popen(cmd, "r"), _pclose);
			if (file)
			{
				while (!std::feof(file.get()))
				{
					if (fgets(buffer.data(), 128, file.get()) != nullptr)
					{
						out << String(buffer.data());
					}
				}
				return EXIT_SUCCESS;
			}
			return EXIT_FAILURE;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DEBUG_HPP_