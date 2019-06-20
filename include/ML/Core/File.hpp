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
		void serialize(ostream & out) const override;

	public:
		inline operator bool() const
		{ 
			return !this->empty(); 
		}

		inline File & operator=(const String & value)
		{
			return ((*this) = File(value));
		}
		
		inline const char & operator[](size_t i) const 
		{ 
			return m_data[i]; 
		}
		
		inline char & operator[](size_t i) 
		{ 
			return m_data[i]; 
		}

	public:
		inline auto at(size_t i)const	-> const char &			{ return m_data[i]; }
		inline auto at(size_t i)		-> char &				{ return m_data[i]; }
		inline auto c_str()		const	-> CString				{ return str().c_str(); }
		inline auto data()		const	-> const List<char> &	{ return m_data; }
		inline auto data()				-> List<char> &			{ return m_data; }
		inline auto empty()		const	-> const bool			{ return m_data.empty(); }
		inline auto path()		const	-> const String &		{ return m_path; }
		inline auto str()		const	-> String				{ return String(begin(), end()); }
		inline auto sstr()		const	-> SStream				{ return SStream(str()); }

	public:
		inline auto begin()			-> iterator					{ return m_data.begin(); }
		inline auto end()			-> iterator					{ return m_data.end(); }
		inline auto begin() const	-> const_iterator			{ return m_data.begin(); }
		inline auto end() const		-> const_iterator			{ return m_data.end(); }
		inline auto cbegin() const	-> const_iterator			{ return m_data.cbegin(); }
		inline auto cend() const	-> const_iterator			{ return m_data.cend(); }
		inline auto rbegin()		-> reverse_iterator			{ return m_data.rbegin(); }
		inline auto rend()			-> reverse_iterator			{ return m_data.rend(); }
		inline auto rbegin() const	-> const_reverse_iterator	{ return m_data.rbegin(); }
		inline auto rend() const	-> const_reverse_iterator	{ return m_data.rend(); }
		inline auto crbegin() const -> const_reverse_iterator	{ return m_data.crbegin(); }
		inline auto crend() const	-> const_reverse_iterator	{ return m_data.crend(); }

	private:
		String		m_path;
		List<char>	m_data;
	};
}

#endif // !_ML_FILE_HPP_