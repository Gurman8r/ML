#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::type_info const & MemoryTracker::Record::get_rtti() const
	{
		return static_cast<Trackable const *>(value)->get_rtti();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::~MemoryTracker()
	{
#if (ML_DEBUG)
		if (!m_records.empty())
		{
			Debug::logError("MEMORY LEAKS DETECTED");

			std::cerr
				<< std::left
				<< FG::Green << std::setw(6) << "Index"
				<< FG::Cyan << std::setw(sizeof(size_t) * 2) << "Size"
				<< FG::Yellow << std::setw(sizeof(size_t) * 3) << "Address"
				<< FG::Normal << std::setw(10) << "Type"
				<< FMT() << '\n';

			for (auto const & [ ptr, rec ] : m_records)
			{
				std::cerr
					<< std::left
					<< FG::Green << std::setw(6) << rec->index
					<< FG::Cyan << std::setw(sizeof(size_t) * 2) << rec->size
					<< FG::Yellow << std::setw(sizeof(size_t) * 3) << rec->value
					<< FG::Normal << std::setw(10) << rec->get_rtti().name()
					<< FMT() << '\n';
			}

			Debug::pause(EXIT_FAILURE);
		}
#endif
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * MemoryTracker::allocate(size_t size)
	{
		auto value{ ML_IMPL_NEW(size) };
		return m_records.insert({
			value, ::new Record { m_current++, size, value }
		}).first->second->value;
	}

	void MemoryTracker::deallocate(void * value)
	{
		if (auto it{ m_records.find(value) }; it != m_records.end())
		{
			// free the pointer
			ML_IMPL_DELETE(it->second->value);
			it->second->value = nullptr;

			// erase the record
			::delete it->second;
			it->second = nullptr;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}