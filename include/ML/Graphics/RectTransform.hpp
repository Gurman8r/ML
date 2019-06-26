#ifndef _ML_RECT_TRANSFORM_HPP_
#define _ML_RECT_TRANSFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RectTransform final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RectTransform();
		RectTransform(const vec2 & position, const vec2 & scale, const float_t rotation, const vec2 & origin);
		RectTransform(const RectTransform & copy);
		~RectTransform();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void update() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RectTransform & setOrigin	(const vec2 &	value);
		RectTransform & setPosition	(const vec2 &	value);
		RectTransform & setRotation	(const float_t	value);
		RectTransform & setScale	(const vec2 &	value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto origin()	const -> const vec2 &	{ return m_origin; }
		inline auto position()	const -> const vec2 &	{ return m_position; }
		inline auto rotation()	const -> const float_t &	{ return m_rotation; }
		inline auto scale()		const -> const vec2 &	{ return m_scale; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const mat4 & getMat() const
		{
			update();
			return m_matrix;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2	m_origin;
		vec2	m_position;
		float_t	m_rotation;
		vec2	m_scale;

		mutable mat4 m_matrix;
		mutable bool m_changed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RECT_TRANSFORM_HPP_