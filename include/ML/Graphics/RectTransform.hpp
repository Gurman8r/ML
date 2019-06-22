#ifndef _ML_RECT_TRANSFORM_HPP_
#define _ML_RECT_TRANSFORM_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RectTransform final
		: public I_Newable
	{
	public:
		RectTransform();
		RectTransform(const vec2 & position, const vec2 & scale, const float rotation, const vec2 & origin);
		RectTransform(const RectTransform & copy);
		~RectTransform();

	public:
		void update() const;

	public:
		RectTransform & setOrigin	(const vec2 &	value);
		RectTransform & setPosition	(const vec2 &	value);
		RectTransform & setRotation	(const float	value);
		RectTransform & setScale	(const vec2 &	value);

	public:
		inline const vec2 & origin()	const { return m_origin;	}
		inline const vec2 & position() const { return m_position;	}
		inline const float & rotation() const { return m_rotation;	}
		inline const vec2 & scale()	const { return m_scale;		}

	public:
		inline const mat4 & getMat() const
		{
			update();
			return m_matrix;
		}

	private:
		vec2 m_origin;
		vec2 m_position;
		float m_rotation;
		vec2 m_scale;

		mutable mat4	m_matrix;
		mutable bool	m_changed;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RECT_TRANSFORM_HPP_