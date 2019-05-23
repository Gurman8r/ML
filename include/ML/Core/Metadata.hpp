#ifndef _ML_METADATA_HPP_
#define _ML_METADATA_HPP_

#include <ML/Core/MetadataValue.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Metadata
		: public ITrackable
		, public IDisposable
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
		bool dispose() override;
		void serialize(std::ostream & out) const override;

	public:
		inline const_pointer getData(const String & name) const
		{
			const_iterator it;
			return (((it = m_values.find(name)) != m_values.cend()) ? it->second : NULL);
		}

		inline pointer getData(const String & name)
		{
			iterator it;
			return (((it = m_values.find(name)) != m_values.end()) ? it->second : NULL);
		}

		inline Metadata & removeData(const String & name)
		{
			iterator it;
			if ((it = m_values.find(name)) != m_values.end())
			{
				delete it->second;
				m_values.erase(it);
			}
			return (*this);
		}

		template <class ... Args>
		inline MetadataValue & setData(const String & name, Args && ... args)
		{
			iterator it;
			if ((it = m_values.find(name)) == m_values.end())
			{
				it = m_values.insert({ name, new value_type() }).first;
			}
			return ((*it->second) = value_type(std::forward<Args>(args)...));
		}

	private:
		map_type m_values;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_HPP_