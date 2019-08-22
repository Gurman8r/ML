#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/I_Newable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BufferLayout final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Element final
		{
			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalized;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			const Element & operator()() const;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BufferLayout();
		BufferLayout(const List<Element> & elements);
		BufferLayout(const Initializer<Element> & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const BufferLayout & get_default();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind() const;

		BufferLayout & push_back(const Element & value);
		
		inline const List<Element> & elements() const { return m_elements; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Element> m_elements;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_