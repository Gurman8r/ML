#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::Record::Record(size_t index, void * ptr, size_t size)
		: ptr(ptr)
		, index(index)
		, size(size)
	{
	}

	ML_SERIALIZE(Ostream & out, const MemoryTracker::Record & value)
	{
		return out << std::left
			<< static_cast<I_Newable *>(value.ptr)->get_type_name()
			<< " index: " << value.index
			<< " | size: " << value.size 
			<< " | address: " << value.ptr
			;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker()
		: m_records	()
		, m_index	(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
#if ML_DEBUG
		if (!m_records.empty())
		{
			Debug::logError("Memory leaks detected:");

			for (const auto & pair : m_records)
			{
				cerr << (*pair.second) << endl;
			}

			Debug::pause(EXIT_FAILURE);
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(size_t size)
	{
		void * ptr { std::malloc(size) };
		return m_records.insert({
			ptr, new Record { m_index++, ptr, size }
		}).first->second->ptr;
	}

	void MemoryTracker::deallocate(void *& value)
	{
		RecordMap::iterator it;
		if ((it = m_records.find(value)) != m_records.end())
		{
			// free the ptr
			std::free(it->second->ptr);
			it->second->ptr = value = nullptr;
			
			// delete the record
			delete it->second;
			it->second = nullptr;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}