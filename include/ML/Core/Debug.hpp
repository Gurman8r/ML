#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <ML/Core/Console.hpp>
#include <ML/Core/String.hpp>

#define ML_WARNING -1 // -1
#define ML_FAILURE	0 //  0
#define ML_SUCCESS	1 // +1

#define ML_MSG_LOG "LOG"
#define ML_MSG_WRN "WRN"
#define ML_MSG_ERR "ERR"

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static void		fatal(const String & message);
		static int32_t	pause(int32_t exitCode);
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
	
}

#endif // !_ML_DEBUG_HPP_