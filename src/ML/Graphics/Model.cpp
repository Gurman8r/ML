#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/RenderTarget.hpp>
#include <ML/Graphics/Geometry.hpp>

#define ML_DEFAULT_CUBE "DEFAULT_CUBE"
#define ML_DEFAULT_QUAD "DEFAULT_QUAD"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Model::Model()
		: Model(BufferLayout::Default)
	{
	}

	Model::Model(const BufferLayout & layout)
		: m_layout	(layout)
		, m_vao		()
		, m_vbo		()
		, m_ibo		()
	{
	}

	Model::Model(const Model & copy)
		: m_layout	(copy.m_layout)
		, m_vao		(copy.m_vao)
		, m_vbo		(copy.m_vbo)
		, m_ibo		(copy.m_ibo)
	{
	}

	Model::~Model()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Model::loadFromFile(const String & filename)
	{
		Mesh mesh;
		return mesh.loadFromFile(filename) && loadFromMemory(mesh);
	}

	bool Model::loadFromMemory(const Mesh & mesh)
	{
		return mesh.indices().empty()
			? loadFromMemory(mesh.contiguous())
			: loadFromMemory(mesh.contiguous(), mesh.indices());
	}

	bool Model::loadFromMemory(const Vertices & vertices)
	{
		return loadFromMemory(vertices.contiguous());
	}

	bool Model::loadFromMemory(const List<float_t> & vertices)
	{
		m_vao.create(GL::Triangles);
		m_vao.bind();
		
		m_vbo.create(GL::StaticDraw);
		m_vbo.bind();
		m_vbo.bufferData(vertices);
		
		m_layout.bind();
		
		m_vbo.unbind();
		m_vao.unbind();
		
		return (m_vao && m_vbo);
	}

	bool Model::loadFromMemory(const Vertices & vertices, const Indices & indices)
	{
		return loadFromMemory(vertices.contiguous(), indices);
	}

	bool Model::loadFromMemory(const List<float_t> & vertices, const Indices & indices)
	{
		m_vao.create(GL::Triangles);
		m_vao.bind();
		
		m_vbo.create(GL::StaticDraw);
		m_vbo.bind();
		m_vbo.bufferData(vertices);
		
		m_ibo.create(GL::StaticDraw, GL::UnsignedInt);
		m_ibo.bind();
		m_ibo.bufferData(indices);
		
		m_layout.bind();
		
		m_ibo.unbind();
		m_vbo.unbind();
		m_vao.unbind();
		
		return (m_vao && m_vbo && m_ibo);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Model::draw(RenderTarget & target, RenderBatch batch) const
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

	/* * * * * * * * * * * * * * * * * * * * */
}