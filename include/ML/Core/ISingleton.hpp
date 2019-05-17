#ifndef _ML_I_SINGLETON_HPP_
#define _ML_I_SINGLETON_HPP_

#include <ML/Core/INonCopyable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T>
	class ISingleton
		: public INonCopyable
	{
	public:
		inline static T & getInstance()
		{
			static T instance;
			return instance;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_SINGLETON_HPP_