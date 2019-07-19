#ifndef _ML_CONTENT_HPP_
#define _ML_CONTENT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/AssetContainer.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Metadata.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Readable.hpp>

#define ML_Content _ML Content::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Content final
		: public I_Disposable
		, public I_Readable
		, public I_Singleton<Content>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using object_map = typename Map<String, AssetContainer *>;
		using typeid_map = typename HashMap<size_t, object_map>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool readMetadata(Metadata & data, Istream & file, String & line) const;
		bool parseMetadata(const Metadata & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline object_map & data(size_t value)
		{
			typeid_map::iterator it;
			if ((it = m_data.find(value)) != m_data.end())
			{
				return it->second;
			}
			else
			{
				return m_data.insert({ value, object_map() }).first->second;
			}
		}

		inline const object_map & data(size_t value) const
		{
			return m_data.at(value);
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline object_map & data()
		{
			return data(typeid(T).hash_code());
		}

		template <
			class T
		> inline const object_map & data() const
		{
			return m_data.at(typeid(T).hash_code());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * insert(const String & name, T * value)
		{
			object_map & d = this->data<T>();
			return static_cast<T *>(d.insert({ 
				name, new AssetContainer { value }
				}).first->second->object
			);
		}
		
		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			object_map & d = this->data<T>();
			object_map::iterator it;
			return (((it = d.find(name)) == d.end())
				? this->insert(name, new T(std::forward<Args>(args)...))
				: static_cast<T *>(nullptr)
			);
		}

		template <
			class T
		> inline bool erase(const String & value)
		{
			object_map & d = this->data<T>();
			object_map::iterator it;
			if ((it = d.find(value)) != d.end())
			{
				delete it->second;
				it->second = nullptr;
				d.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline const T * get(const String & value) const
		{
			object_map & d = this->data<T>();
			object_map::const_iterator it;
			return (((it = d.find(value)) != d.end())
				? static_cast<const T *>(it->second->object)
				: static_cast<T *>(nullptr)
			);
		}

		template <
			class T
		> inline T * get(const String & value)
		{
			object_map & d = this->data<T>();
			object_map::iterator it;
			return (((it = d.find(value)) != d.end())
				? static_cast<T *>(it->second->object)
				: static_cast<T *>(nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline List<String> getKeys() const
		{
			List<String> temp;
			for (const auto & pair : this->data<T>())
			{
				temp.push_back(pair.first);
			}
			return temp;
		}

		template <
			class T
		> inline object_map::const_iterator getIterAt(int32_t value) const
		{
			if ((value >= 0) && ((size_t)value < this->data<T>().size()))
			{
				object_map::const_iterator it = this->data<T>().cbegin();
				for (int32_t i = 0; i < value; i++)
				{
					if ((++it) == this->data<T>().cend())
					{
						return it;
					}
				}
				return it;
			}
			return this->data<T>().cend();
		}

		template <
			class T
		> inline const T * getByIndex(int32_t value) const
		{
			object_map::const_iterator it;
			return (((it = this->getIterAt<T>(value)) != this->data<T>().end())
				? static_cast<const T *>(it->second->object)
				: nullptr
			);
		}

		template <
			class T
		> inline int32_t getIndexOf(const T * value) const
		{
			int32_t index = 0;
			for (const auto & pair : this->data<T>())
			{
				if (pair.second->object == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<Content>;

		Content() : m_data() {}
		~Content() { dispose(); }

		typeid_map m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_HPP_