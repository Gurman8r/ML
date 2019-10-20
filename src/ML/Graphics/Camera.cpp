#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	static inline mat4 look_at(const vec3 & eye, const vec3 & center, const vec3 & up)
	{
		const vec3 f = alg::normalize(center - eye);
		const vec3 s = alg::normalize(alg::cross(f, up));
		const vec3 u = alg::cross(s, f);
		mat4 m { 1 };
		m[0] = s[0];
		m[1] = u[0];
		m[2] = -f[0];
		m[4] = s[1];
		m[5] = u[1];
		m[6] = -f[1];
		m[8] = s[2];
		m[9] = u[2];
		m[10] = -f[2];
		m[12] = -alg::dot(s, eye);
		m[13] = -alg::dot(u, eye);
		m[14] = alg::dot(f, eye);
		return m;
	}

	static inline mat4 perspective(float_t fov, float_t aspect, float_t zNear, float_t zFar)
	{
		mat4 m { NULL };
		m[0 * 4 + 0] = 1.0f / (aspect * std::tan(fov / 2.0f));
		m[1 * 4 + 1] = 1.0f / std::tan(fov / 2.0f);
		m[2 * 4 + 3] = -1.0f;
		m[2 * 4 + 2] = -(zFar + zNear) / (zFar - zNear);
		m[3 * 4 + 2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return m;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera * Camera::s_mainCamera { nullptr };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: Camera { ClearFlags::SolidColor, Projection::Perspective }
	{
	}

	Camera::Camera(ClearFlags clearFlags, Projection projection)
		: m_enabled		{ true }
		, m_clearFlags	{ clearFlags }
		, m_background	{ Colors::black }
		, m_projection	{ projection }
		, m_position	{ 0, 0, 5 }
		, m_direction		{ 0, 0, -1 }
		, m_fieldOfView { 45.0f }
		, m_clipNear	{ 0.001f }
		, m_clipFar		{ 1000.0f }
		, m_viewport	{ { 0, 0 }, { 1920, 1080 } }
	{
		if (!s_mainCamera)
		{
			s_mainCamera = this;
		}
	}

	Camera::Camera(const Camera & copy)
		: m_enabled		{ copy.m_enabled }
		, m_clearFlags	{ copy.m_clearFlags	}
		, m_background	{ copy.m_background	}
		, m_projection	{ copy.m_projection	}
		, m_fieldOfView { copy.m_fieldOfView }
		, m_clipNear	{ copy.m_clipNear }
		, m_clipFar		{ copy.m_clipFar }
		, m_viewport	{ copy.m_viewport }
	{
	}
	
	Camera::~Camera() 
	{
		if (s_mainCamera && (s_mainCamera == this))
		{
			s_mainCamera = nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Camera & Camera::apply() const
	{
		return m_enabled ? applyClear().applyViewport() : (*this);
	}

	const Camera & Camera::applyClear() const
	{
		switch (m_clearFlags)
		{
		case Camera::SolidColor:
			ML_GL.clearColor(
				m_background[0], 
				m_background[1], 
				m_background[2],
				m_background[3]
			);
			ML_GL.clear(GL::ColorBufferBit | GL::DepthBufferBit);
			break;

		case Camera::DepthOnly:
			ML_GL.clear(GL::DepthBufferBit);
			break;

		case Camera::DontClear:
		default:
			break;
		}
		return (*this);
	}

	const Camera & Camera::applyViewport() const
	{
		ML_GL.viewport(
			m_viewport.position()[0],
			m_viewport.position()[1],
			m_viewport.size()[0],
			m_viewport.size()[1]
		);
		return (*this);

	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::setEnabled(bool value)
	{
		m_enabled = value;
		return (*this);
	}

	Camera & Camera::setClearFlags(ClearFlags value)
	{
		m_clearFlags = value;
		return (*this);
	}

	Camera & Camera::setBackground(const vec4 & value)
	{
		m_background = value;
		return (*this);
	}

	Camera & Camera::setFieldOfView(float_t value)
	{
		m_fieldOfView = value;
		return (*this);
	}

	Camera & Camera::setProjection(Projection value)
	{
		m_projection = value;
		return (*this);
	}

	Camera & Camera::setPosition(const vec3 & value)
	{
		m_position = value;
		return (*this);
	}

	Camera & Camera::setDirection(const vec3 & value)
	{
		m_direction = value;
		return (*this);
	}

	Camera & Camera::setClipping(float_t zNear, float_t zFar)
	{
		return setClipNear(zNear).setClipFar(zFar);
	}

	Camera & Camera::setClipFar(float_t value)
	{
		m_clipFar = value;
		return (*this);
	}

	Camera & Camera::setClipNear(float_t value)
	{
		m_clipNear = value;
		return (*this);
	}

	Camera & Camera::setViewport(const vec2i & pos, const vec2i & size)
	{
		return setViewport({ pos, size });
	}

	Camera & Camera::setViewport(const vec2i & value)
	{
		return setViewport({ { 0, 0 }, value });
	}

	Camera & Camera::setViewport(const IntRect & value)
	{
		m_viewport = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}