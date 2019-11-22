#ifndef _ML_METADATA_HPP_
#define _ML_METADATA_HPP_

#include <ML/Engine/MetadataValue.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// A collection of MetadataValues
	struct Metadata final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename Metadata;
		using value_type		= typename MetadataValue;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using map_type			= typename std::map<String, value_type>;
		using init_type			= typename std::initializer_list<std::pair<String, value_type>>;
		using pair_type			= typename map_type::value_type;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Metadata() 
			: m_data {}
		{
		}

		template <class S> explicit Metadata(const std::map<S, S> & values)
			: m_data {}
		{
			for (auto const & pair : values)
			{
				this->setData(pair);
			}
		}

		Metadata(Metadata const & copy)
			: m_data { copy.m_data }
		{
		}

		~Metadata() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose()
		{
			m_data.clear();
			return m_data.empty();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T = typename String
		> inline const_reference getData(String const & name, T const & dv = T()) const
		{
			auto it{ m_data.find(name) };
			return (it != this->cend()
				? it->second
				: m_data.insert({ name, value_type{ dv } }).first->second
			);
		}

		template <
			class T
		> inline T getData(String const & value, const T dv, const std::map<String, T> & m) const
		{
			if (!m.empty())
			{
				if (const String str = util::to_lower(getData(value, String{})))
				{
					auto it{ m.find(str) };
					if (it != m.cend())
					{
						return it->second;
					}
				}
			}
			return dv;
		}

		inline Metadata & removeData(String const & name)
		{
			auto it{ m_data.find(name) };
			if (it != this->end())
			{
				m_data.erase(it);
			}
			return (*this);
		}

		template <
			class ... Args
		> inline const_reference & setData(String const & name, Args && ... args)
		{
			auto it{ m_data.find(name) };
			if (it == m_data.end())
			{
				it = m_data.insert({ name, value_type {} }).first;
			}
			return ((it->second) = value_type { std::forward<Args>(args)... });
		}

		inline const_reference & setData(const std::pair<String, String> & pair)
		{
			return setData(pair.first, pair.second);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin	()		 -> iterator		{ return m_data.begin(); }
		inline auto begin	() const -> const_iterator	{ return m_data.begin(); }
		inline auto cbegin	() const -> const_iterator	{ return m_data.cbegin(); }
		inline auto end		()		 -> iterator		{ return m_data.end(); }
		inline auto end		() const -> const_iterator	{ return m_data.end(); }
		inline auto cend	() const -> const_iterator	{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: mutable map_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, Metadata const & value)
	{
		for (auto const & pair : value)
		{
			out << "[" << pair.first << "] | " << pair.second << std::endl;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_HPP_