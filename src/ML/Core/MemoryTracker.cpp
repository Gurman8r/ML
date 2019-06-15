#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::Record::Record(void * addr, const size_t indx, const size_t size)
		: addr	(addr)
		, indx	(indx)
		, size	(size)
	{
	}

	MemoryTracker::Record::Record(const Record & copy)
		: addr	(copy.addr)
		, indx	(copy.indx)
		, size	(copy.size)
	{
	}

	OStream & operator<<(OStream & out, const MemoryTracker::Record & value)
	{
		return out 
			<< " { addr: " << value.addr
			<< " | size: " << value.size
			<< " | indx: " << value.indx
			<< " }";
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker()
		: m_records()
		, m_guid(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		if (Debug::log("Deleting Memory Tracker...") && (!m_records.empty()))
		{
			Debug::logError("Final allocations follow:");
			
			for (const_iterator it = m_records.begin(); it != m_records.end(); ++it)
			{
				cerr << typeid(*it->first).name() << " -> " << (it->second) << endl;
			}
			cerr << endl;

			Debug::pause(EXIT_FAILURE);

			Debug::fatal("YOUR MEMORY IS LEAKING");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	IObject * MemoryTracker::newAllocation(const size_t size)
	{
		if (IObject * trackable = static_cast<IObject *>(std::malloc(size)))
		{
			if (m_records.find(trackable) == m_records.end())
			{
				return m_records.insert({
					trackable, Record(trackable, m_guid++, size)
				}).first->first;
			}
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		iterator it;
		if ((it = m_records.find(static_cast<IObject *>(value))) != m_records.end())
		{
			m_records.erase(it);
			std::free(value);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}