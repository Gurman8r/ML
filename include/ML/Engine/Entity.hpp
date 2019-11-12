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

		Entity();
		~Entity();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

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

		bool remove(hash_t code);

		bool remove(const String & name);

		template <hash_t H> inline bool remove() { return this->remove(H); }

		template <class T> inline bool remove() { return this->remove<typeof<T>::hash>(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ptr_t<void> addByCode(hash_t code, ptr_t<void> value);

		ptr_t<void> addByName(const String & name, ptr_t<void> value);

		ptr_t<void> addByName(const String & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ptr_t<Newable> getByCode(hash_t value);

		const_ptr_t<Newable> getByCode(hash_t value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ptr_t<Newable> getByName(const String & value);

		const_ptr_t<Newable> getByName(const String & value) const;

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