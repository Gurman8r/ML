#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <ML/Window/Window.hpp>
#include <ML/Graphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderWindow
		: public Window
		, public RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit RenderWindow(EventSystem & eventSystem);
		
		virtual ~RenderWindow();

		bool setup() override;

		virtual void onEvent(const Event * ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_WINDOW_HPP_