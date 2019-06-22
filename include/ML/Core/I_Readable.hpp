#ifndef _ML_I_READABLE_HPP_
#define _ML_I_READABLE_HPP_

#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class I_Readable
	{
	public:
		virtual bool loadFromFile(const String & filename) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_READABLE_HPP_