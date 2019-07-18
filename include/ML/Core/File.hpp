#ifndef _ML_FILE_HPP_
#define _ML_FILE_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Writable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Stores contents and location of a file
	struct ML_CORE_API File final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Writable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename char;
		using data_type			= typename List<value_type>;
		using iterator			= typename data_type::iterator;
		using const_iterator	= typename data_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		File()
			: m_data()
			, m_path()
		{
		}

		explicit File(const size_t count, const C_String * data)
			: m_data()
			, m_path()
		{
			for (size_t i = 0; i < count; i++)
			{
				String line(data[i]);

				line = String::ReplaceAll(line, "\0", "");

				m_data.insert(end(), line.begin(), line.end());
			}
			m_data.push_back('\0');
		}

		File(const String & data)
			: m_data(data.begin(), data.end())
			, m_path()
		{
		}

		File(const data_type & data)
			: m_data(data)
			, m_path()
		{
		}

		File(const File & copy)
			: m_data(copy.m_data)
			, m_path(copy.m_path)
		{
		}

		~File() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			m_data.clear();
			return empty();
		}

		inline bool loadFromFile(const String & filename) override
		{
			if (Ifstream file { (m_path = filename), std::ios_base::binary })
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

					m_data.push_back('\0');
				}
				file.close();
				return true;
			}
			return !dispose();
		}

		inline bool saveToFile(const String & filename) const override
		{
			if (Ofstream file { filename, std::ios_base::binary })
			{
				file << (*this);
				file.close();
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const
		{ 
			return !this->empty(); 
		}

		inline File & operator=(const String & value)
		{
			return ((*this) = File(value));
		}
		
		inline const value_type & operator[](size_t i) const 
		{ 
			return m_data[i]; 
		}
		
		inline value_type & operator[](size_t i) 
		{ 
			return m_data[i]; 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto at(size_t i) const	-> const value_type &	{ return m_data[i]; }
		inline auto at(size_t i)		-> value_type &			{ return m_data[i]; }
		inline auto c_str()	const		-> C_String				{ return str().c_str(); }
		inline auto data() const		-> const data_type &	{ return m_data; }
		inline auto data()				-> data_type &			{ return m_data; }
		inline auto empty() const		-> const bool			{ return m_data.empty(); }
		inline auto path() const		-> const String &		{ return m_path; }
		inline auto str() const			-> String				{ return String(begin(), end()); }
		inline auto sstr() const		-> SStream				{ return SStream(str()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()			-> iterator					{ return m_data.begin(); }
		inline auto end()			-> iterator					{ return m_data.end(); }
		inline auto begin() const	-> const_iterator			{ return m_data.begin(); }
		inline auto end() const		-> const_iterator			{ return m_data.end(); }
		inline auto cbegin() const	-> const_iterator			{ return m_data.cbegin(); }
		inline auto cend() const	-> const_iterator			{ return m_data.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String		m_path;
		data_type	m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const File & value)
	{
		return out << String(value.begin(), value.end());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_HPP_