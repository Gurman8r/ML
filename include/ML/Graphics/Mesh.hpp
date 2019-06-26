#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Graphics/Vertices.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Mesh final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(const Vertices & vertices);
		Mesh(const Vertices & vertices, const Indices & indices);
		Mesh(const Mesh & copy);
		~Mesh();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const List<float> & vertices);
		bool loadFromMemory(const List<float> & vertices, const Indices & indices);
		bool loadFromMemory(const Vertices & vertices);
		bool loadFromMemory(const Vertices & vertices, const Indices & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t> class A, class T, size_t N
		> inline bool loadFromMemory(const A<T, N> & vertices)
		{
			return loadFromMemory(List<float>(vertices.begin(), vertices.end()));
		}

		template <
			template <class, size_t> class A, size_t V, size_t I
		> inline bool loadFromMemory(const A<float, V> & vertices, const A<uint32_t, I> & indices)
		{
			return loadFromMemory(
				List<float>(vertices.begin(), vertices.end()),
				Indices(indices.begin(), indices.end())
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto vertices() const	-> const Vertices &		{ return m_vertices;	}
		inline auto indices	() const	-> const Indices &		{ return m_indices;		}
		inline auto contiguous() const	-> const List<float> &	{ return m_contiguous;	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Vertices	m_vertices;
		Indices		m_indices;
		List<float>	m_contiguous;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_