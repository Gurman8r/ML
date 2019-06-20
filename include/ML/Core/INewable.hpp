#ifndef _ML_I_NEWABLE_HPP_
#define _ML_I_NEWABLE_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base class for anything which might be 'new'-ed
	// Provides memory leak detection and simple serialization.
	class ML_CORE_API INewable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		virtual ~INewable() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual void serialize(OStream & out) const
		{
			out << typeid(*this).name();
		}

		inline friend OStream & operator<<(OStream & out, const INewable & value)
		{
			value.serialize(out);
			return out;
		}

		inline String ToString() const
		{
			SStream s; 
			s << (*this);
			return s.str();
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline void * operator	new		 (size_t size) { return ML_new(size); }
		inline void * operator	new[]	 (size_t size) { return ML_new(size); }
		inline void	  operator	delete	 (void * ptr)  { return ML_free(ptr); }
		inline void	  operator	delete[] (void * ptr)  { return ML_free(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_NEWABLE_HPP_