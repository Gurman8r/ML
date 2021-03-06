#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Mesh.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a drawable collection of Meshes
	struct ML_GRAPHICS_API Model final : public Trackable, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Model();
		explicit Model(String const & filename);
		Model(Model const & copy);
		~Model() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);
		bool loadFromMemory(std::vector<Vertex> const & vertices);
		bool loadFromMemory(std::vector<Vertex> const & vertices, std::vector<uint32_t> const & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t> class A, class T, size_t N
		> inline bool loadFromMemory(const A<T, N> & v)
		{
			return loadFromMemory(std::vector<Vertex> { v.begin(), v.end() });
		}

		template <
			template <class, size_t> class A, size_t V, size_t I
		> inline bool loadFromMemory(const A<Vertex, V> & v, const A<uint32_t, I> & i)
		{
			return loadFromMemory(
				std::vector<Vertex> { v.begin(), v.end() },
				std::vector<uint32_t> { i.begin(), i.end() }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto meshes()		-> std::vector<Mesh *> &		{ return m_meshes; }
		inline auto meshes() const	-> const std::vector<Mesh *> & { return m_meshes; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		std::vector<Mesh *> m_meshes;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_