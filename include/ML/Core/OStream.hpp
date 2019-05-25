#ifndef _ML_OSTREAM_HPP_
#define _ML_OSTREAM_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API BasicOStream
		: public std::ostream
		, public std::streambuf
	{
	public:
		using base_type = std::ostream;

	public:
		BasicOStream()
			: base_type(this)
		{
		}
		virtual ~BasicOStream() {}

		virtual int32_t overflow(int32_t c) override
		{
			return 0;
		}

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OSTREAM_HPP_
