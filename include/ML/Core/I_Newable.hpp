#ifndef _ML_I_NEWABLE_HPP_
#define _ML_I_NEWABLE_HPP_

#include <ML/Core/MemoryManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Base class for anything which might be dynamically allocated.
	// Provides memory leak detection and simple serialization.
	// Use in conjunction with I_NonNewable to help ensure memory safety.
	struct ML_CORE_API I_Newable
	{
		virtual ~I_Newable() {}

		inline const std::type_info & get_type_info() const
		{
			return typeid(*this);
		}

		inline void * operator new		(size_t size) { return ML_new(size); }
		inline void * operator new[]    (size_t size) { return ML_new(size);  }
		inline void	  operator delete   (void * ptr)  { return ML_delete(ptr); }
		inline void	  operator delete[] (void * ptr)  { return ML_delete(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const I_Newable & value)
	{
		return out << value.get_type_info().name();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NEWABLE_HPP_