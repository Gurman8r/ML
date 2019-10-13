#ifndef _ML_FILE_HPP_
#define _ML_FILE_HPP_

#include <ML/Core/Newable.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Stores the contents of a text file
	template <class Ch> struct BasicFile final : public Newable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename Ch;
		using self_type			= typename BasicFile<value_type>;
		using string_type		= typename BasicString<value_type>;
		using sstream_type		= typename string_type::sstream_type;
		using ifstream_type		= typename std::basic_ifstream<value_type>;
		using ofstream_type		= typename std::basic_ofstream<value_type>;
		using list_type			= typename List<value_type>;
		using iterator			= typename list_type::iterator;
		using const_iterator	= typename list_type::const_iterator;
		using pointer			= typename list_type::pointer;
		using reference			= typename list_type::reference;
		using const_pointer		= typename list_type::const_pointer;
		using const_reference	= typename list_type::const_reference;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BasicFile() 
			: m_data()
			, m_path()
		{
		}

		BasicFile(const string_type & data)
			: m_data(data.begin(), data.end())
			, m_path() 
		{
		}

		BasicFile(const list_type & data) 
			: m_data(data)
			, m_path() 
		{
		}

		BasicFile(const self_type & copy)
			: m_data(copy.m_data)
			, m_path(copy.m_path)
		{
		}

		~BasicFile() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			m_data.clear();
			return empty();
		}

		inline bool loadFromFile(const String & filename)
		{
			if (ifstream_type file { (m_path = filename), std::ios_base::binary })
			{
				if (dispose())
				{
					file.seekg(0, std::ios_base::end);
					std::streamsize size;
					if ((size = file.tellg()) > 0)
					{
						file.seekg(0, std::ios_base::beg);
						m_data.resize(static_cast<size_t>(size));
						file.read(&m_data[0], size);
					}
				}
				file.close();
				return true;
			}
			return !dispose();
		}

		inline bool saveToFile(const String & filename) const
		{
			if (ofstream_type file { filename, std::ios_base::binary })
			{
				file << (*this);
				file.close();
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{ 
			return !this->empty(); 
		}

		inline self_type & operator=(const string_type & value)
		{
			return ((*this) = self_type(value));
		}
		
		inline const_reference operator[](size_t i) const 
		{ 
			return m_data[i]; 
		}
		
		inline reference operator[](size_t i) 
		{ 
			return m_data[i]; 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto at(size_t i) const	-> const_reference		{ return m_data[i]; }
		inline auto at(size_t i)		-> value_type &			{ return m_data[i]; }
		inline auto c_str()	const		-> const_pointer		{ return str().c_str(); }
		inline auto data() const		-> const list_type &	{ return m_data; }
		inline auto data()				-> list_type &			{ return m_data; }
		inline auto empty() const		-> bool					{ return m_data.empty(); }
		inline auto path() const		-> const String &		{ return m_path; }
		inline auto str() const			-> string_type			{ return string_type(begin(), end()); }
		inline auto sstr() const		-> sstream_type			{ return sstream_type(str()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator				{ return m_data.begin(); }
		inline auto end()				-> iterator				{ return m_data.end(); }
		inline auto begin() const		-> const_iterator		{ return m_data.begin(); }
		inline auto end() const			-> const_iterator		{ return m_data.end(); }
		inline auto cbegin() const		-> const_iterator		{ return m_data.cbegin(); }
		inline auto cend() const		-> const_iterator		{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String		m_path;
		list_type	m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using File = typename BasicFile<char>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class Ch>
	inline ML_SERIALIZE(std::ostream & out, const BasicFile<Ch> & value)
	{
		return out << value.str();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_HPP_