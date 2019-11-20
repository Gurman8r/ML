#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Graphics/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BufferLayout final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Element final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			uint32_t	index		{ 0 };
			uint32_t	size		{ 0 };
			GL::Type	type		{ (GL::Type)0 };
			bool		normalize	{ false };
			uint32_t	stride		{ 0 };
			uint32_t	offset		{ 0 };
			uint32_t	width		{ 0 };

			constexpr Element() noexcept = default;
			constexpr Element(Element const &) noexcept = default;
			constexpr Element(Element &&) noexcept = default;

			Element const & operator()() const;

			/* * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename Element;
		using self_type			= typename BufferLayout;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr const Element Default[] = {
			Element { 0, 3, GL::Float, false, Vertex::Size, 0, sizeof(float_t) },
			Element { 1, 4, GL::Float, false, Vertex::Size, 3, sizeof(float_t) },
			Element { 2, 2, GL::Float, false, Vertex::Size, 7, sizeof(float_t) },
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BufferLayout() noexcept 
			: self_type{ Default }
		{
		}

		template <size_t N>
		constexpr BufferLayout(const value_type(&elements)[N]) noexcept
			: self_type{ &elements[0], N }
		{
		}

		constexpr BufferLayout(const_iterator first, const_iterator last) noexcept
			: self_type { first, (size_t)(last - first) }
		{
		}

		constexpr BufferLayout(self_type const & copy) noexcept
			: self_type{ copy.m_data, copy.m_size }
		{
		}

		constexpr BufferLayout(const_pointer elements, size_t size) noexcept
			: m_data{ elements }
			, m_size{ size }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type const & bind() const
		{
			for (auto const & elem : (*this))
			{
				elem();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto at(size_t i) const -> const_reference	{ return *(cbegin() + i); }
		constexpr auto back()		const -> const_reference	{ return (*(cend() - 1)); }
		constexpr auto begin()		const -> const_iterator		{ return m_data; }
		constexpr auto cbegin()		const -> const_iterator		{ return m_data; }
		constexpr auto cend()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto data()		const -> const_pointer		{ return m_data; }
		constexpr auto empty()		const -> bool				{ return (m_size == 0); }
		constexpr auto end()		const -> const_iterator		{ return m_data + m_size; }
		constexpr auto front()		const -> const_reference	{ return (*cbegin()); }
		constexpr auto size()		const -> size_t				{ return m_size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator bool() const
		{
			return !empty();
		}

		constexpr const_reference operator[](size_t i) const
		{
			return at(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: const_pointer m_data; size_t m_size;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_