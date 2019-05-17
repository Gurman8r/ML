#include <ML/Graphics/RenderStates.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderStates::RenderStates()
		: m_map()
	{
	}

	RenderStates::RenderStates(const init_type & init)
		: m_map()
	{
		for (auto it = init.begin(); it != init.end(); it++)
		{
			m_map.insert(*it);
		}
	}

	RenderStates::RenderStates(const map_type & flags)
		: m_map(flags)
	{
	}

	RenderStates::RenderStates(const RenderStates & copy)
		: m_map(copy.m_map)
	{
	}

	RenderStates::~RenderStates()
	{
		m_map.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RenderStates::apply() const
	{
		for (auto & pair : (*this))
		{
			apply(pair.first, pair.second);
		}
	}

	bool RenderStates::apply(const uint32_t key, const RenderVar & value) const
	{
		switch (value.type())
		{
		case RenderVar::Bool:
		{
			switch (key)
			{
			case GL::AlphaTest:
			case GL::Blend:
			case GL::CullFace:
			case GL::DepthTest:
			case GL::FramebufferSRGB:
			case GL::Multisample:
			case GL::Texture2D:
				return ((value.data()) ? (ML_GL.enable(key)) : (ML_GL.disable(key)));
			}
		}
		break;

		case RenderVar::Int:
		{
		}
		break;
		}
		return false;
	}


	/* * * * * * * * * * * * * * * * * * * * */
}