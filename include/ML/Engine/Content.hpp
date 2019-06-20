#ifndef _ML_CONTENT_HPP_
#define _ML_CONTENT_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Preprocessor.hpp>

namespace ml
{
	class Resources;

	class Content final
		: public INewable
	{
		friend class Resources;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		using file_map	= typename Map<String, String>;
		using data_map	= typename Map<String, void *>;
		using tree_map	= typename HashMap<size_t, data_map>;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Content() {}
		~Content() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <
			class T
		> inline data_map & data()
		{
			return m_map[typeid(T).hash_code()];
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
			auto makeT = [&]()
			{
				T * temp = (T *)ML_new(sizeof(T));
				(*temp) = T(std::forward<Args>(args)...);
				return temp;
			};

			auto makeT2 = [&]()
			{
				return new T(std::forward<Args>(args)...);
			};

			data_map & d = this->data<T>();
			data_map::iterator it;
			return (((it = d.find(name)) == d.end())
				? this->insert(name, makeT2())
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

		template <
			class T
		> inline const T * get(const String & name) const
		{
			const data_map & d = this->data<T>();
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

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <
			class T
		> inline T * load(const String & name)
		{
			return (this->get(name)
				? static_cast<T *>(nullptr)
				: this->insert(name, new T())
			);
		}

		template <
			class T
		> inline T * load(const String & name, T * value)
		{
			return (!value
				? this->load(name)
				: (value && (name && !this->get(name)))
					? this->insert(name, value)
					: static_cast<T *>(nullptr)
			);
		}

		template <
			class T
		> inline T * load_file(const String & name, const String & file)
		{
			return this->load_file_forward(name, file);
		}

		template <
			class T, class ... Args
		> inline T * load_file_forward(const String & name, const String & file, Args && ... args)
		{
			if (name && !this->get(name))
			{
				if (m_files[name] = file)
				{
					if (const String path = ML_FS.getPathTo(file))
					{
						T temp = new T(std::forward<Args>(args)...);
						if (temp->loadFromFile(path))
						{
							return this->insert(name, temp);
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
					return this->load(name);
				}
			}
			return nullptr;
		}

		template <
			class T, class ... Args
		> inline T * load_forward(const String & name, Args && ... args)
		{
			return (this->get(name)
				? static_cast<T *>(nullptr)
				: this->insert(name, new T(std::forward<Args>(args)...))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		tree_map m_map;
		file_map m_files;
	};
}

#endif // !_ML_CONTENT_HPP_
