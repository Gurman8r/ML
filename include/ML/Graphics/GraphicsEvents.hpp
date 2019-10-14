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

			EV_OpenGLError, // OpenGL Error
			EV_ShaderError, // Shader Error

			MAX_GRAPHICS_EVENT
		};

		static_assert(MAX_GRAPHICS_EVENT < (MIN_GRAPHICS_EVENT + Event::MAX_LIBRARY_EVENTS),
			"too many library event types specified in " __FILE__
		);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct OpenGLErrorEvent final : public I_Event<GraphicsEvents::EV_OpenGLError>
	{
		C_String file;
		uint32_t line;
		C_String expr;
		uint32_t code;
		constexpr OpenGLErrorEvent(C_String file, uint32_t line, C_String expr, uint32_t code)
			: file { file }
			, line { line }
			, expr { expr }
			, code { code }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ShaderErrorEvent final : public I_Event<GraphicsEvents::EV_ShaderError>
	{
		const uint32_t obj;
		const uint32_t type;
		const C_String error;
		constexpr ShaderErrorEvent(uint32_t obj, uint32_t type, C_String error)
			: obj	{ obj }
			, type	{ type }
			, error { error } 
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_