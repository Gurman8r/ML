#ifndef _ML_I_NON_NEWABLE_HPP_
#define _ML_I_NON_NEWABLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	class INonNewable
	{
		inline void * operator	new		 (size_t size) { return nullptr; }
		inline void * operator	new[]	 (size_t size) { return nullptr; }
		inline void	  operator	delete	 (void * ptr)  { return void();  }
		inline void	  operator	delete[] (void * ptr)  { return void();  }
	};
}

#endif // !_ML_I_NON_NEWABLE_HPP_