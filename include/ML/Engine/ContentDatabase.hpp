#ifndef _ML_CONTENT_DATABASE_HPP_
#define _ML_CONTENT_DATABASE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

#define ML_Content _ML ContentDatabase::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Global bank of shared resources.
	// Anything can be stored in Content as long as it derives I_Newable.
	struct ML_ENGINE_API ContentDatabase final
		: public I_Disposable
		, public I_Singleton<ContentDatabase>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using object_map = Tree<String, I_Newable *>;	// Map of String to Object
		using typeid_map = HashMap<size_t, object_map>;	// Map of TypeID to ObjectMap

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		
		object_map & at(size_t index);
		
		const object_map & at(size_t index) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline List<size_t> & get_codes() const
		{ 
			static List<size_t> codes;
			return codes;
		}

		template <
			class T
		> inline size_t get_hash() const
		{
			static List<size_t> & cache { this->get_codes() };
			const size_t code { typeid(T).hash_code() };
			if (std::find(cache.cbegin(), cache.cend(), code) == cache.cend())
			{
				cache.push_back(code);
			}
			return code;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline object_map & data()
		{
			static object_map & temp
			{ 
				this->at(this->get_hash<T>()) 
			};
			return temp;
		}

		template <
			class T
		> inline const object_map & data() const
		{
			static const object_map & temp 
			{ 
				this->at(this->get_hash<T>())
			};
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			return ((!this->get<T>(name))
				? this->insert(name, new T { std::forward<Args>(args)... })
				: nullptr
			);
		}

		template <
			class T
		> inline T * insert(const String & name, T * value)
		{
			return static_cast<T *>(this->data<T>().insert({
				name, value
			}).first->second);
		}

		template <
			class T
		> inline bool erase(const String & name)
		{
			static object_map::iterator it;
			if ((it = this->data<T>().find(name)) != this->data<T>().end())
			{
				this->data<T>().erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * get(const String & name)
		{
			static object_map::iterator it;
			return (((it = this->data<T>().find(name)) != this->data<T>().end())
				? static_cast<T *>(it->second)
				: nullptr
			);
		}

		template <
			class T
		> inline const T * get(const String & name) const
		{
			static object_map::const_iterator it;
			return (((it = this->data<T>().find(name)) != this->data<T>().end())
				? static_cast<const T *>(it->second)
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline List<String> get_keys() const
		{
			List<String> temp;
			temp.reserve(this->data<T>().size());
			for (const auto & pair : this->data<T>())
			{
				temp.push_back(pair.first);
			}
			return temp;
		}

		template <
			class T
		> inline object_map::const_iterator get_iter_at_index(int32_t index) const
		{
			if ((index >= 0) && ((size_t)index < this->data<T>().size()))
			{
				auto it = this->data<T>().cbegin();
				for (int32_t i = 0; i < index; i++)
				{
					if ((++it) == this->data<T>().cend()) { break; }
				}
				return it;
			}
			return this->data<T>().cend();
		}

		template <
			class T
		> inline const T * find_by_index(int32_t index) const
		{
			object_map::const_iterator it;
			return (((it = this->get_iter_at_index<T>(index)) != this->data<T>().end())
				? static_cast<const T *>(it->second)
				: nullptr
			);
		}

		template <
			class T
		> inline int32_t get_index_of(const T * value) const
		{
			int32_t index = 0;
			for (const auto & pair : this->data<T>())
			{
				if (pair.second == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<ContentDatabase>;

		ContentDatabase() : m_data() {}
		~ContentDatabase() { dispose(); }

		mutable typeid_map m_data;	// The Data

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_DATABASE_HPP_