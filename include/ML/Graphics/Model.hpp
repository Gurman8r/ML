#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/Mesh.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/IndexBufferObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a Drawable Mesh
	struct ML_GRAPHICS_API Model final
		: public I_Newable
		, public I_Drawable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Model();
		Model(const BufferLayout & layout);
		Model(const Model & copy);
		~Model();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Mesh & mesh);
		bool loadFromMemory(const List<float_t> & vertices);
		bool loadFromMemory(const Vertices & vertices);
		bool loadFromMemory(const Vertices & vertices, const List<uint32_t> & indices);
		bool loadFromMemory(const List<float_t> & vertices, const List<uint32_t> & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			template <class, size_t> class A, class T, size_t N
		> inline bool loadFromMemory(const A<T, N> & v)
		{
			return loadFromMemory(List<float_t> { v.begin(), v.end() });
		}

		template <
			template <class, size_t> class A, size_t V, size_t I
		> inline bool loadFromMemory(const A<float_t, V> & v, const A<uint32_t, I> & i)
		{
			return loadFromMemory(
				List<float_t>	{ v.begin(), v.end() },
				List<uint32_t>	{ i.begin(), i.end() }
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto vao() const -> const VAO & { return m_vao; }
		inline auto vbo() const -> const VBO & { return m_vbo; }
		inline auto ibo() const -> const IBO & { return m_ibo; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		BufferLayout m_layout;
		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_