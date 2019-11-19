#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Transform final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Transform();
		Transform(Transform const & copy);
		~Transform();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Transform & setEnabled(bool value);
		Transform & setPosition(vec3 const & value);
		Transform & setRotation(vec4 const & value);
		Transform & setScale(vec3 const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const -> bool const & { return m_enabled; }
		inline auto position()	const -> vec3 const & { return m_pos; }
		inline auto rotation()	const -> vec4 const & { return m_rot; }
		inline auto scale()		const -> vec3 const & { return m_scl; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		bool m_enabled; //
		vec3 m_pos; // 
		vec4 m_rot; // 
		vec3 m_scl; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_