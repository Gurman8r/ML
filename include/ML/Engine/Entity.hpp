#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final : public Newable, public Disposable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename HashMap<hash_t, ptr_t<Newable>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity() : m_data{} {}

		~Entity() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (auto & [key, value] : m_data)
			{
				ptr_t<Newable> & ptr{ value };
				delete ptr;
				ptr = nullptr;
			}
			m_data.clear();
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline ptr_t<T> attach(ptr_t<T> value)
		{
			return ((m_data.find(typeof<T>::hash) == m_data.end())
				? static_cast<ptr_t<T>>(this->addByCode(typeof<T>::hash, value))
				: nullptr
			);
		}

		template <class T, class ... Args> inline ptr_t<T> add(Args && ... args)
		{
			return this->attach<T>(new T { std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline ptr_t<T> get()
		{
			return static_cast<ptr_t<T>>(getByCode(typeof<T>::hash));
		}

		template <class T> inline const_ptr_t<T> get() const
		{
			return static_cast<const_ptr_t<T>>(getByCode(typeof<T>::hash));
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

		inline bool remove(const String & name)
		{
			return remove(name.hash());
		}

		template <hash_t H> inline bool remove() { return this->remove(H); }

		template <class T> inline bool remove() { return this->remove<typeof<T>::hash>(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline voidptr_t addByCode(hash_t code, voidptr_t value)
		{
			return (m_data.find(code) == m_data.end())
				? m_data.insert({ code, static_cast<ptr_t<Newable>>(value) }).first->second
				: nullptr;
		}

		inline voidptr_t addByName(const String & name, voidptr_t value)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? addByCode(name.hash(), value)
				: nullptr
				);
		}

		inline voidptr_t addByName(const String & name)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? addByCode(name.hash(), ML_Registry.generate(name))
				: nullptr
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline ptr_t<Newable> getByCode(hash_t value)
		{
			auto it{ m_data.find(value) };
			return ((it != cend()) ? it->second : nullptr);
		}

		inline const_ptr_t<Newable> getByCode(hash_t value) const
		{
			auto it{ m_data.find(value) };
			return ((it != cend()) ? it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline ptr_t<Newable> getByName(const String & value)
		{
			return getByCode(value.hash());
		}

		inline const_ptr_t<Newable> getByName(const String & value) const
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