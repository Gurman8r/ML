#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Light::Light()
		: m_enabled		{ true }
		, m_color		{ Colors::white }
		, m_intensity	{ 1.0f }
		, m_mode		{ Light::Realtime }
	{
	}

	Light::Light(const Light & copy)
		: m_enabled		{ copy.m_enabled }
		, m_color		{ copy.m_color }
		, m_intensity	{ copy.m_intensity }
		, m_mode		{ copy.m_mode }
	{
	}

	Light::~Light() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Light & Light::setEnabled(bool value)
	{
		m_enabled = value;
		return (*this);
	}

	Light & Light::setColor(const Color & value)
	{
		m_color = value;
		return (*this);
	}

	Light & Light::setIntensity(float_t value)
	{
		m_intensity = value;
		return (*this);
	}

	Light & Light::setMode(Mode value)
	{
		m_mode = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}