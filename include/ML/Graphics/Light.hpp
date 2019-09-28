#ifndef _ML_LIGHT_HPP_
#define _ML_LIGHT_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Light final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Mode { Realtime };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Light();
		Light(const Light & copy);
		~Light();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Light & setEnabled(bool value);
		Light & setColor(const vec4 & value);
		Light & setIntensity(float_t value);
		Light & setMode(Light::Mode value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const -> const bool & { return m_enabled; }
		inline auto color()		const -> const vec4 & { return m_color; }
		inline auto intensity() const -> const float_t & { return m_intensity; }
		inline auto mode()		const -> const Mode & { return m_mode; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool	m_enabled;		//
		vec4	m_color;		// 
		float_t m_intensity;	// 
		Mode	m_mode;			// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Light::Mode> final
	{
		inline auto operator()(const String & str) const
		{
			switch (alg::to_lower(str).hash())
			{
			case Hash("realtime"): return Light::Realtime;
			default: return (Light::Mode)0;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIGHT_HPP_