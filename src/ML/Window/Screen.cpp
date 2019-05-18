#include <ML/Window/Screen.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Screen::Screen()
		: Screen(vec2u::Zero, 0)
	{
	}

	Screen::Screen(const uint32_t width, const uint32_t height)
		: Screen(width, height, 32)
	{
	}

	Screen::Screen(const vec2u & resolution)
		: Screen(resolution, 32)
	{
	}

	Screen::Screen(const uint32_t width, const uint32_t height, const uint32_t bitsPerPixel)
		: resolution(width, height)
		, bitsPerPixel(bitsPerPixel)
	{
	}

	Screen::Screen(const vec2u & resolution, uint32_t bitsPerPixel)
		: resolution(resolution)
		, bitsPerPixel(bitsPerPixel)
	{
	}

	Screen::Screen(const Screen & copy)
		: resolution(copy.resolution)
		, bitsPerPixel(copy.bitsPerPixel)
	{
	}

	Screen::~Screen()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Screen & Screen::desktop()
	{
		static Screen temp;
		static bool checked = true;
		if (checked)
		{	checked = false;
# if defined(ML_SYSTEM_WINDOWS)
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);
			temp = Screen(
				win32Mode.dmPelsWidth,
				win32Mode.dmPelsHeight,
				win32Mode.dmBitsPerPel);
# else
			temp = Screen();
# endif
		}
		return temp;
	}

	const List<Screen> & Screen::resolutions()
	{
		static List<Screen> temp;
		static bool check = true;
		if (check)
		{	check = false;
# if defined(ML_SYSTEM_WINDOWS)
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			for (int32_t count = 0; EnumDisplaySettings(NULL, count, &win32Mode); ++count)
			{
				Screen mode(
					win32Mode.dmPelsWidth,
					win32Mode.dmPelsHeight,
					win32Mode.dmBitsPerPel);

				if (std::find(temp.begin(), temp.end(), mode) == temp.end())
				{
					temp.push_back(mode);
				}
			}
# else
			temp = List<Screen>();
# endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Screen::serialize(std::ostream & out) const
	{
		out << resolution << " " << bitsPerPixel;
	}

	void Screen::deserialize(std::istream & in)
	{
		in >> resolution >> bitsPerPixel;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Screen::equals(const Screen & value) const
	{
		return 
			(resolution == value.resolution) && 
			(bitsPerPixel == value.bitsPerPixel);
	}

	bool Screen::lessThan(const Screen & value) const
	{
		return
			(resolution < value.resolution) && 
			(bitsPerPixel < value.bitsPerPixel);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}