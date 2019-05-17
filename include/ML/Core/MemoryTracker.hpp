#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/ISingleton.hpp>
#include <ML/Core/ISerializable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#define ML_new(size) ML_MemoryTracker.newAllocation(size)
#define ML_free(ptr) ML_MemoryTracker.freeAllocation(ptr)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ITrackable;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MemoryTracker final
		: public ISerializable
		, public ISingleton<MemoryTracker>
	{	
		friend ISingleton<MemoryTracker>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		struct Record final : public ISerializable
		{
			void *	addr; // Address
			size_t	indx; // Index
			size_t	size; // Size

			Record(void * addr, const size_t indx, const size_t size);
			Record(const Record & copy);
			void serialize(std::ostream & out) const override;
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using record_map		= typename HashMap<ITrackable *, Record>;
		using iterator			= typename record_map::iterator;
		using const_iterator	= typename record_map::const_iterator;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		MemoryTracker();
		~MemoryTracker();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		ITrackable * newAllocation(const size_t size);

		void freeAllocation(void * ptr);

		void serialize(std::ostream & out) const override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		record_map	m_records;
		size_t		m_guid;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_