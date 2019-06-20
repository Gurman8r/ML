#ifndef _ML_FILE_HPP_
#define _ML_FILE_HPP_

#include <ML/Core/INewable.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IWritable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	class ML_CORE_API File final
		: public INewable
		, public IDisposable
		, public IReadable
		, public IWritable
	{
	public:
		using iterator				= typename List<char>::iterator;
		using const_iterator		= typename List<char>::const_iterator;
		using reverse_iterator		= typename List<char>::reverse_iterator;
		using const_reverse_iterator= typename List<char>::const_reverse_iterator;

	public:
		File();
		File(const size_t count, const CString * data);
		File(const String & data);
		File(const List<char> & data);
		File(const File & copy);
		~File();

	public:
		bool dispose() override;
		
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;
		void serialize(OStream & out) const override;

	public:
		inline File & operator=(const String & value)
		{
			return ((*this) = File(value));
		}

		inline operator		bool()				const	{ return !this->empty(); }
		inline const char & operator[](size_t i)const	{ return m_data[i]; }
		inline char &		operator[](size_t i)		{ return m_data[i]; }

	public:
		inline const char &			at(size_t i) const	{ return m_data[i]; }
		inline char &				at(size_t i)		{ return m_data[i]; }
		inline CString				c_str() const		{ return str().c_str(); }
		inline const List<char> &	data() const		{ return m_data; }
		inline List<char> &			data()				{ return m_data; }
		inline const bool			empty() const		{ return m_data.empty(); }
		inline const String &		path() const		{ return m_path; }
		inline String				str() const			{ return String(begin(), end()); }
		inline SStream				sstr() const		{ return SStream(str()); }

	public:
		inline iterator					begin()			{ return m_data.begin();	}
		inline iterator					end()			{ return m_data.end();		}
		inline const_iterator			begin()  const	{ return m_data.begin();	}
		inline const_iterator			end()	 const	{ return m_data.end();		}
		inline const_iterator			cbegin() const	{ return m_data.cbegin();	}
		inline const_iterator			cend()	 const	{ return m_data.cend();		}
		inline reverse_iterator			rbegin()		{ return m_data.rbegin();	}
		inline reverse_iterator			rend()			{ return m_data.rend();		}
		inline const_reverse_iterator	rbegin()  const	{ return m_data.rbegin();	}
		inline const_reverse_iterator	rend()	  const	{ return m_data.rend();		}
		inline const_reverse_iterator	crbegin() const	{ return m_data.crbegin();	}
		inline const_reverse_iterator	crend()	  const	{ return m_data.crend();	}

	private:
		String		m_path;
		List<char>	m_data;
	};
}

#endif // !_ML_FILE_HPP_