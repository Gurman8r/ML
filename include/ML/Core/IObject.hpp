#ifndef _ML_I_OBJECT_HPP_
#define _ML_I_OBJECT_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Base class for anything which might be 'new'-ed
	// Provides automatic memory leak detection and simple serialization.
	class ML_CORE_API IObject
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		virtual ~IObject() {}

	public:
		inline virtual void serialize(std::ostream & out) const
		{
			out << typeid(*this).name();
		}

		inline friend std::ostream & operator<<(std::ostream & out, const IObject & value)
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

#endif // !_ML_I_OBJECT_HPP_