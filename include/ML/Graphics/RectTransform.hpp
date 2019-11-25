#ifndef _ML_RECT_TRANSFORM_HPP_
#define _ML_RECT_TRANSFORM_HPP_

#include <ML/Graphics/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API RectTransform final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RectTransform();
		RectTransform(vec2 const & position, vec2 const & scale, const float_t rotation, vec2 const & origin);
		RectTransform(RectTransform const & copy);
		~RectTransform();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void update() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RectTransform & setOrigin(vec2 const & value);
		RectTransform & setPosition(vec2 const & value);
		RectTransform & setRotation(float_t value);
		RectTransform & setScale(vec2 const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto origin()	const -> vec2 const & { return m_origin; }
		inline auto position()	const -> vec2 const & { return m_position; }
		inline auto rotation()	const -> float_t const & { return m_rotation; }
		inline auto scale()		const -> vec2 const & { return m_scale; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mat4 const & getMat() const
		{
			update();
			return m_matrix;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2	m_origin;
		vec2	m_position;
		float_t	m_rotation;
		vec2	m_scale;

		mutable mat4 m_matrix;
		mutable bool m_changed;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RECT_TRANSFORM_HPP_