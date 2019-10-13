#ifndef _ML_NON_COPYABLE_HPP_
#define _ML_NON_COPYABLE_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API NonCopyable
	{
	protected:
		NonCopyable() {}
		~NonCopyable() {}

	private:
		NonCopyable(const NonCopyable &) = delete;
		NonCopyable & operator=(const NonCopyable &) = delete;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NON_COPYABLE_HPP_