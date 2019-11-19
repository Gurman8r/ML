#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <ML/Window/Window.hpp>
#include <ML/Graphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderWindow : public Window, public RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderWindow();
		
		virtual ~RenderWindow();

		virtual bool create(
			String const & title,
			VideoMode const & videoMode,
			WindowStyle const & style,
			ContextSettings const & context
		) override;

		virtual void onEvent(Event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_WINDOW_HPP_