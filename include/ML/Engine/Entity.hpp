#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename Tree<hash_t, Trackable *>;

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

		template <class T> inline T * attach(T * value)
		{
			return ((m_data.find(typeof<T>::hash) == m_data.end())
				? static_cast<T *>(this->addByCode(typeof<T>::hash, value))
				: nullptr
			);
		}

		template <class T, class ... Args> inline T * add(Args && ... args)
		{
			return this->attach<T>(new T { std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * get()
		{
			return static_cast<T *>(getByCode(typeof<T>::hash));
		}

		template <class T> inline T const * get() const
		{
			return static_cast<T const *>(getByCode(typeof<T>::hash));
		}

		template <class T> inline T const * cget() const
		{
			return return this->get<T>();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool remove(hash_t code)
		{
			auto it{ m_data.find(code) };
			if (it != m_data.end())
			{
				auto & ptr{ it->second };
				delete ptr;
				ptr = nullptr;
				m_data.erase(it);
				return true;
			}
			return false;
		}

		inline bool remove(String const & name)
		{
			return remove(name.hash());
		}

		template <hash_t H> inline bool remove() { return this->remove(H); }

		template <class T> inline bool remove() { return this->remove<typeof<T>::hash>(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * addByCode(hash_t code, void * value)
		{
			return (m_data.find(code) == m_data.end())
				? m_data.insert({ code, static_cast<Trackable *>(value) }).first->second
				: nullptr;
		}

		inline void * addByName(String const & name, void * value)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? addByCode(name.hash(), value)
				: nullptr
				);
		}

		inline void * addByName(String const & name)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? addByCode(name.hash(), ML_Registry.generate(name))
				: nullptr
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * getByCode(hash_t value)
		{
			auto it{ m_data.find(value) };
			return ((it != cend()) ? it->second : nullptr);
		}

		inline Trackable const * getByCode(hash_t value) const
		{
			auto it{ m_data.find(value) };
			return ((it != cend()) ? it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * getByName(String const & value)
		{
			return getByCode(value.hash());
		}

		inline Trackable const * getByName(String const & value) const
		{
			return getByCode(value.hash());
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