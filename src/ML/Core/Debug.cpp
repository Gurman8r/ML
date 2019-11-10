#include <ML/Core/Debug.hpp>
#include <ML/Core/Array.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <shellapi.h>
# endif

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

	int32_t Debug::clear()
	{
# if ML_DEBUG
#	ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#	else
		return std::system("clear");
#	endif
# else
		return 0;
# endif
	}

	int32_t Debug::pause(int32_t exitCode)
	{
# if ML_DEBUG
#	ifdef ML_SYSTEM_WINDOWS
		std::system("pause");
#	else
		cin.get();
#	endif
# endif
		return exitCode;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Debug::execute(const String & cmd)
	{
		return execute(cmd, String());
	}

	void * Debug::execute(const String & cmd, const String & file)
	{
		return execute(cmd, file, String());
	}

	void * Debug::execute(const String & cmd, const String & file, const String & args)
	{
		return execute(cmd, file, args, String());
	}

	void * Debug::execute(const String & cmd, const String & file, const String & args, const String & path)
	{
		return execute(cmd, file, args, path, 5); // SW_SHOW
	}

	void * Debug::execute(const String & cmd, const String & file, const String & args, const String & path, int32_t flags)
	{
#if defined(ML_SYSTEM_WINDOWS)
		return ShellExecuteA(
			GetDesktopWindow(), cmd.c_str(), file.c_str(), args.c_str(), path.c_str(), flags
		);
#else
		return nullptr;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}