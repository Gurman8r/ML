#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ML_DEBUG)
	MemoryTracker::~MemoryTracker()
	{
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

			for (auto const & [ ptr, rec ] : m_records)
			{
				std::cerr
					<< std::left
					<< FG::Green << std::setw(6) << rec->index
					<< FG::Cyan << std::setw(sizeof(size_t) * 2) << rec->size
					<< FG::Yellow << std::setw(sizeof(size_t) * 3) << rec->value
					<< FG::Normal << std::setw(10) << rec->target_type().name()
					<< FMT() << std::endl;
			}

			Debug::pause(EXIT_FAILURE);
		}
	}
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}