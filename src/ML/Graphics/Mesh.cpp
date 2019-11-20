#include <ML/Graphics/Mesh.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/ScopedBinder.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh()
		: m_vertices{}
		, m_indices	{}
		, m_textures{}
		, m_layout	{}
		, m_vao		{}
		, m_vbo		{}
		, m_ibo		{}
	{
	}

	Mesh::Mesh(ArrayList<Vertex> const & vertices, ArrayList<uint32_t> const & indices, ArrayList<Texture const *> const & textures)
		: m_vertices{ vertices }
		, m_indices	{ indices }
		, m_textures{ textures }
		, m_layout	{}
		, m_vao		{}
		, m_vbo		{}
		, m_ibo		{}
	{
		create();
	}

	Mesh::Mesh(Mesh const & copy)
		: m_vertices{ copy.m_vertices }
		, m_indices	{ copy.m_indices }
		, m_textures{ copy.m_textures }
		, m_layout	{ copy.m_layout }
		, m_vao		{}
		, m_vbo		{}
		, m_ibo		{}
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
			if (ML_BIND_SCOPE(VAO, m_vao.create(GL::Triangles)))
			{
				if (ML_BIND_SCOPE_EX(VBO, _vb, m_vbo.create(GL::StaticDraw)))
				{
					if (ML_BIND_SCOPE_EX(IBO, _ib, m_ibo.create(GL::StaticDraw, GL::UnsignedInt)))
					{
						_vb->bufferData(alg::contiguous(m_vertices));
						
						_ib->bufferData(m_indices);
						
						m_layout.bind();
					}
				}
			}
		}
		else if (!m_vertices.empty())
		{
			if (ML_BIND_SCOPE(VAO, m_vao.create(GL::Triangles)))
			{
				if (ML_BIND_SCOPE_EX(VBO, _vb, m_vbo.create(GL::StaticDraw)))
				{
					_vb->bufferData(alg::contiguous(m_vertices));

					m_layout.bind();
				}
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh & Mesh::setLayout(BufferLayout const & value)
	{
		m_layout = value;
		return (*this);
	}

	Mesh & Mesh::setIndices(ArrayList<uint32_t> const & value)
	{
		m_indices = value;
		return (*this);
	}

	Mesh & Mesh::setTextures(ArrayList<Texture const *> const & value)
	{
		m_textures = value;
		return (*this);
	}

	Mesh & Mesh::setVertices(ArrayList<Vertex> const & value)
	{
		m_vertices = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Mesh::draw(RenderTarget const & target, RenderBatch & batch) const
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