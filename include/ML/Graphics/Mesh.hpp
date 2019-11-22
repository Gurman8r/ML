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

	struct ML_GRAPHICS_API Mesh final : public Trackable, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(std::vector<Vertex> const & vertices, std::vector<uint32_t> const & indices, std::vector<Texture const *> const & textures);
		Mesh(Mesh const & copy);
		~Mesh() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();

		Mesh & create();
		Mesh & setLayout(BufferLayout const & value);
		Mesh & setIndices(std::vector<uint32_t> const & value);
		Mesh & setTextures(std::vector<Texture const *> const & value);
		Mesh & setVertices(std::vector<Vertex> const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget const & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto layout() const -> BufferLayout const & { return m_layout; }
		
		inline auto ibo() const -> IBO const & { return m_ibo; }
		
		inline auto indices() const -> std::vector<uint32_t> const & { return m_indices; }
		
		inline auto textures() const -> std::vector<Texture const *> const &	{ return m_textures; }
		
		inline auto vao() const -> VAO const & { return m_vao; }
		
		inline auto vbo() const -> VBO const & { return m_vbo; }
		
		inline auto vertices() const -> std::vector<Vertex> const & { return m_vertices; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Texture const *> m_textures;
		
		BufferLayout m_layout;
		
		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_