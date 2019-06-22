#ifndef _ML_METADATA_HPP_
#define _ML_METADATA_HPP_

#include <ML/Core/MetadataValue.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Metadata final
		: public I_Newable
	{
	public:
		using value_type		= typename MetadataValue;
		using pointer			= typename value_type *;
		using const_pointer		= typename const value_type *;
		using reference			= typename value_type &;
		using const_reference	= typename const value_type &;
		using map_type			= typename Map<String, pointer>;
		using pair_type			= typename map_type::value_type;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

	public:
		Metadata();
		explicit Metadata(const map_type & values);
		Metadata(const Metadata & copy);
		~Metadata();

	public:
		inline const_pointer getData(const String & name) const
		{
			const_iterator it;
			return (((it = m_data.find(name)) != this->cend()) ? it->second : nullptr);
		}

		inline pointer getData(const String & name)
		{
			iterator it;
			return (((it = m_data.find(name)) != this->end()) ? it->second : nullptr);
		}

		inline Metadata & removeData(const String & name)
		{
			iterator it;
			if ((it = m_data.find(name)) != this->end())
			{
				delete it->second;
				m_data.erase(it);
			}
			return (*this);
		}

		template <class ... Args>
		inline MetadataValue & setData(const String & name, Args && ... args)
		{
			iterator it;
			if ((it = m_data.find(name)) == m_data.end())
			{
				it = m_data.insert({ name, new value_type() }).first;
			}
			return ((*it->second) = value_type(std::forward<Args>(args)...));
		}

	public:
		inline auto begin	()		 -> iterator		{ return m_data.begin(); }
		inline auto begin	() const -> const_iterator	{ return m_data.begin(); }
		inline auto cbegin	() const -> const_iterator	{ return m_data.cbegin(); }
		inline auto end		()		 -> iterator		{ return m_data.end(); }
		inline auto end		() const -> const_iterator	{ return m_data.end(); }
		inline auto cend	() const -> const_iterator	{ return m_data.cend(); }

	private:
		map_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(ostream & out, const Metadata & value)
	{
		for (const auto & pair : value)
		{
			out << "[" << pair.first << "] | " << (*pair.second) << endl;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_HPP_