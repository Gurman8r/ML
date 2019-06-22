#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Writable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Writable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename I_Newable *;
		using map_type		= typename HashMap<size_t, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity();
		~Entity();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline iterator find()
		{
			return (iterator)(m_map.find(typeid(Component).hash_code()));
		}

		template <
			class Component
		> inline const_iterator find() const
		{
			return (const_iterator)(m_map.find(typeid(Component).hash_code()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * add()
		{
			return ((this->find<Component>() == this->end())
				? (reinterpret_cast<Component *>(m_map.insert({ 
						typeid(Component).hash_code(), 
						new Component() 
					}).first->second))
				: (nullptr)
			);
		}

		template <
			class Component
		> inline Component * add(Component * value)
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * get()
		{
			iterator it;
			return (((it = this->find<Component>()) != this->end())
				? (reinterpret_cast<Component *>(it->second))
				: (nullptr)
			);
		}

		template <
			class Component
		> inline const Component * get() const
		{
			const_iterator it;
			return (((it = this->find<Component>()) != this->cend())
				? (reinterpret_cast<const Component *>(it->second))
				: (nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline iterator			begin()				{ return m_map.begin();  }
		inline const_iterator	begin()		const	{ return m_map.begin();  }
		inline const_iterator	cbegin()	const	{ return m_map.cbegin(); }
		inline iterator			end()				{ return m_map.end();	 }
		inline const_iterator	end()		const	{ return m_map.end();	 }
		inline const_iterator	cend()		const	{ return m_map.cend();	 }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: map_type m_map;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_