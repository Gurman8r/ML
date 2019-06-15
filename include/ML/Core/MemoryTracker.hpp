#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/ISingleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#define ML_new(size) ML_MemoryTracker.newAllocation(size)
#define ML_free(ptr) ML_MemoryTracker.freeAllocation(ptr)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class IObject;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MemoryTracker final
		: public ISingleton<MemoryTracker>
	{	
		friend ISingleton<MemoryTracker>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		struct Record final
		{
			void *	addr; // Address
			size_t	indx; // Index
			size_t	size; // Size

			Record(void * addr, const size_t indx, const size_t size);
			Record(const Record & copy);

			friend OStream & operator<<(OStream & out, const Record & value);
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using record_map		= typename HashMap<IObject *, Record>;
		using iterator			= typename record_map::iterator;
		using const_iterator	= typename record_map::const_iterator;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		MemoryTracker();
		~MemoryTracker();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		IObject * newAllocation(const size_t size);

		void freeAllocation(void * ptr);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		record_map	m_records;
		size_t		m_guid;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_