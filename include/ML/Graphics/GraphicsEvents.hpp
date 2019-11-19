#ifndef _ML_GRAPHICS_EVENTS_HPP_
#define _ML_GRAPHICS_EVENTS_HPP_

#include <ML/Core/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Shader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum class GraphicsEventType
	{
		MIN_GRAPHICS_EVENT = Event::EV_GRAPHICS,

		EV_OpenGLError, // OpenGL Error
		EV_ShaderError, // Shader Error

		MAX_GRAPHICS_EVENT
	};

	static_assert(
		(int32_t)GraphicsEventType::MAX_GRAPHICS_EVENT <
		(int32_t)GraphicsEventType::MIN_GRAPHICS_EVENT + Event::MAX_LIBRARY_EVENTS,
		"too many library event types specified in " __FILE__
	);

	template <GraphicsEventType ID> struct GraphicsEvent : public T_Event<GraphicsEventType, ID>
	{
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct OpenGLErrorEvent final : public GraphicsEvent<GraphicsEventType::EV_OpenGLError>
	{
		const C_String file;
		const uint32_t line;
		const C_String expr;
		const uint32_t code;
		constexpr OpenGLErrorEvent(C_String file, uint32_t line, C_String expr, uint32_t code)
			: file { file }
			, line { line }
			, expr { expr }
			, code { code }
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ShaderErrorEvent final : public GraphicsEvent<GraphicsEventType::EV_ShaderError>
	{
		Shader const * shader;
		uint32_t type;
		C_String error;
		constexpr ShaderErrorEvent(Shader const * shader, uint32_t type, C_String error)
			: shader{ shader }
			, type	{ type }
			, error { error } 
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_GRAPHICS_EVENTS_HPP_