#ifndef _ML_I_SINGLETON_HPP_
#define _ML_I_SINGLETON_HPP_

#include <ML/Core/I_NonCopyable.hpp>
#include <ML/Core/I_NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct I_Singleton
		: public I_NonCopyable
		, public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline T & getInstance()
		{
			static T instance;
			return instance;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_SINGLETON_HPP_