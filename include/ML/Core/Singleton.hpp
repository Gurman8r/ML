#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <ML/Core/NonCopyable.hpp>
#include <ML/Core/NonNewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct Singleton : public NonCopyable, public NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline T & getInstance()
		{
			static T instance; return instance;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_