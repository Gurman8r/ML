#include <ML/Window/VideoMode.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VideoMode::VideoMode()
		: VideoMode({ 0, 0 }, 0)
	{
	}

	VideoMode::VideoMode(uint32_t width, uint32_t height, uint32_t colorDepth)
		: resolution({ width, height })
		, colorDepth(colorDepth)
	{
	}

	VideoMode::VideoMode(const vec2u & resolution, uint32_t colorDepth)
		: resolution(resolution)
		, colorDepth(colorDepth)
	{
	}

	VideoMode::VideoMode(const VideoMode & copy)
		: resolution(copy.resolution)
		, colorDepth(copy.colorDepth)
	{
	}

	VideoMode::~VideoMode()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const VideoMode & VideoMode::get_desktop()
	{
		static VideoMode temp;
		static bool checked = true;
		if (checked)
		{	checked = false;
#if defined(ML_SYSTEM_WINDOWS)
			DEVMODE winMode;
			winMode.dmSize = sizeof(winMode);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &winMode);
			temp = VideoMode {
				winMode.dmPelsWidth, 
				winMode.dmPelsHeight,
				winMode.dmBitsPerPel 
			};
#else
			temp = VideoMode {};
#endif
		}
		return temp;
	}

	const List<VideoMode> & VideoMode::get_modes()
	{
		static List<VideoMode> temp;
		static bool check = true;
		if (check)
		{	check = false;
#if defined(ML_SYSTEM_WINDOWS)
			DEVMODE winMode;
			winMode.dmSize = sizeof(winMode);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &winMode); ++count)
			{
				VideoMode videoMode { 
					winMode.dmPelsWidth, 
					winMode.dmPelsHeight, 
					winMode.dmBitsPerPel
				};

				if (std::find(temp.begin(), temp.end(), videoMode) == temp.end())
				{
					temp.push_back(videoMode);
				}
			}
#else
			temp = List<VideoMode>{};
#endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}