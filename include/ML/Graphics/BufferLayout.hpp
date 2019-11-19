#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BufferLayout final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Element final
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Element() = default;

			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalize;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			Element const & operator()() const;

			/* * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BufferLayout();
		BufferLayout(const List<Element> & elements);
		BufferLayout(const std::initializer_list<Element> & elements);
		BufferLayout(BufferLayout const & copy);
		~BufferLayout();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static BufferLayout const & get_default();

		BufferLayout const & bind() const;

		BufferLayout & push_back(Element const & value);
		
		inline const List<Element> & elements() const { return m_elements; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Element> m_elements;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_