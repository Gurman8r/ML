#ifndef _ML_I_NEWABLE_HPP_
#define _ML_I_NEWABLE_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base class for anything which might be dynamically allocated.
	// Provides memory leak detection and simple serialization.
	// Use in conjunction with I_NonNewable to help ensure memory safety.
	struct ML_CORE_API I_Newable
	{
		virtual ~I_Newable() {}

		inline friend ML_SERIALIZE(Ostream & out, const I_Newable & value)
		{
			return out << typeid(value).name();
		}

		inline void * operator new		(size_t size) { return ML_allocate(size);  }
		inline void * operator new[]    (size_t size) { return ML_allocate(size);  }
		inline void	  operator delete   (void * ptr)  { return ML_deallocate(ptr); }
		inline void	  operator delete[] (void * ptr)  { return ML_deallocate(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NEWABLE_HPP_