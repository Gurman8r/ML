#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <ML/Graphics/IDrawable.hpp>
#include <ML/Graphics/Mesh.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>
#include <ML/Graphics/IndexBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a Drawable Mesh
	class ML_GRAPHICS_API Model final
		: public IObject
		, public IDisposable
		, public IDrawable
		, public IReadable
	{
	public:
		Model();
		Model(const BufferLayout & layout);
		Model(const Model & copy);
		~Model();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Mesh & mesh);
		bool loadFromMemory(const List<float> & vertices);
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const List<uint32_t> & indices);
		bool loadFromMemory(const List<float> & vertices, const List<uint32_t> & indices);

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		BufferLayout m_layout;

		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_