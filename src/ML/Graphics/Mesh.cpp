#include <ML/Graphics/Mesh.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh()
		: m_vertices()
		, m_indices	()
		, m_textures()
		, m_layout	(BufferLayout::get_default())
		, m_vao		()
		, m_vbo		()
		, m_ibo		()
	{
	}

	Mesh::Mesh(const List<Vertex> & vertices, const List<uint32_t> & indices, const List<const Texture *> & textures)
		: m_vertices(vertices)
		, m_indices	(indices)
		, m_textures(textures)
		, m_layout	(BufferLayout::get_default())
		, m_vao		()
		, m_vbo		()
		, m_ibo		()
	{
		create();
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices(copy.m_vertices)
		, m_indices	(copy.m_indices)
		, m_textures(copy.m_textures)
		, m_layout	(copy.m_layout)
		, m_vao		()
		, m_vbo		()
		, m_ibo		()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	
	Mesh & Mesh::create()
	{
		if (!m_vertices.empty() && !m_indices.empty())
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(alg::contiguous(m_vertices));
			m_ibo.create(GL::StaticDraw, GL::UnsignedInt).bind().bufferData(m_indices);
			m_layout.bind();
			m_ibo.unbind();
			m_vbo.unbind();
			m_vao.unbind();
		}
		else if (!m_vertices.empty())
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(alg::contiguous(m_vertices));
			m_layout.bind();
			m_vbo.unbind();
			m_vao.unbind();
		}
		return (*this);
	}

	Mesh & Mesh::addVertex(const Vertex & value)
	{
		m_vertices.push_back(value);
		return (*this);
	}

	Mesh & Mesh::addIndex(uint32_t value)
	{
		m_indices.push_back(value);
		return (*this);
	}

	Mesh & Mesh::setLayout(const BufferLayout & value)
	{
		m_layout = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}