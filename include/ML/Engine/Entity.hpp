#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Writable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Entity final
		: public I_Newable
		, public I_Disposable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type = typename I_Newable *;
		using base_type = typename HashMap<hash_t, value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Entity() : m_data {} {}
		~Entity() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (auto & pair : m_data)
			{
				if (pair.second)
				{
					delete pair.second;
					pair.second = nullptr;
				}
			}
			m_data.clear();
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * attach(T * value)
		{
			return ((m_data.find(typeof<T>().hash) == m_data.end())
				? static_cast<T *>(this->addByCode(typeof<T>().hash, value))
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

		inline void * addByCode(hash_t code, void * value)
		{
			return ((m_data.find(code) == m_data.end())
				? m_data.insert({ code, static_cast<value_type>(value) }).first->second
				: nullptr
			);
		}

		inline void * addByName(const String & name, void * value)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? this->addByCode(name.hash(), value)
				: nullptr
			);
		}

		inline void * addByName(const String & name)
		{
			return ((m_data.find(name.hash()) == m_data.end())
				? this->addByCode(name.hash(), ML_Registry.generate(name))
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type getByCode(hash_t value)
		{
			base_type::iterator it { m_data.find(value) };
			return ((it != this->cend()) ? it->second : nullptr);
		}

		inline const value_type getByCode(hash_t value) const
		{
			base_type::const_iterator it { m_data.find(value) };
			return ((it != this->cend()) ? it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type getByName(const String & value)
		{
			return getByCode(value.hash());
		}

		inline const value_type getByName(const String & value) const
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