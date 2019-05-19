#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Preprocessor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Registry final
		: public ITrackable
		, public INonCopyable
	{
		friend class Resources;

	public:
		using value_type	= typename _Elem;
		using pointer		= typename value_type * ;
		using const_pointer = typename const value_type *;
		using map_type		= typename HashMap<String, pointer>;
		using file_map		= typename HashMap<String, String>;
		using pair_type		= typename Pair<String, pointer>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Registry(String && name)
			: m_name(name)
			, m_data()
			, m_files()
		{
		}
		~Registry() {}

		String		m_name;
		map_type	m_data;
		file_map	m_files;

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline void serialize(std::ostream & out) const override
		{
			for (auto & pair : m_files)
			{
				out << std::left << std::setw(12)
					<< pair.first << " \'" << pair.second << "\'"
					<< endl;
			}
			out << endl;
		}

		inline void deserialize(std::istream & in) override
		{
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const String & name() const { return m_name; }

		inline bool empty() const { return m_data.empty(); }

		inline const size_t size() const { return m_data.size(); }

		inline const List<String> keys() const
		{
			List<String> keys;
			keys.reserve(this->size());
			for (auto & pair : (*this))
			{
				keys.push_back(pair.first);
			}
			return keys;
		}

		inline const String getFile(const String & value) const
		{
			file_map::const_iterator it;
			return ((it = m_files.find(value)) != m_files.end())
				? it->second
				: String();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline size_t clean()
		{
			size_t count = 0;
			for (auto it = m_data.begin(); it != m_data.end(); it++)
			{
				if (it->second)
				{
					delete it->second;
					it->second = NULL;
					count++;
				}
			}
			m_data.clear();
			return count;
		}

		inline size_t reload()
		{
			size_t count = 0;
			for (file_map::const_iterator it = m_files.cbegin(); it != m_files.end(); it++)
			{
				const String & name = it->first;
				const String & file = it->second;
				if (name && file)
				{
					if (const String path = ML_FS.getPathTo(file))
					{
						if (pointer temp = get(name))
						{
							count += ((temp->loadFromFile(path))
								? (1U)
								: (Debug::logError("Failed reloading {0}: \'{1}\'", name, path))
							);
						}
					}
				}
			}
			return count;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer insert(const String & name, pointer value)
		{
			return m_data.insert({ name, value }).first->second;
		}

		inline pointer load(const String & name)
		{
			return ((get(name))
				? (NULL)
				: (this->insert(name, new value_type()))
			);
		}

		inline pointer load(const String & name, pointer value)
		{
			return ((!value)
				? (load(name))
				: ((value && (name && !get(name)))
					? (this->insert(name, value))
					: (NULL)
				)
			);
		}

		inline pointer load(const String & name, const String & file)
		{
			if (name && !get(name))
			{
				if (m_files[name] = file)
				{
					if (const String path = ML_FS.getPathTo(file))
					{
						pointer temp = new value_type();
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
					return load(name);
				}
			}
			return NULL;
		}

		template <class ... Args>
		inline pointer load_forward(const String & name, Args && ... args)
		{
			return ((get(name))
				? (NULL)
				: (this->insert(name, new value_type(std::forward<Args>(args)...)))
			);
		}


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(const String & name)
		{
			return m_data.find(name);
		}

		inline const_iterator find(const String & name) const
		{
			return m_data.find(name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const_pointer get(const String & name) const
		{
			const_iterator it;
			return ((name)
				? ((it = find(name)) != end())
					? (it->second)
					: (NULL)
				: (NULL));
		}

		inline pointer get(const String & name)
		{
			iterator it;
			return ((name)
				? ((it = find(name)) != end())
					? (it->second)
					: (NULL)
				: (NULL)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const_iterator getIterAt(const int32_t value) const
		{
			if ((value >= 0) && ((size_t)value < this->size()))
			{
				const_iterator it = this->cbegin();
				for (int32_t i = 0; i < value; i++)
				{
					if ((++it) == this->cend())
					{
						return it;
					}
				}
				return it;
			}
			return this->cend();
		}

		inline const_pointer getByIndex(const int32_t value) const
		{
			const_iterator it;
			return (((it = this->getIterAt(value)) != this->end())
				? (it->second)
				: (NULL));
		}

		inline const int32_t getIndexOf(const_pointer value) const
		{
			int32_t index = 0;
			for (auto it = this->cbegin(); it != this->cend(); it++)
			{
				if (it->second == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}


	public: // Iterators
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()			{ return m_data.begin();  }
		inline iterator			end()			{ return m_data.end();	  }
		inline const_iterator	begin()  const	{ return m_data.begin();  }
		inline const_iterator	end()	 const	{ return m_data.end();	  }
		inline const_iterator	cbegin() const	{ return m_data.cbegin(); }
		inline const_iterator	cend()	 const	{ return m_data.cend();	  }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_