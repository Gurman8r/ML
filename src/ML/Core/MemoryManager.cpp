#include <ML/Core/MemoryManager.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryManager::Record::Record(size_t index, void * ptr, size_t size)
		: ptr(ptr)
		, index(index)
		, size(size)
	{
	}

	ML_SERIALIZE(Ostream & out, const MemoryManager::Record & value)
	{
		auto object { static_cast<I_Newable *>(value.ptr) };
		return out << std::left
			<< FG::Green << std::setw(6) << value.index
			<< FG::Cyan << std::setw(sizeof(size_t) * 2) << value.size
			<< FG::Yellow << std::setw(sizeof(size_t) * 3) << value.ptr
			<< FG::Normal << std::setw(10) << object->get_type_name()
			<< FMT();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MemoryManager::MemoryManager()
		: m_records {}
		, m_allocID { 0 }
	{
	}

	MemoryManager::~MemoryManager() 
	{
#if ML_DEBUG
		if (!m_records.empty())
		{
			Debug::logError("Memory leaks detected:");

			cerr << std::left
				<< FG::Green << std::setw(6) << "Index"
				<< FG::Cyan << std::setw(sizeof(size_t) * 2) << "Size"
				<< FG::Yellow << std::setw(sizeof(size_t) * 3) << "Address"
				<< FG::Normal << std::setw(10) << "Type"
				<< FMT() << endl;

			for (const auto & pair : m_records)
			{
				cerr << (*pair.second) << endl;
			}

			Debug::pause(EXIT_FAILURE);
		}
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * MemoryManager::allocate(size_t size)
	{
		void * ptr { std::malloc(size) };
		return m_records.insert({
			ptr, new Record { m_allocID++, ptr, size }
		}).first->second->ptr;
	}

	void MemoryManager::deallocate(void *& value)
	{
		auto it { m_records.find(value) };
		if (it != m_records.end())
		{
			// free the pointer
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