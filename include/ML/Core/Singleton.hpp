#ifndef _ML_SINGLETON_HPP_
#define _ML_SINGLETON_HPP_

#include <ML/Core/NonCopyable.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct Singleton : public NonCopyable
	{
		static inline T & getInstance()
		{
			static T instance;
			return instance;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SINGLETON_HPP_