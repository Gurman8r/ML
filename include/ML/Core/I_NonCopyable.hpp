#ifndef _ML_I_NON_COPYABLE_HPP_
#define _ML_I_NON_COPYABLE_HPP_

#include <ML/Core/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API I_NonCopyable
	{
	protected:
		I_NonCopyable() {}
		~I_NonCopyable() {}

	private:
		I_NonCopyable(const I_NonCopyable &) = delete;
		I_NonCopyable & operator=(const I_NonCopyable &) = delete;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NON_COPYABLE_HPP_