#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/RegistryManager.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Writable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Writable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename I_Newable *;
		using map_type		= typename HashMap<hash_t, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity() : m_map {} {}
		
		virtual ~Entity() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool dispose() override;
		virtual bool loadFromFile(const String & filename) override;
		virtual bool saveToFile(const String & filename) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline hash_t get_hash()
		{
			return static_cast<hash_t>(typeid(T).hash_code());
		}

		template <class T> inline iterator find()
		{
			return m_map.find(get_hash<T>());
		}

		template <class T> inline const_iterator find() const
		{
			return m_map.find(get_hash<T>());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * attach(hash_t code, void * value)
		{
			return ((code && value)
				? (I_Newable *)m_map.insert({ code, (I_Newable *)value }).first->second
				: nullptr
			);
		}

		template <class T> inline T * attach(T * value)
		{
			return ((this->find<T>() == this->end())
				? static_cast<T *>(this->attach(get_hash<T>(), value))
				: nullptr
			);
		}

		template <
			class T, class ... Args
		> inline T * add(Args && ... args)
		{
			return this->attach<T>(new T { std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * get()
		{
			iterator it { this->find<T>() };
			return ((it != this->end()) ? static_cast<T *>(it->second) : nullptr);
		}

		template <class T> inline const T * get() const
		{
			const_iterator it { this->find<T>() };
			return ((it != this->cend()) ? static_cast<const T *>(it->second) : nullptr);
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