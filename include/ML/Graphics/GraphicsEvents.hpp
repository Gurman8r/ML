#ifndef _ML_GRAPHICS_EVENTS_HPP_
#define _ML_GRAPHICS_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct GraphicsEvents final
	{
		GraphicsEvents() = delete;

		enum : int32_t
		{
			MIN_GRAPHICS_EVENT = Event::EV_GRAPHICS,

			EV_OpenGlError, // OpenGL Error

			MAX_GRAPHICS_EVENT
		};

		static_assert(MAX_GRAPHICS_EVENT < (MIN_GRAPHICS_EVENT + Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct OpenGlErrorEvent final : public I_Event<GraphicsEvents::EV_OpenGlError>
	{
		C_String file;
		uint32_t line;
		C_String expr;
		uint32_t code;
		constexpr OpenGlErrorEvent(C_String file, uint32_t line, C_String expr, uint32_t code)
			: file { file }
			, line { line }
			, expr { expr }
			, code { code }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_