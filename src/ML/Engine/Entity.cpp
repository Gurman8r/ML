#include <ML/Engine/Entity.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool Entity::dispose()
	{
		for (auto & pair : m_data)
		{
			if (pair.second)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}
		m_data.clear();
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Entity::loadFromFile(const String & filename)
	{
		return true;
	}

	bool Entity::saveToFile(const String & filename) const
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}