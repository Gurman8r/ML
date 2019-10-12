/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>

# if defined(_DEBUG)
#	define CONF "Debug"
# else
#	define CONF "Release"
# endif

# if defined(_WIN32) || defined(WIN32)
#	include <Windows.h>
#	if defined(_WIN64) || defined(WIN64)
#		define ARCH "x64"
#	else
#		define ARCH "x86"
#	endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char ** argv)	
{
	using namespace std;

	system("cls");

	cout << "CWD: " << filesystem::current_path().string() << endl;

	if (argc == 1)
	{
		constexpr string_view path { ".\\bin\\" CONF "\\" ARCH "\\" };
		struct stat info;
		if ((stat(path.data(), &info) == 0) && (info.st_mode & S_IFDIR))
		{
			filesystem::current_path(path);

			constexpr string_view name { "ML_Launcher_" CONF "_" ARCH ".exe" };
			if ((bool)ifstream { name })
			{
				cout << name << endl;

				ShellExecuteA(GetDesktopWindow(), "open", name.data(), "", "", SW_SHOW);

				return EXIT_SUCCESS;
			}
			else
			{
				cout << "File not found: \'" << name << "\'" << endl;
			}
		}
		else
		{
			cout << "Directory not found: \'" << path << "\'" << endl;
		}
		
	}
	system("pause");
	return EXIT_FAILURE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */