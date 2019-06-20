#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/INewable.hpp>
#include <ML/Core/IWritable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Entity final
		: public INewable
		, public IDisposable
		, public IReadable
		, public IWritable
		, public INonCopyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename INewable *;
		using map_type		= typename HashMap<size_t, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Entity();
		~Entity();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;

	public:
		// Find Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component> 
		inline iterator find()
		{
			return (iterator)(m_map.find(typeid(Component).hash_code()));
		}

		template <class Component>
		inline const_iterator find() const
		{
			return (const_iterator)(m_map.find(typeid(Component).hash_code()));
		}

		// Add Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component>
		inline Component * add()
		{
			return ((this->find<Component>() == this->end())
				? (reinterpret_cast<Component *>(m_map.insert({ 
						typeid(Component).hash_code(), 
						new Component() 
					}).first->second))
				: (nullptr)
			);
		}

		template <class Component>
		inline Component * add(Component * value)
		{
			return ((this->find<Component>() == this->end())
				? (reinterpret_cast<Component *>(m_map.insert({
						typeid(Component).hash_code(),
						value
					}).first->second))
				: (nullptr)
			);
		}

		template <
			class Component,
			class ... Args
		> inline Component * add(Args && ... args)
		{
			return add<Component>(new Component(std::forward<Args>(args)...));
		}

		// Get Component
		/* * * * * * * * * * * * * * * * * * * * */
		template <class Component>
		inline Component * get()
		{
			iterator it;
			return (((it = this->find<Component>()) != this->end())
				? (reinterpret_cast<Component *>(it->second))
				: (nullptr)
			);
		}

		template <class Component>
		inline const Component * get() const
		{
			const_iterator it;
			return (((it = this->find<Component>()) != this->cend())
				? (reinterpret_cast<const Component *>(it->second))
				: (nullptr)
			);
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()				{ return m_map.begin();  }
		inline const_iterator	begin()		const	{ return m_map.begin();  }
		inline const_iterator	cbegin()	const	{ return m_map.cbegin(); }
		inline iterator			end()				{ return m_map.end();	 }
		inline const_iterator	end()		const	{ return m_map.end();	 }
		inline const_iterator	cend()		const	{ return m_map.cend();	 }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		map_type m_map;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_