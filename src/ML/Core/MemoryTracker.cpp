#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/ITrackable.hpp>
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

	void MemoryTracker::Record::serialize(std::ostream & out) const
	{
		out << " { addr: " << addr
			<< " | size: " << size
			<< " | indx: " << indx
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
			
			cerr << (*this);

			Debug::pause(EXIT_FAILURE);
			
			//Debug::fatal("Memory Leaks Detected");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ITrackable * MemoryTracker::newAllocation(const size_t size)
	{
		if (ITrackable * trackable = static_cast<ITrackable *>(std::malloc(size)))
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
		if ((it = m_records.find(static_cast<ITrackable *>(value))) != m_records.end())
		{
			m_records.erase(it);
			std::free(value);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void MemoryTracker::serialize(std::ostream & out) const
	{
		for (const_iterator it = m_records.begin(); it != m_records.end(); ++it)
		{
			out << std::left
				<< std::setw(15)
				//<< ctti::type_id(it->first).name() << " -> " << (it->second)
				<< it->first->GetTypeName() << " -> " << (it->second) 
				<< endl;
		}
		out << endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}