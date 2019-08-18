#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Vertices.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/IndexBufferObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Mesh final
		: public I_Newable
		, public I_Disposable
		, public I_Drawable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct tex_t
		{
			uint32_t id;
			String type;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Mesh();
		Mesh(const Vertices & vertices, const List<uint32_t> & indices, const List<tex_t> & textures);
		Mesh(const Mesh & copy);
		~Mesh() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void setup();

		bool dispose() override;

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		Vertices		m_vertices;
		List<uint32_t>	m_indices;
		List<tex_t>		m_textures;

		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_