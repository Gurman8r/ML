#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <ML/Engine/Registry.hpp>
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

		Entity();
		explicit Entity(const String & filename);
		virtual ~Entity();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual bool dispose() override;
		virtual bool loadFromFile(const String & filename) override;
		virtual bool saveToFile(const String & filename) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * addByCode(hash_t code, void * value)
		{
			return ((m_data.find(code) == m_data.end())
				? m_data.insert({ code, static_cast<I_Newable *>(value) }).first->second
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
			iterator it { m_data.find(typeof<T>().hash) };
			return ((it != this->end()) ? static_cast<T *>(it->second) : nullptr);
		}

		template <class T> inline const T * get() const
		{
			const_iterator it { m_data.find(typeof<T>().hash) };
			return ((it != this->cend()) ? static_cast<const T *>(it->second) : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto data() const	-> const map_type & { return m_data; }
		inline auto begin()			-> iterator			{ return m_data.begin(); }
		inline auto begin() const	-> const_iterator	{ return m_data.begin(); }
		inline auto cbegin() const	-> const_iterator	{ return m_data.cbegin(); }
		inline auto end()			-> iterator			{ return m_data.end(); }
		inline auto end() const		-> const_iterator	{ return m_data.end(); }
		inline auto cend() const	-> const_iterator	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: map_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_