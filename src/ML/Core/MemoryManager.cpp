#include <ML/Core/MemoryManager.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
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

			cerr<< std::left
				<< FG::Green << std::setw(6) << "Index"
				<< FG::Cyan << std::setw(sizeof(size_t) * 2) << "Size"
				<< FG::Yellow << std::setw(sizeof(size_t) * 3) << "Address"
				<< FG::Normal << std::setw(10) << "Type"
				<< FMT() << endl;

			for (const auto & [ ptr, rec ] : m_records)
			{
				if (auto obj{ static_cast<Newable *>(rec->ptr) })
				{
					cerr<< std::left
						<< FG::Green << std::setw(6) << rec->index
						<< FG::Cyan << std::setw(sizeof(size_t) * 2) << rec->size
						<< FG::Yellow << std::setw(sizeof(size_t) * 3) << rec->ptr
						<< FG::Normal << std::setw(10) << (obj ? obj->get_type_info().name() : "?")
						<< FMT() << endl;
				}
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