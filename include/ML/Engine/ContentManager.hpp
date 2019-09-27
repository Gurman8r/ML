#ifndef _ML_CONTENT_MANAGER_HPP_
#define _ML_CONTENT_MANAGER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>
#include <ML/Engine/Registry.hpp>

#define ML_Content ::ml::ContentManager::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Global bank of shared resources.
	// Anything can be stored in Content as long as it derives I_Newable.
	struct ML_ENGINE_API ContentManager final
		: public I_Disposable
		, public I_Singleton<ContentManager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using ObjectDatabase = Tree<String, I_Newable *>;	// Map of String to Object
		using TypeDatabase = HashMap<hash_t, ObjectDatabase>;	// Map of TypeID to ObjectMap

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (auto & types : m_data)
			{
				for (auto & elem : types.second)
				{
					if (elem.second)
					{
						delete elem.second;
					}
				}
				types.second.clear();
			}
			m_data.clear();
			return m_data.empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline ObjectDatabase & data(hash_t index)
		{
			TypeDatabase::iterator it;
			return (((it = m_data.find(index)) != m_data.end())
				? it->second
				: m_data.insert({ index, {} }).first->second
			);
		}
		
		inline const ObjectDatabase & data(hash_t index) const
		{
			TypeDatabase::const_iterator it;
			return (((it = m_data.find(index)) != m_data.cend())
				? it->second
				: m_data.insert({ index, {} }).first->second
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline ObjectDatabase & data()
		{
			static ObjectDatabase & temp { data(typeof<T>().hash) };
			return temp;
		}

		template <class T> inline const ObjectDatabase & data() const
		{
			static const ObjectDatabase & temp { data(typeof<T>().hash) };
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			return ((!get<T>(name))
				? insert(name, new T { std::forward<Args>(args)... })
				: nullptr
			);
		}

		inline I_Newable * insert(hash_t code, const String & name, void * value)
		{
			return static_cast<I_Newable *>(data(code).insert({
				name, (I_Newable *)value
			}).first->second);
		}

		template <class T> inline T * insert(const String & name, T * value)
		{
			return static_cast<T *>(data<T>().insert({
				name, value
			}).first->second);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline I_Newable * generate(const String & type, const String & name)
		{
			if (const hash_t * code { ML_Registry.get_code(type) })
			{
				if (data(*code).find(name) == data(*code).end())
				{
					return data(*code).insert({ 
						name, (I_Newable *)ML_Registry.generate(*code) 
					}).first->second;
				}
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool destroy(hash_t code, const String & name)
		{
			ObjectDatabase::iterator it { data(code).find(name) };
			if (it != data(code).end())
			{
				if (it->second) { delete it->second; }
				data(code).erase(it);
				return true;
			}
			return false;
		}

		template <class T> inline bool destroy(const String & name)
		{
			return destroy(typeof<T>().hash, name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool exists(hash_t code, const String & name)
		{
			return data(code).find(name) != data(code).end();
		}

		template <class T> inline bool exists(const String & name) const
		{
			return exists(typeof<T>::hash, name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline T * get(const String & name)
		{
			ObjectDatabase::iterator it;
			return (((it = data<T>().find(name)) != data<T>().end())
				? static_cast<T *>(it->second)
				: nullptr
			);
		}

		template <class T> inline const T * get(const String & name) const
		{
			ObjectDatabase::const_iterator it;
			return (((it = data<T>().find(name)) != data<T>().end())
				? static_cast<const T *>(it->second)
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline List<String> get_keys() const
		{
			List<String> temp;
			temp.reserve(data<T>().size());
			for (const auto & pair : data<T>())
			{
				temp.push_back(pair.first);
			}
			return temp;
		}

		template <class T> inline String get_name(const T * value) const
		{
			const int32_t i { get_index_of<T>(value) };
			return (i >= 0) ? get_keys<T>()[(hash_t)i] : String();
		}

		template <class T> inline auto get_iter_at_index(int32_t index) const
			-> ObjectDatabase::const_iterator
		{
			if ((index >= 0) && ((hash_t)index < data<T>().size()))
			{
				auto it = data<T>().cbegin();
				for (int32_t i = 0; i < index; i++)
				{
					if ((++it) == data<T>().cend()) { break; }
				}
				return it;
			}
			return data<T>().cend();
		}

		template <class T> inline const T * find_by_index(int32_t index) const
		{
			ObjectDatabase::const_iterator it;
			return (((it = get_iter_at_index<T>(index)) != data<T>().end())
				? static_cast<const T *>(it->second)
				: nullptr
			);
		}

		template <class T> inline int32_t get_index_of(const T * value) const
		{
			int32_t index = 0;
			for (const auto & pair : data<T>())
			{
				if (pair.second == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<ContentManager>;
		ContentManager();
		~ContentManager();
		mutable TypeDatabase m_data; // The Data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_MANAGER_HPP_