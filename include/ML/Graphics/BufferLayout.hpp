#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <ML/Graphics/GL.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API BufferLayout final
		: public ITrackable
	{
	public:
		static const BufferLayout Default;

	public:
		struct Element final
			: public ITrackable
			, public IComparable<Element>
		{
			/* * * * * * * * * * * * * * * * * * * * */

			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalized;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			/* * * * * * * * * * * * * * * * * * * * */

			Element();
			Element(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width);
			Element(const Element & copy);

			/* * * * * * * * * * * * * * * * * * * * */

			void use() const;
			bool equals(const Element & other) const override;
			bool lessThan(const Element & other) const override;

			/* * * * * * * * * * * * * * * * * * * * */
		};

	public:
		BufferLayout();
		BufferLayout(const List<Element> & elements);
		BufferLayout(const Initializer<Element> & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		void bind() const;

		BufferLayout & push_back(const Element & value);
		
		inline const List<Element> & elements() const { return m_elements; }

	private:
		List<Element> m_elements;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_