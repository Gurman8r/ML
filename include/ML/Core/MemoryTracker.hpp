#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/I_Singleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker	_ML MemoryTracker::getInstance()
#define ML_allocate			ML_MemoryTracker.allocate
#define ML_deallocate		ML_MemoryTracker.deallocate

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MemoryTracker final : public I_Singleton<MemoryTracker>
	{	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Record final : public I_NonCopyable
		{
			size_t	index;	// Index
			void *	ptr;	// Value
			size_t	size;	// Size

			Record(size_t index, void * ptr, size_t size);

			friend ML_SERIALIZE(Ostream & out, const Record & value);
		};

		using RecordMap = typename HashMap<void *, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void *	allocate(size_t size);
		void	deallocate(void *& ptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto records() const -> const RecordMap & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend I_Singleton<MemoryTracker>;

		MemoryTracker();
		~MemoryTracker();

		RecordMap	m_records;
		size_t		m_index;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_