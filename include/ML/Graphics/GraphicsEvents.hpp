#ifndef _ML_GRAPHICS_EVENTS_HPP_
#define _ML_GRAPHICS_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Shader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class GraphicsEvents
	{
		MIN_GRAPHICS_EVENT = Event::EV_GRAPHICS,

		EV_OpenGLError, // OpenGL Error
		EV_ShaderError, // Shader Error

		MAX_GRAPHICS_EVENT
	};

	static_assert(
		(int32_t)GraphicsEvents::MAX_GRAPHICS_EVENT <
		(int32_t)GraphicsEvents::MIN_GRAPHICS_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
		);

	template <GraphicsEvents ID> struct GraphicsEvent : public T_Event<GraphicsEvents, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct OpenGLErrorEvent final : public GraphicsEvent<GraphicsEvents::EV_OpenGLError>
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

	struct ShaderErrorEvent final : public GraphicsEvent<GraphicsEvents::EV_ShaderError>
	{
		const Shader * obj;
		uint32_t type;
		C_String error;
		constexpr ShaderErrorEvent(const Shader * obj, uint32_t type, C_String error)
			: obj	{ obj }
			, type	{ type }
			, error { error } 
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_