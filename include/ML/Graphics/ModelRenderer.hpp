#ifndef _ML_MODEL_RENDERER_HPP_
#define _ML_MODEL_RENDERER_HPP_

#include <ML/Graphics/I_Drawable.hpp>
#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/BufferLayout.hpp>
#include <ML/Graphics/VertexArrayObject.hpp>
#include <ML/Graphics/VertexBufferObject.hpp>
#include <ML/Graphics/IndexBufferObject.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API ModelRenderer
		: public I_Newable
		, public I_Drawable
		, public I_NonCopyable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ModelRenderer();
		explicit ModelRenderer(const BufferLayout & layout);
		~ModelRenderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const List<float_t> & vertices);
		bool loadFromMemory(const Vertices & vertices);
		bool loadFromMemory(const Vertices & vertices, const List<uint32_t> & indices);
		bool loadFromMemory(const List<float_t> & vertices, const List<uint32_t> & indices);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto layout()			-> BufferLayout &		{ return m_layout; }
		inline auto layout()	const	-> const BufferLayout & { return m_layout; }
		inline auto vao()		const	-> const VAO &			{ return m_vao; }
		inline auto vbo()		const	-> const VBO &			{ return m_vbo; }
		inline auto ibo()		const	-> const IBO &			{ return m_ibo; }
		inline auto material()			-> const Material *&	{ return m_material; }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto states()			-> RenderStates	&		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		BufferLayout		m_layout;
		VAO					m_vao;
		VBO					m_vbo;
		IBO					m_ibo;
		RenderStates		m_states;
		const Material *	m_material;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_RENDERER_HPP_