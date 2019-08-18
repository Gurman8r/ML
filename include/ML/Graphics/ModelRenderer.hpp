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

		struct tex_t
		{
			uint32_t	id;
			String		type;
		};

		struct mesh_t : public I_Drawable, public I_Newable
		{
			Vertices		vertices;
			List<uint32_t>	indices;
			List<tex_t>		textures;

			mesh_t(const Vertices & v, const List<uint32_t> & i, const List<tex_t> & t)
				: vertices(v)
				, indices(i)
				, textures(t)
			{
				setup();
			}

			VAO	m_vao;
			VBO	m_vbo;
			IBO	m_ibo;

			void setup();

			void draw(RenderTarget & target, RenderBatch batch) const override;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ModelRenderer();
		~ModelRenderer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ModelRenderer & setMaterial(const Material * value);
		ModelRenderer & setStates(const RenderStates & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto material()			-> const Material *&	{ return m_material; }
		inline auto material()	const	-> const Material *		{ return m_material; }
		inline auto states()			-> RenderStates	&		{ return m_states; }
		inline auto states()	const	-> const RenderStates & { return m_states; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		RenderStates		m_states;
		const Material *	m_material;
		List<mesh_t *>		m_meshes;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_RENDERER_HPP_