#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	struct ML_WINDOW_API StyleSettings
		: public ITrackable
	{
		bool resizable;
		bool visible;
		bool decorated;
		bool focused;
		bool autoIconify;
		bool floating;
		bool maximized;

		StyleSettings();
		StyleSettings(bool resizable, bool visible, bool decorated, bool focused, bool autoIconify, bool floating, bool maximized);
		StyleSettings(const StyleSettings & copy);
	};
}

#endif // !_ML_WINDOW_STYLE_HPP_
