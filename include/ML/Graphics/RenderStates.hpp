#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <ML/Core/IObject.hpp>
#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API AlphaMode final
	{
		bool		enabled		= true;
		GL::Comp	comp		= GL::Greater;
		float		coeff		= 0.01f;

		const AlphaMode & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BlendMode final
	{
		bool		enabled		= true;
		GL::Factor	srcRGB		= GL::SrcAlpha;
		GL::Factor	srcAlpha	= GL::OneMinusSrcAlpha;
		GL::Factor	dstRGB		= GL::SrcAlpha;
		GL::Factor	dstAlpha	= GL::OneMinusSrcAlpha;

		const BlendMode & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API CullingMode final
	{
		bool		enabled		= true;
		GL::Face	face		= GL::Back;

		const CullingMode & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthMode final
	{
		bool		enabled		= true;
		GL::Comp	comp		= GL::Less;

		const DepthMode & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API TextureMode final
	{
		bool		enabled		= true;
		GL::Target	target		= GL::Texture2D;
		GL::TexID	texture		= GL::Texture0;

		const TextureMode & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderStates final : public IObject
	{
		/* * * * * * * * * * * * * * * * * * * * */

		struct MiscFlags final
		{
			bool multisample		= false;
			bool framebufferSRGB	= false;

			const MiscFlags & operator()() const;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		RenderStates();
		explicit RenderStates(
			const AlphaMode		&	alpha,
			const BlendMode		&	blend,
			const CullingMode	&	culling,
			const DepthMode		&	depth,
			const TextureMode	&	texture,
			const MiscFlags		&	misc);
		RenderStates(const RenderStates & copy);
		~RenderStates();

		/* * * * * * * * * * * * * * * * * * * * */

		const RenderStates & apply() const;

		/* * * * * * * * * * * * * * * * * * * * */

		AlphaMode		alpha;
		BlendMode		blend;
		CullingMode		culling;
		DepthMode		depth;
		TextureMode		texture;
		MiscFlags		misc;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_