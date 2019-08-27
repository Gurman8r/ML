#include <ML/Engine/ContentManager.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentManager::dispose()
	{
		for (auto & asset_types : m_data)
		{
			for (auto & asset_data : asset_types.second)
			{
				if (asset_data.second)
				{
					// Delete the asset
					delete asset_data.second;
				}
			}
			asset_types.second.clear();
		}
		m_data.clear();
		return m_data.empty();
	}

	ContentManager::object_map & ContentManager::at(size_t index)
	{
		typeid_map::iterator it;
		return (((it = m_data.find(index)) != m_data.end())
			? it->second
			: m_data.insert({ index, {} }).first->second
		);
	}

	const ContentManager::object_map & ContentManager::at(size_t index) const
	{
		typeid_map::const_iterator it;
		return (((it = m_data.find(index)) != m_data.cend())
			? it->second
			: m_data.insert({ index, {} }).first->second
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}