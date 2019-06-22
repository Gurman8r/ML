#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/I_Singleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#define ML_NEW	ML_MemoryTracker.allocate
#define ML_FREE ML_MemoryTracker.deallocate

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MemoryTracker final
		: public I_Singleton<MemoryTracker>
	{	
		friend I_Singleton<MemoryTracker>;

	private:
		struct	Record;
		using	RecordMap = typename HashMap<void *, Record *>;

	private:
		MemoryTracker();
		~MemoryTracker();

	public:
		void *	allocate(size_t size);
		void	deallocate(void * ptr);

	private:
		RecordMap	m_records;
		size_t		m_recordIndex;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_