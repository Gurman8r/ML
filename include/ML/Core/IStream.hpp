#ifndef _ML_ISTREAM_HPP_
#define _ML_ISTREAM_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API BasicIStream
		: public std::istream
		, public std::streambuf
	{
	public:
		using base_type		= std::basic_istream<char>;
		using traits_type	= std::char_traits<char>;
		using self_type		= BasicIStream;

	public:
		BasicIStream()
			: base_type(this)
		{
		}
		~BasicIStream()
		{
		}

	protected:
		virtual inline std::streambuf * setbuf(char * s, std::streamsize n) override
		{
			return NULL;
		}

		virtual inline int32_t overflow(int32_t c) override
		{
			return 0;
		}

		virtual inline int32_t sync() override
		{
			return 0;
		}

		virtual inline int32_t underflow() override
		{
			return 0;
		}

	public:
		template <class T>
		self_type & operator>>(T & t)
		{
			(base_type)(*this) >> t;
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ISTREAM_HPP_
