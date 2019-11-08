#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <ML/Graphics/Drawable.hpp>
#include <ML/Graphics/Vertex.hpp>
#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/IndexBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Mesh final
		: public Newable
		, public Disposable
		, public Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(const List<Vertex> & vertices, const List<uint32_t> & indices, const List<const Texture *> & textures);
		Mesh(const Mesh & copy);
		~Mesh() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		Mesh & create();
		Mesh & addVertex(const Vertex & value);
		Mesh & addIndex(uint32_t value);
		Mesh & setLayout(const BufferLayout & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(const RenderTarget & target, RenderBatch & batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto vertices()	const -> const List<Vertex> &			{ return m_vertices; }
		inline auto indices()	const -> const List<uint32_t> &			{ return m_indices; }
		inline auto textures()	const -> const List<const Texture *> &	{ return m_textures; }
		inline auto layout()	const -> const BufferLayout &			{ return m_layout; }
		inline auto vao()		const -> const VAO &					{ return m_vao; }
		inline auto vbo()		const -> const VBO &					{ return m_vbo; }
		inline auto ibo()		const -> const IBO &					{ return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		List<Vertex>			m_vertices;
		List<uint32_t>			m_indices;
		List<const Texture *>	m_textures;
		
		BufferLayout m_layout;
		
		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_