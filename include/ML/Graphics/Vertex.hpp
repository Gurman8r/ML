#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Vertex final
		: public I_Newable
		, public I_Comparable<Vertex>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { Size = 9U };

		using array_type = typename float[Size];

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vertex();
		Vertex(const vec3 & position);
		Vertex(const vec3 & position, const vec4 & color);
		Vertex(const vec3 & position, const vec2 & texcoords);
		Vertex(const vec3 & position, const vec4 & color, const vec2 & texcoords);
		Vertex(const Vertex & copy);
		~Vertex();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const vec3 & position() const;
		const vec4 & color() const;
		const vec2 & texcoords() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vertex & position(const vec3 & value);
		Vertex & color(const vec4 & value);
		Vertex & texcoords(const vec2 & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vertex & position(float x, float y, float z);
		Vertex & color(float r, float g, float b, float a);
		Vertex & texcoords(float x, float y);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const float & operator[](size_t index) const 
		{
			return m_data[index]; 
		}
		
		inline float & operator[](size_t index)
		{ 
			return m_data[index]; 
		}

		inline const float & at(const size_t index) const
		{
			return m_data[index];
		}

		inline const float * ptr() const
		{
			return m_data;
		}

		inline bool equals(const Vertex & other) const override
		{
			for (auto i = 0; i < Size; i++)
			{
				if (at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}

		inline bool lessThan(const Vertex & other) const override
		{
			for (auto i = 0; i < Size; i++)
			{
				if (at(i) >= other.at(i))
				{
					return false;
				}
			}
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		array_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(ostream & out, const Vertex & value)
	{
		for (size_t i = 0; i < Vertex::Size; i++)
		{
			out << value[i] << ' ';
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_