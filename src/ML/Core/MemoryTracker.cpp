#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct MemoryTracker::Record
	{
		IObject *	object; // Value of Allocation
		size_t		index;	// Index of Allocation
		size_t		size;	// Size of Allocation

		Record(IObject * object, const size_t index, const size_t size)
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

		friend OStream & operator<<(OStream & out, const MemoryTracker::Record & value)
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

			size_t largest = 0;
			for (auto & pair : m_records)
			{
				const size_t s = String(typeid(*pair.second->object).name()).size();
				if (s >= largest)
				{
					largest = s;
				}
			}
			
			for (RecordMap::const_iterator it = m_records.begin(); it != m_records.end(); ++it)
			{
				cerr << std::left << std::setw(largest)
					<< typeid(*it->second->object).name() << " | " << (*it->second) 
					<< endl;
			}
			cerr << endl;

			Debug::pause(EXIT_FAILURE);

			Debug::fatal("YOUR MEMORY IS LEAKING");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(const size_t size)
	{
		if (IObject * object = static_cast<IObject *>(std::malloc(size)))
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