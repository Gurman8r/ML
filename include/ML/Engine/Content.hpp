#ifndef _ML_CONTENT_HPP_
#define _ML_CONTENT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Metadata.hpp>

#define ML_Content ml::Content::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct CubeMap;
	struct Entity;
	struct Font;
	struct Image;
	struct Material;
	struct Mesh;
	struct Model;
	struct Script;
	struct Shader;
	struct Sound;
	struct Sprite;
	struct Surface;
	struct Texture;

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
		bool readMetadata(Metadata & data, istream & file, String & line) const;
		bool parseMetadata(const Metadata & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline data_map & data()
		{
			const size_t id { typeid(T).hash_code() };
			
			type_map::iterator it;
			if ((it = m_data.find(id)) != m_data.end())
			{
				return it->second;
			}
			else
			{
				return m_data.insert({ id, data_map() }).first->second;
			}
		}

		template <
			class T
		> inline const data_map & data() const
		{
			return m_data.at(typeid(T).hash_code());
		}

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
			class T, class ... Args
		> inline T * create_from_file(const String & name, const String & file, Args && ... args)
		{
			if (name && !this->get<T>(name))
			{
				if (m_files[name] = file)
				{
					if (const String path = ML_FS.getPathTo(file))
					{
						T * temp = new T(std::forward<Args>(args)...);
						if (temp->loadFromFile(path))
						{
							return this->insert<T>(name, temp);
						}
						else
						{
							delete temp;
							Debug::logError("Failed loading {0}: \'{1}\'", name, path);
						}
					}
				}
				else
				{
					return this->create<T>(name);
				}
			}
			return nullptr;
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
		> inline auto keys() const 
			-> List<String>
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
		> inline auto getIterAt(const int32_t value) const 
			-> data_map::const_iterator
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
		> inline auto getByIndex(const int32_t value) const 
			-> const T *
		{
			data_map::const_iterator it;
			return (((it = this->getIterAt<T>(value)) != this->data<T>().end())
				? static_cast<const T *>(it->second)
				: nullptr
			);
		}

		template <
			class T
		> inline auto getIndexOf(const T * value) const 
			-> int32_t
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
		type_map m_data;
		file_map m_files;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_HPP_
