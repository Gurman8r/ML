#ifndef _ML_I_NON_NEWABLE_HPP_
#define _ML_I_NON_NEWABLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base class for anything you REALLY don't want to be dynamically allocated.
	// Use in conjunction with I_Newable to ensure there are no memory leaks.
	// Good idea use when a class/struct has a constructor but no destructor. (e.g. constexpr)
	struct I_NonNewable
	{
		inline friend ML_SERIALIZE(ostream & out, const I_NonNewable & value)
		{
			return out << typeid(value).name();
		}

	private:
		inline void * operator	new		 (size_t size) { return nullptr; }
		inline void * operator	new[]	 (size_t size) { return nullptr; }
		inline void	  operator	delete	 (void * ptr)  { return;  }
		inline void	  operator	delete[] (void * ptr)  { return;  }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NON_NEWABLE_HPP_