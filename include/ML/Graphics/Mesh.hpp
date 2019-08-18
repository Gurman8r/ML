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
		Mesh(const Vertices & vertices, const List<uint32_t> & indices);
		Mesh(const Mesh & copy);
		~Mesh() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const List<float_t> & vertices);
		bool loadFromMemory(const List<float_t> & vertices, const List<uint32_t> & indices);
		bool loadFromMemory(const Vertices & vertices);
		bool loadFromMemory(const Vertices & vertices, const List<uint32_t> & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t> class A, class T, size_t N
		> inline bool loadFromMemory(const A<T, N> & v)
		{
			return loadFromMemory(List<float_t> { v.begin(), v.end() });
		}

		template <
			template <class, size_t> class A, size_t V, size_t I
		> inline bool loadFromMemory(const A<float_t, V> & v, const A<uint32_t, I> & i)
		{
			return loadFromMemory(
				List<float_t>	{ v.begin(), v.end() },
				List<uint32_t>	{ i.begin(), i.end() }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		inline auto filename	() const -> const String &			{ return m_filename; }
		inline auto directory	() const -> const String &			{ return m_directory; }
		inline auto vertices	() const -> const Vertices &		{ return m_vertices; }
		inline auto indices		() const -> const List<uint32_t> &	{ return m_indices; }
		inline auto contiguous	() const -> const List<float_t> &	{ return m_contiguous; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String			m_filename;
		String			m_directory;
		Vertices		m_vertices;
		List<uint32_t>	m_indices;
		List<float_t>	m_contiguous;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_