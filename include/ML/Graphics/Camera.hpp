#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Camera final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ClearFlags { Skybox, SolidColor, DepthOnly, DontClear };

		enum Projection { Perspective, Orthographic };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera();
		Camera(const Camera & copy);
		~Camera();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & clear();
		Camera & clear(const vec4 & color);
		Camera & setViewport(const vec2i & pos, const vec2i & size);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto clearFlags()	const -> const ClearFlags & { return m_clearFlags; }
		inline auto background()	const -> const vec4 &		{ return m_background; }
		inline auto projection()	const -> const Projection & { return m_projection; }
		inline auto fieldOfView()	const -> const float_t &	{ return m_fieldOfView; }
		inline auto clipNear()		const -> const float_t &	{ return m_near; }
		inline auto clipFar()		const -> const float_t &	{ return m_far; }
		inline auto viewport()		const -> const FloatRect &	{ return m_viewport; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ClearFlags	m_clearFlags;	// 
		vec4		m_background;	// 
		Projection	m_projection;	// 
		float_t		m_fieldOfView;	// 
		float_t		m_near;			// 
		float_t		m_far;			// 
		FloatRect	m_viewport;		// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_