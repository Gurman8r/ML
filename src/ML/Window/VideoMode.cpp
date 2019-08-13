#include <ML/Window/VideoMode.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	VideoMode::VideoMode()
		: resolution	{ 0, 0 }
		, bitsPerPixel	{ 0 }
	{
	}

	VideoMode::VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel)
		: resolution	{ width, height }
		, bitsPerPixel	{ bitsPerPixel }
	{
	}

	VideoMode::VideoMode(const vec2u & resolution, uint32_t bitsPerPixel)
		: resolution	{ resolution }
		, bitsPerPixel	{ bitsPerPixel }
	{
	}

	VideoMode::VideoMode(const VideoMode & copy)
		: resolution	{ copy.resolution }
		, bitsPerPixel	{ copy.bitsPerPixel }
	{
	}

	VideoMode::~VideoMode() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const VideoMode & VideoMode::get_desktop_mode()
	{
		static VideoMode temp {};
		static bool check = false;
		if (!check && (check = true))
		{
#ifdef ML_SYSTEM_WINDOWS
			DEVMODE winMode;
			winMode.dmSize = sizeof(winMode);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &winMode);
			temp = VideoMode {
				winMode.dmPelsWidth, 
				winMode.dmPelsHeight,
				winMode.dmBitsPerPel 
			};
#else
			// do the thing
#endif
		}
		return temp;
	}

	const List<VideoMode> & VideoMode::get_fullscreen_modes()
	{
		static List<VideoMode> temp {};
		static bool check = false;
		if (!check && (check = true))
		{
#ifdef ML_SYSTEM_WINDOWS
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
			// do the thing
#endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}