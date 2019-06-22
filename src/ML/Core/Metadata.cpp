#include <ML/Core/Metadata.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Metadata::Metadata()
	{
	}

	Metadata::Metadata(const map_type & values)
		: m_data(values)
	{
	}

	Metadata::Metadata(const Metadata & copy)
		: Metadata(copy.m_data)
	{
	}

	Metadata::~Metadata()
	{
		for (pair_type & pair : m_data)
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_data.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}