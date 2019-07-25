#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderStates::RenderStates()
		: alpha		{ }
		, blend		{ }
		, culling	{ }
		, depth		{ }
		, texture	{ }
		, misc		{ }
	{
	}

	RenderStates::RenderStates(
		const AlphaMode		&	alpha,
		const BlendMode		&	blend,
		const CullingMode	&	culling,
		const DepthMode		&	depth,
		const TextureMode	&	texture,
		const MiscStates			&	misc)
		: alpha		{ alpha.enabled, alpha.comp, alpha.coeff }
		, blend		{ blend.enabled, blend.srcRGB, blend.srcAlpha, blend.dstRGB, blend.dstAlpha }
		, culling	{ culling.enabled, culling.face }
		, depth		{ depth.enabled, depth.comp }
		, texture	{ texture.enabled, texture.target, texture.texture }
		, misc		{ misc.multisample, misc.framebufferSRGB }
	{
	}

	RenderStates::RenderStates(const RenderStates & copy) : RenderStates(
		copy.alpha,
		copy.blend,
		copy.culling,
		copy.depth,
		copy.texture,
		copy.misc)
	{
	}

	RenderStates::~RenderStates()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const RenderStates & RenderStates::apply() const
	{
		this->alpha();
		this->blend();
		this->culling();
		this->depth();
		//this->texture();
		this->misc();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const AlphaMode & AlphaMode::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Flag::AlphaTest);
		}
		else if (ML_GL.enable(GL::Flag::AlphaTest, this->enabled))
		{
			ML_GL.alphaFunc(this->comp, this->coeff);
		}
		else
		{
			Debug::logWarning("Failed enabling Alpha Mode");
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const BlendMode & BlendMode::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Flag::Blend);
		}
		else if (ML_GL.enable(GL::Flag::Blend, this->enabled))
		{
			ML_GL.blendFuncSeparate(
				this->srcRGB, this->srcAlpha, 
				this->dstRGB, this->dstAlpha
			);
		}
		else
		{
			Debug::logWarning("Failed enabling Blend Mode");
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const CullingMode & CullingMode::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Flag::CullFace);
		}
		else if (ML_GL.enable(GL::Flag::CullFace, this->enabled))
		{
			ML_GL.cullFace(this->face);
		}
		else
		{
			Debug::logWarning("Failed enabling Culling Mode");
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const DepthMode & DepthMode::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(GL::Flag::DepthTest);
		}
		else if (ML_GL.enable(GL::Flag::DepthTest, this->enabled))
		{
			ML_GL.depthFunc(this->comp);
		}
		else
		{
			Debug::logWarning("Failed enabling Depth Mode");
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const TextureMode & TextureMode::operator()() const
	{
		if (!this->enabled)
		{
			ML_GL.disable(this->target);
		}
		else if (ML_GL.enable(this->target, this->enabled))
		{
			ML_GL.activeTexture(this->texture);
		}
		else
		{
			Debug::logWarning("Failed enabling Texture Mode");
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const MiscStates & MiscStates::operator()() const
	{
		// Multisample
		if (!this->multisample)
		{
			ML_GL.disable(GL::Flag::Multisample);
		}
		else if (!ML_GL.enable(GL::Flag::Multisample, this->multisample))
		{
			Debug::logWarning("Failed enabling Multisample");
		}

		// Framebuffer SRGB
		if (!this->framebufferSRGB)
		{
			ML_GL.disable(GL::Flag::FramebufferSRGB);
		}
		else if (!ML_GL.enable(GL::Flag::FramebufferSRGB, this->framebufferSRGB))
		{
			Debug::logWarning("Failed enabling FramebufferSRGB");
		}

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}