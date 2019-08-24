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
			class T
		> inline iterator find()
		{
			return (iterator)(m_map.find(typeid(T).hash_code()));
		}

		template <
			class T
		> inline const_iterator find() const
		{
			return (const_iterator)(m_map.find(typeid(T).hash_code()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * add()
		{
			return ((this->find<T>() == this->end())
				? (static_cast<T *>(m_map.insert({ 
						typeid(T).hash_code(), 
						new T() 
					}).first->second))
				: (nullptr)
			);
		}

		template <
			class T
		> inline T * add(T * value)
		{
			return ((this->find<T>() == this->end())
				? (static_cast<T *>(m_map.insert({
						typeid(T).hash_code(),
						value
					}).first->second))
				: (nullptr)
			);
		}

		template <
			class T, class ... Args
		> inline T * add(Args && ... args)
		{
			return this->add<T>(new T { std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * get()
		{
			iterator it;
			return (((it = this->find<T>()) != this->end())
				? (static_cast<T *>(it->second))
				: (nullptr)
			);
		}

		template <
			class T
		> inline const T * get() const
		{
			const_iterator it;
			return (((it = this->find<T>()) != this->cend())
				? (static_cast<const T *>(it->second))
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