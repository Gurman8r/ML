#include <ML/Engine/Content.hpp>
#include <ML/Engine/ContentLoader.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Content::dispose()
	{
		for (auto & asset_types : m_data)
		{
			for (auto & asset_data : asset_types.second)
			{
				if (asset_data.second)
				{
					// Delete the asset container
					delete asset_data.second;
					asset_data.second = nullptr;
				}
			}
			asset_types.second.clear();
		}
		m_data.clear();
		return m_data.empty();
	}

	bool Content::loadFromFile(const String & filename)
	{
		return ContentLoader().loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}