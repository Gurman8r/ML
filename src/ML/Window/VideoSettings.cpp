#include <ML/Window/VideoSettings.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	VideoSettings::VideoSettings()
		: VideoSettings(vec2u::Zero, 0)
	{
	}

	VideoSettings::VideoSettings(const uint32_t width, const uint32_t height, const uint32_t colorDepth)
		: resolution(width, height)
		, colorDepth(colorDepth)
	{
	}

	VideoSettings::VideoSettings(const vec2u & resolution, uint32_t colorDepth)
		: resolution(resolution)
		, colorDepth(colorDepth)
	{
	}

	VideoSettings::VideoSettings(const VideoSettings & copy)
		: resolution(copy.resolution)
		, colorDepth(copy.colorDepth)
	{
	}

	VideoSettings::~VideoSettings()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const VideoSettings & VideoSettings::desktop()
	{
		static VideoSettings temp;
		static bool checked = true;
		if (checked)
		{	checked = false;
# if defined(ML_SYSTEM_WINDOWS)
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &win32Mode);
			temp = VideoSettings(
				win32Mode.dmPelsWidth,
				win32Mode.dmPelsHeight,
				win32Mode.dmBitsPerPel);
# else
			temp = VideoSettings();
# endif
		}
		return temp;
	}

	const List<VideoSettings> & VideoSettings::resolutions()
	{
		static List<VideoSettings> temp;
		static bool check = true;
		if (check)
		{	check = false;
# if defined(ML_SYSTEM_WINDOWS)
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			for (int32_t count = 0; EnumDisplaySettings(nullptr, count, &win32Mode); ++count)
			{
				VideoSettings mode(
					win32Mode.dmPelsWidth,
					win32Mode.dmPelsHeight,
					win32Mode.dmBitsPerPel);

				if (std::find(temp.begin(), temp.end(), mode) == temp.end())
				{
					temp.push_back(mode);
				}
			}
# else
			temp = List<VideoSettings>();
# endif
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void VideoSettings::serialize(OStream & out) const
	{
		out << resolution << " " << colorDepth;
	}

	bool VideoSettings::equals(const VideoSettings & value) const
	{
		return 
			(resolution == value.resolution) && 
			(colorDepth == value.colorDepth);
	}

	bool VideoSettings::lessThan(const VideoSettings & value) const
	{
		return
			(resolution < value.resolution) && 
			(colorDepth < value.colorDepth);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}