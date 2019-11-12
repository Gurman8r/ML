#ifndef _ML_NEWABLE_HPP_
#define _ML_NEWABLE_HPP_

#include <ML/Core/MemoryManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// "Memory Leak Insurance"
	// Base class for anything which might be dynamically allocated.
	// Use in conjunction with NonNewable to help ensure memory safety.
	struct ML_CORE_API Newable
	{
		virtual ~Newable() {}

		inline const std::type_info & get_type_info() const
		{
			return typeid(*this);
		}

		inline friend ML_SERIALIZE(std::ostream & out, const Newable & value)
		{
			return out << value.get_type_info().name();
		}

		inline ptr_t<void> operator new(size_t size) { return ML_new(size); }
		inline ptr_t<void> operator new[](size_t size) { return ML_new(size);  }
		inline void operator delete(ptr_t<void> ptr) { return ML_delete(ptr); }
		inline void operator delete[] (ptr_t<void> ptr) { return ML_delete(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NEWABLE_HPP_