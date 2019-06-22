#ifndef _ML_I_NEWABLE_HPP_
#define _ML_I_NEWABLE_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base class for anything which might be dynamically allocated.
	// Provides memory leak detection and simple serialization.
	struct ML_CORE_API I_Newable
	{
		virtual ~I_Newable() {}

		inline friend ML_SERIALIZE(ostream & out, const I_Newable & value)
		{
			return out << typeid(value).name();
		}

		inline void * operator	new		 (size_t size) { return ML_NEW(size); }
		inline void * operator	new[]	 (size_t size) { return ML_NEW(size); }
		inline void	  operator	delete	 (void * ptr)  { return ML_FREE(ptr); }
		inline void	  operator	delete[] (void * ptr)  { return ML_FREE(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NEWABLE_HPP_