#ifndef _ML_NEWABLE_HPP_
#define _ML_NEWABLE_HPP_

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

		inline voidptr_t operator new(size_t size) { return ML_Memory.allocate(size); }

		inline voidptr_t operator new[](size_t size) { return ML_Memory.allocate(size); }

		inline void operator delete(voidptr_t ptr) { return ML_Memory.deallocate(ptr); }
		
		inline void operator delete[](voidptr_t ptr) { return ML_Memory.deallocate(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NEWABLE_HPP_