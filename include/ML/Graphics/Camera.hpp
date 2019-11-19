#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Camera final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum ClearFlags { SolidColor, DepthOnly, DontClear };

		enum Projection { Perspective, Orthographic };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera();
		Camera(ClearFlags clearFlags, Projection projection);
		Camera(Camera const & copy);
		~Camera();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera const & applyClear() const;
		Camera const & applyViewport() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Camera & setEnabled(bool value);
		Camera & setClearFlags(ClearFlags value);
		Camera & setBackground(vec4 const & value);
		Camera & setFieldOfView(float_t value);
		Camera & setProjection(Projection value);
		Camera & setPosition(vec3 const & value);
		Camera & setDirection(vec3 const & value);
		Camera & setClipping(float_t zNear, float_t zFar);
		Camera & setClipFar(float_t value);
		Camera & setClipNear(float_t value);
		Camera & setViewport(vec2i const & pos, vec2i const & size);
		Camera & setViewport(vec2i const & value);
		Camera & setViewport(IntRect const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()		const -> bool const &		{ return m_enabled; }
		inline auto clearFlags()	const -> ClearFlags const & { return m_clearFlags; }
		inline auto background()	const -> vec4 const &		{ return m_background; }
		inline auto projection()	const -> Projection const & { return m_projection; }
		inline auto position()		const -> vec3 const &		{ return m_position; }
		inline auto direction()		const -> vec3 const &		{ return m_direction; }
		inline auto fieldOfView()	const -> float_t const &	{ return m_fieldOfView; }
		inline auto clipNear()		const -> float_t const &	{ return m_clipNear; }
		inline auto clipFar()		const -> float_t const &	{ return m_clipFar; }
		inline auto viewport()		const -> IntRect const &	{ return m_viewport; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline Camera * mainCamera()
		{
			return mainCamera(nullptr, false);
		}

		static inline Camera * mainCamera(Camera * value, bool overwrite = true)
		{
			static Camera * temp { nullptr };
			return (overwrite ? (temp = value) : temp);
		}

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Camera::ClearFlags> final
	{
		inline auto operator()(String const & value) const
		{
			switch (util::to_lower(value).hash())
			{
			case Hash("solidcolor"): return Camera::SolidColor;
			case Hash("depthonly"): return Camera::DepthOnly;
			case Hash("dontclear"): return Camera::DontClear;
			}
			return (Camera::ClearFlags)0;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Camera::Projection> final
	{
		inline auto operator()(String const & value) const
		{
			switch (util::to_lower(value).hash())
			{
			case Hash("orthographic"): return Camera::Orthographic;
			case Hash("perspective"): return Camera::Perspective;
			}
			return (Camera::Projection)0;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_