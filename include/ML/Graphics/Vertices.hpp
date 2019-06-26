#ifndef _ML_LISTS_HPP_
#define _ML_LISTS_HPP_

#include <ML/Graphics/Vertex.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Indices = typename List<uint32_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Vertices final : public List<Vertex>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename Vertices;
		using base_type			= typename List<value_type>;
		using size_type			= typename base_type::size_type;
		using allocator_type	= typename base_type::allocator_type;
		using init_type			= typename base_type::init_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vertices()
			: base_type()
		{
		}

		explicit Vertices(const allocator_type & alloc)
			: base_type(alloc)
		{
		}

		explicit Vertices(const size_type count, const allocator_type & alloc)
			: base_type(count, alloc)
		{
		}

		Vertices(const base_type & value)
			: base_type(value)
		{
		}

		Vertices(const init_type & value)
			: base_type(value)
		{
		}

		Vertices(const self_type & value)
			: base_type(value)
		{
		}

		template <
			class Iter
		> Vertices(Iter begin, Iter end)
			: base_type(begin, end)
		{
		}
		
		~Vertices() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const List<float_t> & contiguous() const
		{
			if (const size_type imax = ((*this).size() * Vertex::Size))
			{
				m_contiguous.resize(imax);

				for (size_type i = 0; i < imax; i++)
				{
					m_contiguous[i] = (*this)[i / Vertex::Size][i % Vertex::Size];
				}
			}
			else if (!m_contiguous.empty())
			{
				m_contiguous.clear();
			}
			return m_contiguous;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: mutable List<float_t> m_contiguous;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LISTS_HPP_