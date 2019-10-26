#include <ML/Engine/Entity.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Entity::Entity() : m_data {} {}
	
	Entity::~Entity() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Entity::dispose()
	{
		for (auto & [key, value] : m_data)
		{
			Newable *& ptr { value };
			delete ptr;
			ptr = nullptr;
		}
		m_data.clear();
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Entity::remove(hash_t code)
	{
		auto it { m_data.find(code) };
		if (it != m_data.end())
		{
			Newable *& ptr { it->second };
			delete ptr;
			ptr = nullptr;
			m_data.erase(it);
			return true;
		}
		return false;
	}

	bool Entity::remove(const String & name)
	{
		return remove(name.hash());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Entity::addByCode(hash_t code, void * value)
	{
		return ((m_data.find(code) == m_data.end())
			? m_data.insert({ code, static_cast<Newable *>(value) }).first->second
			: nullptr
			);
	}

	void * Entity::addByName(const String & name, void * value)
	{
		return ((m_data.find(name.hash()) == m_data.end())
			? addByCode(name.hash(), value)
			: nullptr
			);
	}

	void * Entity::addByName(const String & name)
	{
		return ((m_data.find(name.hash()) == m_data.end())
			? addByCode(name.hash(), ML_Registry.generate(name))
			: nullptr
			);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Newable * Entity::getByCode(hash_t value)
	{
		auto it { m_data.find(value) };
		return ((it != cend()) ? it->second : nullptr);
	}

	const Newable * Entity::getByCode(hash_t value) const
	{
		auto it { m_data.find(value) };
		return ((it != cend()) ? it->second : nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Newable * Entity::getByName(const String & value)
	{
		return getByCode(value.hash());
	}

	const Newable * Entity::getByName(const String & value) const
	{
		return getByCode(value.hash());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}