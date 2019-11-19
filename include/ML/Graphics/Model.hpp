#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Mesh.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a drawable collection of Meshes
	struct ML_GRAPHICS_API Model final
		: public Trackable
		, public Drawable
		, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Model();
		explicit Model(const String & filename);
		Model(const Model & copy);
		~Model();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool loadFromMemory(const List<Vertex> & vertices);
		bool loadFromMemory(const List<Vertex> & vertices, const List<uint32_t> & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t> class A, class T, size_t N
		> inline bool loadFromMemory(const A<T, N> & v)
		{
			return loadFromMemory(List<Vertex> { v.begin(), v.end() });
		}

		template <
			template <class, size_t> class A, size_t V, size_t I
		> inline bool loadFromMemory(const A<Vertex, V> & v, const A<uint32_t, I> & i)
		{
			return loadFromMemory(
				List<Vertex> { v.begin(), v.end() },
				List<uint32_t> { i.begin(), i.end() }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto meshes()		-> List<Mesh *> &		{ return m_meshes; }
		inline auto meshes() const	-> const List<Mesh *> & { return m_meshes; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		List<Mesh *> m_meshes;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_