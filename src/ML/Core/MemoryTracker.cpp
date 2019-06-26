#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct MemoryTracker::Record
	{
		I_Newable *	object; // Value of Allocation
		size_t		index;	// Index of Allocation
		size_t		size;	// Size of Allocation

		Record(I_Newable * object, const size_t index, const size_t size)
			: object(object)
			, index(index)
			, size(size)
		{
		}

		Record(const Record & copy)
			: object(copy.object)
			, index(copy.index)
			, size(copy.size)
		{
		}

		friend ML_SERIALIZE(Ostream & out, const MemoryTracker::Record & value)
		{
			return out << std::left
				<< " { addr: " << value.object
				<< " | size: " << std::setw(4) << value.size
				<< " | index: " << std::setw(4) << value.index
				<< " }";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker()
		: m_records	()
		, m_recordIndex	(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		if (Debug::log("Deleting Memory Tracker...") && (!m_records.empty()))
		{
			Debug::logError("Final allocations follow:");

			for (const auto & pair : m_records)
			{
				cerr << typeid(pair.second->object).name() << " | " << pair.second << endl;
			}
			
#if ML_DEBUG
			Debug::pause(EXIT_FAILURE);
			Debug::fatal("YOUR MEMORY IS LEAKING");
#endif
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(size_t size)
	{
		if (I_Newable * object = static_cast<I_Newable *>(std::malloc(size)))
		{
			if (m_records.find(object) == m_records.end())
			{
				return m_records.insert({
					(void *)object, new Record(object, m_recordIndex++, size)
				}).first->second->object;
			}
		}
		return nullptr;
	}

	void MemoryTracker::deallocate(void * value)
	{
		RecordMap::iterator it;
		if ((it = m_records.find(value)) != m_records.end())
		{
			// free the object
			std::free(it->second->object);
			
			// delete the record
			delete it->second;
			assert(!(it->second = nullptr) && "Failed deleting MemoryTracker::Record");
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}