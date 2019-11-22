#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename std::map<hash_t, Trackable *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity() : m_data{} {}

		~Entity() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose()
		{
			for (auto & [key, value] : m_data)
			{
				Trackable * & ptr{ value };
				delete ptr;
				ptr = nullptr;
			}
			m_data.clear();
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline base_type::iterator find(hash_t code)
		{
			return m_data.find(code);
		}

		template <hash_t H> inline base_type::iterator find()
		{
			return this->find(H);
		}

		template <class T> inline base_type::iterator find()
		{
			return this->find<typeof<T>::hash>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline base_type::const_iterator find(hash_t code) const
		{
			return m_data.find(code);
		}

		template <hash_t H> inline base_type::const_iterator find() const
		{
			return this->find(H);
		}

		template <class T> inline base_type::const_iterator find() const
		{
			return this->find<typeof<T>::hash>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * get(hash_t value)
		{
			if (auto it{ this->find(value) }; it != this->end())
			{
				return it->second;
			}
			return nullptr;
		}

		template <hash_t H> inline Trackable * get()
		{
			return this->get(H);
		}

		template <class T> inline T * get()
		{
			return static_cast<T *>(this->get<typeof<T>::hash>());
		}

		inline Trackable * get(String const & type)
		{
			return this->get(type.hash());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable const * get(hash_t value) const
		{
			if (auto it{ this->find(value) }; it != this->cend())
			{
				return it->second;
			}
			return nullptr;
		}

		template <hash_t H> inline Trackable const * get() const
		{
			return this->get(H);
		}

		template <class T> inline T const * get() const
		{
			return static_cast<T const *>(this->get<typeof<T>::hash>());
		}

		inline void const * get(String const & type) const
		{
			return this->get(type.hash());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool remove(hash_t code)
		{
			if (auto it{ this->find(code) }; it != this->end())
			{
				Trackable *& ptr{ it->second };
				delete ptr;
				m_data.erase(it);
				return true;
			}
			return false;
		}

		template <hash_t H> inline bool remove()
		{
			return this->remove(H);
		}

		template <class T> inline bool remove()
		{
			return this->remove<typeof<T>::hash>();
		}

		inline bool remove(String const & type)
		{
			return this->remove(type.hash());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * add(hash_t code, Trackable * value)
		{
			return ((this->find(code) == this->end())
				? m_data.insert({ code, static_cast<Trackable *>(value) }).first->second
				: nullptr
			);
		}

		inline Trackable * add(String const & type)
		{
			return ((this->find(type.hash()) == this->end())
				? this->add(type.hash(), ML_Registry.generate(type))
				: nullptr
			);
		}

		template <class T> inline T * add(T * value)
		{
			return ((this->find<T>() == this->end())
				? static_cast<T *>(this->add(typeof<T>::hash, value))
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class ... Args> inline T * create(Args && ... args)
		{
			return this->add<T>(new T{ std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()			-> base_type::iterator			{ return m_data.begin(); }
		inline auto begin() const	-> base_type::const_iterator	{ return m_data.begin(); }
		inline auto cbegin() const	-> base_type::const_iterator	{ return m_data.cbegin(); }
		inline auto end()			-> base_type::iterator			{ return m_data.end(); }
		inline auto end() const		-> base_type::const_iterator	{ return m_data.end(); }
		inline auto cend() const	-> base_type::const_iterator	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_