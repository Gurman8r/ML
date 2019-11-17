#include <ML/Core/MemoryManager.hpp>
#include <ML/Core/Trackable.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const std::type_info & MemoryManager::Record::get_type_info() const
	{
		return ((const_ptr_t<Trackable>)this->ptr)->get_type_info();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryManager::MemoryManager()
		: m_current { 0 }
		, m_records {}
	{
	}

	MemoryManager::~MemoryManager()
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
				<< FMT() << std::endl;

			for (const auto & [ ptr, rec ] : m_records)
			{
				std::cerr
					<< std::left
					<< FG::Green << std::setw(6) << rec->index
					<< FG::Cyan << std::setw(sizeof(size_t) * 2) << rec->size
					<< FG::Yellow << std::setw(sizeof(size_t) * 3) << rec->ptr
					<< FG::Normal << std::setw(10) << rec->get_type_info().name()
					<< FMT() << std::endl;
			}

			Debug::pause(EXIT_FAILURE);
		}
#else
		ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	voidptr_t MemoryManager::allocate(size_t size)
	{
		auto ptr { std::malloc(size) };
		return m_records.insert({
			ptr, ::new Record { m_current++, size, ptr }
		}).first->second->ptr;
	}

	void MemoryManager::deallocate(voidptr_t value)
	{
		if (auto it{ m_records.find(value) }; it != m_records.end())
		{
			// free the pointer
			std::free(it->second->ptr);
			it->second->ptr = nullptr;
			
			// delete the record
			::delete it->second;
			it->second = nullptr;
			m_records.erase(it);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}