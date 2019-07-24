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
			, index	(index)
			, size	(size)
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
				<< (*value.object)
				<< " addr: "  << value.object
				<< " | size: "  << value.size
				<< " | indx: "  << value.index;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker()
		: m_records	()
		, m_index	(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		if (!m_records.empty())
		{
			Debug::logError("Memory leaks detected:");

			for (const auto & pair : m_records)
			{
				cerr << (*pair.second) << endl;
			}
			
#if ML_DEBUG
			Debug::pause(EXIT_FAILURE);
#endif
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(size_t size)
	{
		if (auto value = static_cast<I_Newable *>(std::malloc(size)))
		{
			if (m_records.find(value) == m_records.end())
			{
				return m_records.insert({
					(void *)value, new Record(value, m_index++, size)
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
			it->second = nullptr;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}