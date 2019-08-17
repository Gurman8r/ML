#ifndef _ML_METADATA_HPP_
#define _ML_METADATA_HPP_

#include <ML/Core/I_Disposable.hpp>
#include <ML/Engine/MetadataValue.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// A collection of MetadataValues
	struct Metadata final 
		: public I_Newable
		, public I_Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename Metadata;
		using value_type		= typename MetadataValue;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using map_type			= typename Tree<String, pointer>;
		using init_type			= typename Initializer<Pair<String, value_type>>;
		using pair_type			= typename map_type::value_type;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Metadata() 
			: m_data()
		{
		}

		explicit Metadata(const map_type & values)
			: m_data(values)
		{
		}

		Metadata(const Metadata & copy)
			: m_data(copy.m_data)
		{
		}

		~Metadata() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			for (pair_type & pair : m_data)
			{
				if (pair.second) delete pair.second;
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
		> inline T getData(const String & value, const T dv, const Tree<String, T> & m) const
		{
			if (!m.empty())
			{
				if (const String str = alg::to_lower(getData(value, String())))
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

	inline ML_SERIALIZE(Ostream & out, const Metadata & value)
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