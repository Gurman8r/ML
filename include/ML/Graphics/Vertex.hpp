#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Vertex final : public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { Size = vec3::Size + vec4::Size + vec2::Size };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename float_t;
		using self_type			= typename Vertex;
		using array_type		= typename Array<value_type, Size>;
		using pointer			= typename array_type::pointer;
		using reference			= typename array_type::reference;
		using const_pointer		= typename array_type::const_pointer;
		using const_reference	= typename array_type::const_reference;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit Vertex(const array_type & value)
			: m_data { value }
		{
		}

		constexpr explicit Vertex(const vec3 & p, const vec4 & n, const vec2 & t)
			: self_type { { p[0], p[1], p[2], n[0], n[1], n[2], n[3], t[0], t[1] } }
		{
		}

		constexpr explicit Vertex(const vec3 & pos, const vec4 & col)
			: self_type { pos, col, vec2 { NULL } }
		{
		}

		constexpr explicit Vertex(const vec3 & pos, const vec2 & tex)
			: self_type { pos, Colors::white, tex }
		{
		}

		constexpr explicit Vertex(const vec4 & col)
			: self_type { vec3 { NULL }, col, vec2 { NULL } }
		{
		}

		constexpr explicit Vertex(const vec3 & pos)
			: self_type { pos, Colors::white, vec2 { NULL } }
		{
		}

		constexpr explicit Vertex(const vec2 & tex)
			: self_type { vec3 { NULL }, Colors::white, tex }
		{
		}

		constexpr Vertex(const Vertex & copy)
			: self_type { copy.pos(), copy.col(), copy.tex() }
		{
		}

		constexpr Vertex()
			: self_type { vec3 { NULL }, Colors::white, vec2 { NULL } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto begin()			-> pointer			{ return m_data.begin(); }
		constexpr auto begin()	const	-> const_pointer	{ return m_data.begin(); }
		constexpr auto cbegin() const	-> const_pointer	{ return m_data.cbegin(); }
		constexpr auto cend()	const	-> const_pointer	{ return m_data.cend(); }
		constexpr auto data()			-> pointer			{ return m_data.data(); }
		constexpr auto data()	const	-> const_pointer	{ return m_data.data(); }
		constexpr auto end()			-> pointer			{ return m_data.end(); }
		constexpr auto end()	const	-> const_pointer	{ return m_data.end(); }
		constexpr auto size()	const	-> size_t			{ return m_data.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr const_reference operator[](size_t i) const { return m_data[i]; }

		constexpr reference operator[](size_t i) { return m_data[i]; }

		constexpr operator array_type() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i) const -> const_reference
		{
			return (*this)[i];
		}

		constexpr auto at(size_t i, const_reference value) -> self_type &
		{
			(*this)[i] = value;
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto pos() const -> vec3 { return { at(0), at(1), at(2) }; }
		constexpr auto col() const -> vec4 { return { at(3), at(4), at(5), at(6) }; }
		constexpr auto tex() const -> vec2 { return { at(7), at(8) }; }

		constexpr auto pos(const vec3 & v) -> self_type & { return at(0, v[0]).at(1, v[1]).at(2, v[2]); }
		constexpr auto col(const vec4 & v) -> self_type & { return at(3, v[0]).at(4, v[1]).at(5, v[2]).at(6, v[3]); }
		constexpr auto tex(const vec2 & v) -> self_type & { return at(7, v[0]).at(8, v[1]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, const self_type & rhs)
		{
			return out << rhs.m_data;
		}

		inline friend ML_DESERIALIZE(std::istream & in, self_type & rhs)
		{
			return in >> rhs.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: array_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		static inline const List<float_t> & contiguous(const List<Vertex> & vertices)
		{
			static List<float_t> temp;
			if (const size_t imax = (vertices.size() * Vertex::Size))
			{
				temp.resize(imax);

				for (size_t i = 0; i < imax; i++)
				{
					temp[i] = vertices[i / Vertex::Size][i % Vertex::Size];
				}
			}
			else if (!temp.empty())
			{
				temp.clear();
			}
			return temp;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_