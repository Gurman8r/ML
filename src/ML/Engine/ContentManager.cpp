#include <ML/Engine/ContentManager.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ContentManager::ContentManager() : m_data {} {}

	ContentManager::~ContentManager() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentManager::dispose()
	{
		for (auto & types : m_data)
		{
			for (auto & elem : types.second)
			{
				if (elem.second) { delete elem.second; }
			}
			types.second.clear();
		}
		m_data.clear();
		return m_data.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ContentManager::AssetMap & ContentManager::data(hash_t code)
	{
		auto it { m_data.find(code) };
		return ((it != m_data.end())
			? it->second
			: m_data.insert({ code, {} }).first->second
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const ContentManager::AssetMap & ContentManager::data(hash_t code) const
	{
		auto it { m_data.find(code) };
		return ((it != m_data.end())
			? it->second
			: m_data.insert({ code, {} }).first->second
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ptr_t<Newable> ContentManager::insert(hash_t code, const String & name, ptr_t<void> value)
	{
		return this->data(code).insert({ name, (ptr_t<Newable>)value }).first->second;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ptr_t<Newable> ContentManager::generate(const String & type, const String & name)
	{
		if (const hash_t * code { ML_Registry.get_code(type) })
		{
			if (!this->exists(*code, name))
			{
				return this->insert(*code, name, ML_Registry.generate(*code));
			}
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentManager::destroy(hash_t code, const String & name)
	{
		auto it { this->data(code).find(name) };
		if (it != this->data(code).end())
		{
			ptr_t<Newable>& ptr { it->second };
			delete ptr;
			ptr = nullptr;
			this->data(code).erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ContentManager::exists(hash_t code, const String & name) const
	{
		return (this->data(code).find(name) != this->data(code).cend());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}