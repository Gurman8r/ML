#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Vertex final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum { Size = vec3::size() + vec4::size() + vec2::size() };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename float_t;
		using self_type			= typename Vertex;
		using base_type			= typename Array<value_type, Size>;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Vertex()
			: self_type{ vec3::zero(), vec4::one(), vec2::zero() }
		{
		}

		constexpr explicit Vertex(vec2 const & texcoord)
			: self_type{ vec3::zero(), vec4::one(), texcoord }
		{
		}

		constexpr explicit Vertex(vec3 const & position)
			: self_type{ position, vec4::one(), vec2::zero() }
		{
		}

		constexpr explicit Vertex(vec4 const & normal)
			: self_type{ vec3::zero(), normal, vec2::zero() }
		{
		}

		constexpr explicit Vertex(vec3 const & position, vec4 const & normal)
			: self_type{ position, normal, vec2::zero() }
		{
		}

		constexpr explicit Vertex(vec3 const & position, vec2 const & texcoord)
			: self_type{ position, vec4::one(), texcoord }
		{
		}

		constexpr explicit Vertex(vec3 const & p, vec4 const & n, vec2 const & t)
			: self_type{ { p[0], p[1], p[2], n[0], n[1], n[2], n[3], t[0], t[1] } }
		{
		}

		constexpr Vertex(self_type const & copy)
			: self_type{ copy.m_data }
		{
		}

		constexpr explicit Vertex(base_type const & value)
			: m_data{ value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec3 position() const { return { at(0), at(1), at(2) }; }

		constexpr vec4 normal() const { return { at(3), at(4), at(5), at(6) }; }

		constexpr vec2 texcoord() const { return { at(7), at(8) }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i)			-> reference		{ return m_data.at(i); }
		constexpr auto at(size_t i) const	-> const_reference	{ return m_data.at(i); }
		constexpr auto back()				-> reference		{ return m_data.back(); }
		constexpr auto back()		const	-> const_reference	{ return m_data.back(); }	
		constexpr auto begin()				-> iterator			{ return m_data.begin(); }	
		constexpr auto begin()		const	-> const_iterator	{ return m_data.begin(); }	
		constexpr auto cbegin()		const	-> const_iterator	{ return m_data.cbegin(); }	
		constexpr auto cend()		const	-> const_iterator	{ return m_data.cend(); }	
		constexpr auto data()				-> pointer			{ return m_data.data(); }	
		constexpr auto data()		const	-> const_pointer	{ return m_data.data(); }	
		constexpr auto end()				-> iterator			{ return m_data.end(); }	
		constexpr auto end()		const	-> const_iterator	{ return m_data.end(); }	
		constexpr auto front()				-> reference		{ return m_data.front(); }	
		constexpr auto front()		const	-> const_reference	{ return m_data.front(); }	
		constexpr auto hash()		const	-> hash_t			{ return m_data.hash(); }	

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_t size() { return self_type::Size; }

		inline reference operator[](size_t i) { return at(i); }

		constexpr const_reference operator[](size_t i) const { return at(i); }

		constexpr operator base_type const &() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline friend ML_SERIALIZE(std::ostream & out, Vertex const & value)
		{
			return out << value.m_data;
		}

		inline friend ML_DESERIALIZE(std::istream & in, Vertex & value)
		{
			return in >> value.m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	namespace alg
	{
		static inline std::vector<float_t> const & contiguous(std::vector<Vertex> const & vertices)
		{
			static std::vector<float_t> temp;
			if (const size_t imax{ vertices.size() * Vertex::Size })
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