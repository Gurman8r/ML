#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Camera final : public Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ClearFlags { SolidColor, DepthOnly, DontClear };

		enum Projection { Perspective, Orthographic };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera();
		Camera(ClearFlags clearFlags, Projection projection);
		Camera(const Camera & copy);
		~Camera();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Camera & apply() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & setEnabled(bool value);
		Camera & setClearFlags(ClearFlags value);
		Camera & setBackground(const vec4 & value);
		Camera & setFieldOfView(float_t value);
		Camera & setProjection(Projection value);
		Camera & setPosition(const vec3 & value);
		Camera & setDirection(const vec3 & value);
		Camera & setClipping(float_t zNear, float_t zFar);
		Camera & setClipFar(float_t value);
		Camera & setClipNear(float_t value);
		Camera & setViewport(const vec2i & pos, const vec2i & size);
		Camera & setViewport(const vec2i & value);
		Camera & setViewport(const IntRect & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()		const -> const bool &		{ return m_enabled; }
		inline auto clearFlags()	const -> const ClearFlags & { return m_clearFlags; }
		inline auto background()	const -> const vec4 &		{ return m_background; }
		inline auto projection()	const -> const Projection & { return m_projection; }
		inline auto position()		const -> const vec3 &		{ return m_position; }
		inline auto direction()		const -> const vec3 &		{ return m_direction; }
		inline auto fieldOfView()	const -> const float_t &	{ return m_fieldOfView; }
		inline auto clipNear()		const -> const float_t &	{ return m_clipNear; }
		inline auto clipFar()		const -> const float_t &	{ return m_clipFar; }
		inline auto viewport()		const -> const IntRect &	{ return m_viewport; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return enabled(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline Camera * mainCamera() { return s_mainCamera; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool		m_enabled;		//
		ClearFlags	m_clearFlags;	// 
		Color		m_background;	// 
		Projection	m_projection;	// 
		vec3		m_position;		// 
		vec3		m_direction;	// 
		float_t		m_fieldOfView;	// 
		float_t		m_clipNear;		// 
		float_t		m_clipFar;		// 
		IntRect		m_viewport;		// 

		static Camera * s_mainCamera;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Camera::ClearFlags> final
	{
		inline auto operator()(const String & value) const
		{
			switch (util::to_lower(value).hash())
			{
			case Hash("solidcolor"): return Camera::SolidColor;
			case Hash("depthonly"): return Camera::DepthOnly;
			case Hash("dontclear"): return Camera::DontClear;
			default: return (Camera::ClearFlags)0;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Camera::Projection> final
	{
		inline Camera::Projection operator()(const String & value) const
		{
			switch (util::to_lower(value).hash())
			{
			case Hash("orthographic"): return Camera::Orthographic;
			case Hash("perspective"): return Camera::Perspective;
			default: return (Camera::Projection)0;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_