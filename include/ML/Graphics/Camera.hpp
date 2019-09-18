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
		Camera(ClearFlags clearFlags, Projection projection);
		Camera(const Camera & copy);
		~Camera();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Camera & apply() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & setClearFlags(ClearFlags value);
		Camera & setBackground(const vec4 & value);
		Camera & setFieldOfView(float_t value);
		Camera & setProjection(Projection value);
		Camera & setClipping(float_t zNear, float_t zFar);
		Camera & setClipFar(float_t value);
		Camera & setClipNear(float_t value);
		Camera & setViewport(const vec2i & pos, const vec2i & size);
		Camera & setViewport(const vec2i & value);
		Camera & setViewport(const IntRect & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto clearFlags()	const -> const ClearFlags & { return m_clearFlags; }
		inline auto background()	const -> const vec4 &		{ return m_background; }
		inline auto projection()	const -> const Projection & { return m_projection; }
		inline auto fieldOfView()	const -> const float_t &	{ return m_fieldOfView; }
		inline auto clipNear()		const -> const float_t &	{ return m_clipNear; }
		inline auto clipFar()		const -> const float_t &	{ return m_clipFar; }
		inline auto viewport()		const -> const IntRect &	{ return m_viewport; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ClearFlags	m_clearFlags;	// 
		vec4		m_background;	// 
		Projection	m_projection;	// 
		float_t		m_fieldOfView;	// 
		float_t		m_clipNear;		// 
		float_t		m_clipFar;		// 
		IntRect		m_viewport;		// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_