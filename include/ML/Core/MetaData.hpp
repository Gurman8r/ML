#ifndef _ML_METADATA_HPP_
#define _ML_METADATA_HPP_

#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/MetaValue.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// A collection of MetaValues
	struct MetaData final 
		: public I_Newable
		, public I_Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename MetaValue;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using map_type			= typename Map<String, pointer>;
		using pair_type			= typename map_type::value_type;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetaData() 
			: m_data()
		{
		}

		explicit MetaData(const map_type & values)
			: m_data(values)
		{
		}

		MetaData(const MetaData & copy)
			: m_data(copy.m_data)
		{
		}

		~MetaData() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (pair_type & pair : m_data)
			{
				delete pair.second;
			}
			m_data.clear();
			return m_data.empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T = typename String
		> inline const_reference getData(const String & name, const T & dv = T()) const
		{
			const_iterator it;
			return (((it = m_data.find(name)) != this->cend())
				? (*it->second)
				: (*m_data.insert({ name, new value_type(dv) }).first->second)
			);
		}

		template <
			class T
		> inline T getData(const String & value, const T dv, const Map<String, T> & m) const
		{
			if (!m.empty())
			{
				if (const String str = getData(value, String()))
				{
					auto it = m.find(str);
					if (it != m.cend())
					{
						return it->second;
					}
				}
			}
			return dv;
		}

		inline MetaData & removeData(const String & name)
		{
			iterator it;
			if ((it = m_data.find(name)) != this->end())
			{
				delete it->second;
				m_data.erase(it);
			}
			return (*this);
		}

		template <
			class ... Args
		> inline const_reference & setData(const String & name, Args && ... args)
		{
			iterator it;
			if ((it = m_data.find(name)) == m_data.end())
			{
				it = m_data.insert({ name, new value_type() }).first;
			}
			return ((*it->second) = value_type { std::forward<Args>(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin	()		 -> iterator		{ return m_data.begin(); }
		inline auto begin	() const -> const_iterator	{ return m_data.begin(); }
		inline auto cbegin	() const -> const_iterator	{ return m_data.cbegin(); }
		inline auto end		()		 -> iterator		{ return m_data.end(); }
		inline auto end		() const -> const_iterator	{ return m_data.end(); }
		inline auto cend	() const -> const_iterator	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: mutable map_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const MetaData & value)
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