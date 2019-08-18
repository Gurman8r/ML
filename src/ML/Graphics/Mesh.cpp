#include <ML/Graphics/Mesh.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>


namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh()
		: m_vertices()
		, m_indices()
		, m_textures()
		, m_vao()
		, m_vbo()
		, m_ibo()
	{
	}

	Mesh::Mesh(const Vertices & vertices, const List<uint32_t> & indices, const List<tex_t> & textures)
		: m_vertices(vertices)
		, m_indices	(indices)
		, m_textures(textures)
		, m_vao()
		, m_vbo()
		, m_ibo()
	{
		setup();
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices(copy.m_vertices)
		, m_indices	(copy.m_indices)
		, m_textures(copy.m_textures)
		, m_vao(copy.m_vao)
		, m_vbo(copy.m_vbo)
		, m_ibo(copy.m_ibo)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void Mesh::setup()
	{
		if (m_vertices && m_indices)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(m_vertices.contiguous());
			m_ibo.create(GL::StaticDraw, GL::UnsignedInt).bind().bufferData(m_indices);
			BufferLayout::Default.bind();
			m_ibo.unbind();
			m_vbo.unbind();
			m_vao.unbind();
		}
		else if (m_vertices)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(m_vertices.contiguous());
			BufferLayout::Default.bind();
			m_vbo.unbind();
			m_vao.unbind();
		}
	}

	bool Mesh::dispose()
	{
		m_vertices.clear();
		m_indices.clear();
		m_textures.clear();

		m_vao.clean();
		m_vbo.clean();
		m_ibo.clean();

		return true;
	}

	void Mesh::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_vao && m_vbo)
		{
			if (m_ibo)
			{
				target.draw(m_vao, m_vbo, m_ibo);
			}
			else
			{
				target.draw(m_vao, m_vbo);
			}
		}
	}
	

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}