#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <ML/Graphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { Size = vec3::Size + vec4::Size + vec2::Size };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename float;
		using self_type			= typename Vertex;
		using array_type		= typename array_t<value_type, Size>;
		using pointer			= typename array_type::pointer;
		using reference			= typename array_type::reference;
		using const_pointer		= typename array_type::const_pointer;
		using const_reference	= typename array_type::const_reference;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr explicit Vertex(const array_type & value)
			: m_data { value }
		{
		}

		constexpr explicit Vertex(const vec3 & pos, const vec4 & col, const vec2 & tex)
			: self_type { {
			pos[0], pos[1], pos[2],
			col[0], col[1], col[2], col[3],
			tex[0], tex[1] } }
		{
		}

		constexpr explicit Vertex(const vec3 & pos, const vec4 & col)
			: self_type { pos, col, vec2::zero() }
		{
		}

		constexpr explicit Vertex(const vec3 & pos, const vec2 & tex)
			: self_type { pos, color::white, tex }
		{
		}

		constexpr explicit Vertex(const vec4 & col)
			: self_type { vec3::zero(), col, vec2::zero() }
		{
		}

		constexpr explicit Vertex(const vec3 & pos)
			: self_type { pos, color::white, vec2::zero() }
		{
		}

		constexpr explicit Vertex(const vec2 & tex)
			: self_type { vec3::zero(), color::white, tex }
		{
		}

		constexpr Vertex(const Vertex & copy)
			: self_type { copy.pos(), copy.col(), copy.tex() }
		{
		}

		constexpr Vertex()
			: self_type { vec3::zero(), color::white, vec2::zero() }
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

		constexpr auto get(size_t i) const
			-> const_reference
		{
			return (*this)[i];
		}

		constexpr auto pos() const -> vec3 { return { get(0), get(1), get(2) }; }
		constexpr auto col() const -> vec4 { return { get(3), get(4), get(5), get(6) }; }
		constexpr auto tex() const -> vec2 { return { get(7), get(8) }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto set(size_t i, const_reference value)
			-> self_type &
		{
			(*this)[i] = value;
			return (*this);
		}

		constexpr auto pos(const vec3 & v) -> self_type & { return set(0, v[0]).set(1, v[1]).set(2, v[2]); }
		constexpr auto col(const vec4 & v) -> self_type & { return set(3, v[0]).set(4, v[1]).set(5, v[2]).set(6, v[3]); }
		constexpr auto tex(const vec2 & v) -> self_type & { return set(7, v[0]).set(8, v[1]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(ostream & out, const self_type & rhs)
		{
			return out << rhs.m_data;
		}

		inline friend ML_DESERIALIZE(istream & in, self_type & rhs)
		{
			return in >> rhs.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: array_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_