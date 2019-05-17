#include <ML/Graphics/RenderVar.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderVar::RenderVar()
		: RenderVar(NULL, NULL)
	{
	}

	RenderVar::RenderVar(const uint32_t type, const_reference data)
		: m_type(type)
		, m_data(data)
	{
	}

	RenderVar::RenderVar(const RenderVar & copy)
		: RenderVar(copy.m_type, (value_type)copy.m_data)
	{
	}

	RenderVar::~RenderVar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}