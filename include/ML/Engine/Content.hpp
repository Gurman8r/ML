#ifndef _ML_CONTENT_HPP_
#define _ML_CONTENT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Metadata.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Readable.hpp>

#define ML_Content ::ml::Content::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Content final
		: public I_Disposable
		, public I_Readable
		, public I_Singleton<Content>
	{
		friend struct I_Singleton<Content>;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		using file_map	= typename Map<String, String>;
		using data_map	= typename Map<String, I_Newable *>;
		using type_map	= typename HashMap<size_t, data_map>;

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Content() : m_data() { }
		~Content() { dispose(); }

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool readMetadata(Metadata & data, Istream & file, String & line) const;
		bool parseMetadata(const Metadata & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline data_map & data(size_t ID)
		{
			type_map::iterator it;
			if ((it = m_data.find(ID)) != m_data.end())
			{
				return it->second;
			}
			else
			{
				return m_data.insert({ ID, data_map() }).first->second;
			}
		}

		template <
			class T
		> inline data_map & data()
		{
			return data(typeid(T).hash_code());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const data_map & data(size_t ID) const
		{
			return m_data.at(ID);
		}

		template <
			class T
		> inline const data_map & data() const
		{
			return m_data.at(typeid(T).hash_code());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * insert(const String & name, T * value)
		{
			data_map & d = this->data<T>();
			return static_cast<T *>(d.insert({ name, value }).first->second);
		}
		
		template <
			class T, class ... Args
		> inline T * create(const String & name, Args && ... args)
		{
			data_map & d = this->data<T>();
			data_map::iterator it;
			return (((it = d.find(name)) == d.end())
				? this->insert(name, new T(std::forward<Args>(args)...))
				: static_cast<T *>(nullptr)
			);
		}

		template <
			class T
		> inline bool erase(const String & name)
		{
			data_map & d = this->data<T>();
			data_map::iterator it;
			if ((it = d.find(name)) != d.end())
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
		> inline const T * get(const String & name) const
		{
			data_map & d = this->data<T>();
			data_map::const_iterator it;
			return (((it = d.find(name)) != d.end())
				? static_cast<const T *>(it->second)
				: static_cast<T *>(nullptr)
			);
		}

		template <
			class T
		> inline T * get(const String & name)
		{
			data_map & d = this->data<T>();
			data_map::iterator it;
			return (((it = d.find(name)) != d.end())
				? static_cast<T *>(it->second)
				: static_cast<T *>(nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline List<String> keys() const
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
		> inline data_map::const_iterator getIterAt(const int32_t value) const
		{
			if ((value >= 0) && ((size_t)value < this->data<T>().size()))
			{
				data_map::const_iterator it = this->data<T>().cbegin();
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
		> inline const T * getByIndex(const int32_t value) const
		{
			data_map::const_iterator it;
			return (((it = this->getIterAt<T>(value)) != this->data<T>().end())
				? static_cast<const T *>(it->second)
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
				if (pair.second == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: type_map m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_HPP_
