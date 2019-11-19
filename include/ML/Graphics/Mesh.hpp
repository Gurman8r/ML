#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <ML/Graphics/Drawable.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/IndexBufferObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Mesh final : public Trackable, public Disposable, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(ArrayList<Vertex> const & vertices, ArrayList<uint32_t> const & indices, ArrayList<Texture const *> const & textures);
		Mesh(Mesh const & copy);
		~Mesh() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		Mesh & create();
		Mesh & addVertex(Vertex const & value);
		Mesh & addIndex(uint32_t value);
		Mesh & setLayout(BufferLayout const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto vertices()	const -> ArrayList<Vertex> const & { return m_vertices; }
		inline auto indices()	const -> ArrayList<uint32_t> const & { return m_indices; }
		inline auto textures()	const -> ArrayList<Texture const *> const &	{ return m_textures; }
		inline auto layout()	const -> BufferLayout const & { return m_layout; }
		inline auto vao()		const -> VAO const & { return m_vao; }
		inline auto vbo()		const -> VBO const & { return m_vbo; }
		inline auto ibo()		const -> IBO const & { return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ArrayList<Vertex>			m_vertices;
		ArrayList<uint32_t>			m_indices;
		ArrayList<Texture const *>	m_textures;
		
		BufferLayout m_layout;
		
		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_