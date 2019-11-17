#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Transform final : public Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Transform();
		Transform(const Transform & copy);
		~Transform();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Transform & setEnabled(bool value);
		Transform & setPosition(const vec3 & value);
		Transform & setRotation(const vec4 & value);
		Transform & setScale(const vec3 & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto enabled()	const -> const bool & { return m_enabled; }
		inline auto position()	const -> const vec3 & { return m_pos; }
		inline auto rotation()	const -> const vec4 & { return m_rot; }
		inline auto scale()		const -> const vec3 & { return m_scl; }

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