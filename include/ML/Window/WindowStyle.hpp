#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	struct WindowStyle final : public I_NonNewable
	{
		bool fullscreen;
		bool resizable;
		bool visible;
		bool decorated;
		bool focused;
		bool autoIconify;
		bool floating;
		bool maximized;

		constexpr WindowStyle()
			: WindowStyle(false, true, true, true, true, true, false, false)
		{
		}

		constexpr WindowStyle(bool fullscreen, bool resizable, bool visible, bool decorated, bool focused, bool autoIconify, bool floating, bool maximized)
			: fullscreen	(fullscreen)
			, resizable		(resizable)
			, visible		(visible)
			, decorated		(decorated)
			, focused		(focused)
			, autoIconify	(autoIconify)
			, floating		(floating)
			, maximized		(maximized)
		{
		}

		constexpr WindowStyle(const WindowStyle & copy) : WindowStyle(
			copy.fullscreen,
			copy.resizable, 
			copy.visible, 
			copy.decorated, 
			copy.focused, 
			copy.autoIconify, 
			copy.floating, 
			copy.maximized)
		{
		}

		constexpr bool operator==(const WindowStyle & other) const
		{
			return
				this->fullscreen	== other.fullscreen		&&
				this->resizable		== other.resizable		&&
				this->visible		== other.visible		&&
				this->decorated		== other.decorated		&&
				this->focused		== other.focused		&&
				this->autoIconify	== other.autoIconify	&&
				this->floating		== other.floating		&&
				this->maximized		== other.maximized		;
		}

		constexpr bool operator!=(const WindowStyle & other) const
		{
			return !((*this) == other);
		}
	};
}

#endif // !_ML_WINDOW_STYLE_HPP_
