#ifndef _ML_LIGHT_HPP_
#define _ML_LIGHT_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Quaternion.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Input.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Light final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Mode { Realtime };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Light();
		Light(Light const & copy);
		~Light();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Light & setEnabled(bool value);
		Light & setColor(Color const & value);
		Light & setIntensity(float_t value);
		Light & setMode(Light::Mode value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const -> bool const & { return m_enabled; }
		inline auto color()		const -> Color const & { return m_color; }
		inline auto intensity() const -> float_t const & { return m_intensity; }
		inline auto mode()		const -> Mode const & { return m_mode; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool	m_enabled;		//
		Color	m_color;		// 
		float_t m_intensity;	// 
		Mode	m_mode;			// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct input<Light::Mode> final
	{
		inline auto operator()(String const & str) const
		{
			switch (util::to_lower(str).hash())
			{
			case Hash("realtime"): return Light::Realtime;
			}
			return (Light::Mode)0;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIGHT_HPP_