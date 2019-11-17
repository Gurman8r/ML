#ifndef _ML_NON_NEWABLE_HPP_
#define _ML_NON_NEWABLE_HPP_

#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// "Memory Leak Insurance"
	// Base class for anything you REALLY don't want to be dynamically allocated.
	// Use in conjunction with Newable to help ensure memory safety.
	struct NonNewable
	{
		inline const std::type_info & get_type_info() const
		{
			return typeid(*this);
		}

		inline friend ML_SERIALIZE(std::ostream & out, const NonNewable & value)
		{
			return out << value.get_type_info().name();
		}

	private:
		inline voidptr_t operator new(size_t) { return nullptr; }
		
		inline voidptr_t operator new[](size_t) { return nullptr; }
		
		inline void operator delete(voidptr_t) { return; }
		
		inline void operator delete[](voidptr_t) { return; }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NON_NEWABLE_HPP_