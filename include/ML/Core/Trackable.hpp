#ifndef _ML_TRACKABLE_HPP_
#define _ML_TRACKABLE_HPP_

#include <ML/Core/MemoryManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// "Memory Leak Insurance"
	// Base class for anything which might be dynamically allocated.
	struct ML_CORE_API Trackable
	{
		virtual ~Trackable() {}

		inline const std::type_info & get_type_info() const { return typeid(*this); }

		inline void * operator new(size_t size) { return ML_Memory.allocate(size); }

		inline void * operator new[](size_t size) { return ML_Memory.allocate(size); }

		inline void operator delete(void * ptr) { return ML_Memory.deallocate(ptr); }
		
		inline void operator delete[](void * ptr) { return ML_Memory.deallocate(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRACKABLE_HPP_