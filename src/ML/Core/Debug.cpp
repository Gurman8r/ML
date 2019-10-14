#include <ML/Core/Debug.hpp>
#include <ML/Core/Array.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::ostream & FMT::operator()(std::ostream & out) const
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HANDLE handle { GetStdHandle(STD_OUTPUT_HANDLE) })
		{
			SetConsoleTextAttribute(handle, (*(*this)));
		}
#else 
		// Do the thing
#endif
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Debug::exit(int32_t exitCode)
	{
		std::exit(exitCode);
	}

	void Debug::fatal(const String & message)
	{
		Logger()(cerr, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
		std::abort();
	}

	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#else
		return std::system("clear");
#endif
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		std::system("pause");
#else
		cin.get();
#endif
		return exitCode;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}