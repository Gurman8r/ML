#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final : public Newable, public Disposable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename HashMap<hash_t, Newable *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity();
		~Entity();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

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

		template <class T> inline const T * get() const
		{
			return static_cast<const T *>(getByCode(typeof<T>::hash));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool remove(hash_t code);

		bool remove(const String & name);

		template <hash_t H> inline bool remove() { return this->remove(H); }

		template <class T> inline bool remove() { return this->remove<typeof<T>::hash>(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * addByCode(hash_t code, void * value);

		void * addByName(const String & name, void * value);

		void * addByName(const String & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Newable * getByCode(hash_t value);

		const Newable * getByCode(hash_t value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Newable * getByName(const String & value);

		const Newable * getByName(const String & value) const;

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