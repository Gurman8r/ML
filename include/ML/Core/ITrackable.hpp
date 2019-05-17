#ifndef _ML_I_TRACKABLE_HPP_
#define _ML_I_TRACKABLE_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API ITrackable
		: public ISerializable
	{
	public:
		virtual ~ITrackable() {}

	public:
		inline void * operator	new		 (size_t size) { return ML_new(size); }
		inline void * operator	new[]	 (size_t size) { return ML_new(size); }
		inline void	  operator	delete	 (void * ptr)  { return ML_free(ptr); }
		inline void	  operator	delete[] (void * ptr)  { return ML_free(ptr); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_TRACKABLE_HPP_