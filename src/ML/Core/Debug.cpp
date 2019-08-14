#include <ML/Core/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ostream & FMT::operator()(Ostream & out) const
	{
#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (*(*this)));
#else 
		// Do the thing
#endif
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Debug::exit(int32_t exitCode)
	{
		return std::exit(exitCode);
	}

	void Debug::fatal(const String & message)
	{
		Logger()(cerr, ML_FAILURE, FG::Red, ML_MSG_ERR, message);
		return std::abort();
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

	int32_t Debug::system(C_String cmd)
	{
		return system(cmd, cout);
	}

	int32_t Debug::system(C_String cmd, Ostream & out)
	{
		if (auto file = std::shared_ptr<std::FILE>(popen(cmd, "r"), pclose))
		{
			Array<char, 128> buffer;
			while (!std::feof(file.get()))
			{
				if (std::fgets(buffer.data(), 128, file.get()) != nullptr)
				{
					out << String(buffer.data());
				}
			}
			return EXIT_SUCCESS;
		}
		return EXIT_FAILURE;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}