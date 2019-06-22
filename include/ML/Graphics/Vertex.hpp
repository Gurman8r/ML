#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Vertex final
		: public I_Newable
		, public I_Comparable<Vertex>
	{
	public:
		enum { Size = 9U };

	public:
		using array_type = typename float[Size];

	public:
		Vertex();
		Vertex(const vec3 & position);
		Vertex(const vec3 & position, const vec4 & color);
		Vertex(const vec3 & position, const vec2 & texcoords);
		Vertex(const vec3 & position, const vec4 & color, const vec2 & texcoords);
		Vertex(const Vertex & copy);
		~Vertex();

	public:
		const vec3 & position() const;
		const vec4 & color() const;
		const vec2 & texcoords() const;

	public:
		Vertex & position(const vec3 & value);
		Vertex & color(const vec4 & value);
		Vertex & texcoords(const vec2 & value);

	public:
		Vertex & position(const float x, const float y, const float z);
		Vertex & color(const float r, const float g, const float b, const float a);
		Vertex & texcoords(const float x, const float y);

	public:
		inline const float & operator[](size_t index) const	{ return m_data[index]; }
		inline float &		 operator[](size_t index)		{ return m_data[index]; }

	public:
		inline const float & at(const size_t index) const
		{
			return m_data[index];
		}

		inline const float * ptr() const
		{
			return m_data;
		}

	public:
		inline friend ML_SERIALIZE(ostream & out, const Vertex & value)
		{
			for (size_t i = 0; i < Vertex::Size; i++)
			{
				out << value[i] << ' ';
			}
			return out;
		}
		
	public:
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

	private:
		array_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_