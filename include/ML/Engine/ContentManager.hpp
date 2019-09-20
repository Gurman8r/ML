#ifndef _ML_CONTENT_MANAGER_HPP_
#define _ML_CONTENT_MANAGER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

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

		bool dispose() override;
		
		ObjectDatabase & at(hash_t index);
		
		const ObjectDatabase & at(hash_t index) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline List<hash_t> & get_codes() const
		{ 
			static List<hash_t> codes;
			return codes;
		}

		template <class T> inline hash_t get_hash() const
		{
			static List<hash_t> & cache { get_codes() };
			const hash_t code { typeof<T>().hash() };
			if (std::find(cache.cbegin(), cache.cend(), code) == cache.cend())
			{
				cache.push_back(code);
			}
			return code;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> inline ObjectDatabase & data()
		{
			static ObjectDatabase & temp { at(get_hash<T>()) };
			return temp;
		}

		template <class T> inline const ObjectDatabase & data() const
		{
			static const ObjectDatabase & temp { at(get_hash<T>()) };
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

		template <class T> inline T * insert(const String & name, T * value)
		{
			return static_cast<T *>(data<T>().insert({
				name, value
			}).first->second);
		}

		template <class T> inline bool erase(const String & name)
		{
			ObjectDatabase::iterator it;
			if ((it = data<T>().find(name)) != data<T>().end())
			{
				if (it->second) { delete it->second; }
				data<T>().erase(it);
				return true;
			}
			return false;
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
		ContentManager() : m_data() {}
		~ContentManager() { dispose(); }
		mutable TypeDatabase m_data; // The Data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_MANAGER_HPP_