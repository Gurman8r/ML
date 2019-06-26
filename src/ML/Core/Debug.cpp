#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	inline static const int32_t ml_Log(
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
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#else
		return std::system("clear");
#endif
	}

	void Debug::exit(int32_t exitCode)
	{
		return std::exit(exitCode);
	}

	void Debug::fatal(const String & message)
	{
		ml_Log(cerr, ML_FAILURE, FG::Red, ML_MSG_ERR, message);

		return std::abort();
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

	int32_t	Debug::system(C_String cmd)
	{
		return system(cmd, cout);
	}

	int32_t Debug::system(C_String cmd, Ostream & out)
	{
		std::array<char, 128> buffer;
		std::shared_ptr<FILE> file(_popen(cmd, "r"), _pclose);
		if (file)
		{
			while (!feof(file.get()))
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

	/* * * * * * * * * * * * * * * * * * * * */

	int32_t Debug::logWarning(const String & message)
	{
		return ml_Log(cout, ML_WARNING, FG::Yellow, ML_MSG_WRN, message);
	}

	int32_t Debug::logError(const String & message)
	{
		return ml_Log(cout, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
	}
	
	int32_t Debug::log(const String & message)
	{
		return ml_Log(cout, ML_SUCCESS, FG::Green, ML_MSG_LOG, message);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}