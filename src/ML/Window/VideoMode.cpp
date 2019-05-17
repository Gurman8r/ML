#include <ML/Window/VideoMode.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VideoMode::VideoMode()
		: VideoMode(vec2u::Zero, 0)
	{
	}

	VideoMode::VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel)
		: VideoMode(vec2u(width, height), bitsPerPixel)
	{
	}

	VideoMode::VideoMode(const vec2u & size, uint32_t bitsPerPixel)
		: size(size)
		, bitsPerPixel(bitsPerPixel)
	{
	}

	VideoMode::VideoMode(const VideoMode & copy)
		: VideoMode(copy.size, copy.bitsPerPixel)
	{
	}

	VideoMode::~VideoMode()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const VideoMode & VideoMode::desktop()
	{
		static VideoMode temp;
		static bool checked = true;
		if (checked)
		{	checked = false;
#ifdef ML_SYSTEM_WINDOWS
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);
			temp = VideoMode(
				win32Mode.dmPelsWidth,
				win32Mode.dmPelsHeight,
				win32Mode.dmBitsPerPel);
#else
			temp = VideoMode();
#endif
		}
		return temp;
	}

	const List<VideoMode> & VideoMode::resolutions()
	{
		static List<VideoMode> temp;
		static bool check = true;
		if (check)
		{	check = false;
#ifdef ML_SYSTEM_WINDOWS
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			for (int32_t count = 0; EnumDisplaySettings(NULL, count, &win32Mode); ++count)
			{
				VideoMode mode(
					win32Mode.dmPelsWidth,
					win32Mode.dmPelsHeight,
					win32Mode.dmBitsPerPel);

				if (std::find(temp.begin(), temp.end(), mode) == temp.end())
				{
					temp.push_back(mode);
				}
			}
#else
			temp = List<VideoMode>();
#endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void VideoMode::serialize(std::ostream & out) const
	{
		out << size << " " << bitsPerPixel;
	}

	void VideoMode::deserialize(std::istream & in)
	{
		in >> size >> bitsPerPixel;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool VideoMode::equals(const VideoMode & value) const
	{
		return 
			(size == value.size) && 
			(bitsPerPixel == value.bitsPerPixel);
	}

	bool VideoMode::lessThan(const VideoMode & value) const
	{
		return
			(size < value.size) && 
			(bitsPerPixel < value.bitsPerPixel);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}