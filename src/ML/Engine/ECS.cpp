#include <ML/Engine/ECS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ECS::ECS()
		: m_cmp()
	{
	}

	ECS::~ECS()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ECS::dispose()
	{
		if (!m_cmp.empty())
		{
			for (auto & pair : m_cmp)
			{
				for (auto & e : pair.second)
				{
					delete e.second;
					e.second = NULL;
				}
				pair.second.clear();
			}
			m_cmp.clear();
		}
		return m_cmp.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * ECS::createAlloc(const size_t type, const size_t index, const size_t size)
	{
		type_map::iterator it;
		if ((it = m_cmp.find(type)) != m_cmp.end())
		{
			return it->second.insert({
				index,
				std::malloc(size)
			}).first->second;
		}
		return NULL;
	}

	bool ECS::freeAlloc(const size_t type, const size_t index, void * ptr)
	{
		type_map::iterator it;
		if ((it = m_cmp.find(type)) != m_cmp.end())
		{
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}