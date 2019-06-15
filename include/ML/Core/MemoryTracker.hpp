#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/ISingleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#define ML_new	ML_MemoryTracker.allocate
#define ML_free ML_MemoryTracker.deallocate

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MemoryTracker final
		: public ISingleton<MemoryTracker>
	{	
		friend ISingleton<MemoryTracker>;

	private:
		struct	Record;
		using	RecordMap = typename HashMap<void *, Record *>;

	private:
		MemoryTracker();
		~MemoryTracker();

	public:
		void *	allocate(const size_t size);
		void	deallocate(void * ptr);

	private:
		RecordMap	m_records;
		size_t		m_recordIndex;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_