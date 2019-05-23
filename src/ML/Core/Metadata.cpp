#include <ML/Core/Metadata.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Metadata::Metadata()
	{
	}

	Metadata::Metadata(const map_type & values)
		: m_values(values)
	{
	}

	Metadata::Metadata(const Metadata & copy)
		: Metadata(copy.m_values)
	{
	}

	Metadata::~Metadata()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Metadata::dispose()
	{
		for (pair_type & pair : m_values)
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_values.clear();
		return m_values.empty();
	}

	void Metadata::serialize(std::ostream & out) const
	{
		for (const pair_type & pair : m_values)
		{
			out << "[" << pair.first << "] | " << (*pair.second) << endl;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}