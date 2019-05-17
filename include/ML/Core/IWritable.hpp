#ifndef _ML_I_WRITABLE_HPP_
#define _ML_I_WRITABLE_HPP_

#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class IWritable
	{
	public:
		virtual bool saveToFile(const String & filename) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_WRITABLE_HPP_