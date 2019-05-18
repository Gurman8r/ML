#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <ML/Window/Window.hpp>
#include <ML/Graphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderWindow
		: public Window
		, public RenderTarget
	{
	public:
		RenderWindow(EventSystem & eventSystem);
		virtual ~RenderWindow();

	public:
		bool setup() override;

		virtual void onEvent(const IEvent * ev) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_WINDOW_HPP_