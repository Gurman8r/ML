#ifndef _ML_LISTS_HPP_
#define _ML_LISTS_HPP_

#include <ML/Graphics/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API VertexList final
		: public List<Vertex>
	{
	public:
		using self_type			= typename VertexList;
		using base_type			= typename List<value_type>;
		using size_type			= typename base_type::size_type;
		using allocator_type	= typename base_type::allocator_type;
		using init_type			= typename base_type::init_type;

	public:
		VertexList();
		explicit VertexList(const allocator_type & alloc);
		explicit VertexList(const size_type count, const allocator_type & alloc = allocator_type());
		VertexList(const base_type & value);
		VertexList(const init_type & value);
		VertexList(const self_type & value);
		
		template <
			class Iter
		> VertexList(Iter begin, Iter end)
			: base_type(begin, end)
		{
		}
		
		virtual ~VertexList();

	public:
		const List<float> & contiguous() const;

	private:
		mutable List<float> m_contiguous;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LISTS_HPP_