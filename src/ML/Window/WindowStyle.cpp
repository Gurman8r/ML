#include <ML/Window/StyleSettings.hpp>

namespace ml
{
	StyleSettings::StyleSettings()
		: resizable		(true)
		, visible		(true)
		, decorated		(true)
		, focused		(true)
		, autoIconify	(true)
		, floating		(false)
		, maximized		(false)
	{
	}

	StyleSettings::StyleSettings(
		bool resizable,
		bool visible,
		bool decorated,
		bool focused,
		bool autoIconify,
		bool floating,
		bool maximized)
		: resizable		(resizable)
		, visible		(visible)
		, decorated		(decorated)
		, focused		(focused)
		, autoIconify	(autoIconify)
		, floating		(floating)
		, maximized		(maximized)
	{
	}

	StyleSettings::StyleSettings(const StyleSettings & copy)
		: resizable		(copy.resizable)
		, visible		(copy.visible)
		, decorated		(copy.decorated)
		, focused		(copy.focused)
		, autoIconify	(copy.autoIconify)
		, floating		(copy.floating)
		, maximized		(copy.maximized)
	{
	}
}