#ifndef _ML_MEMORY_MANAGER_HPP_
#define _ML_MEMORY_MANAGER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>

#define ML_Memory		::ml::MemoryManager::getInstance()
#define ML_new(size)	ML_Memory.allocate(size)
#define ML_delete(ptr)	ML_Memory.deallocate(ptr)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MemoryManager final : public Singleton<MemoryManager>
	{	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Record final : public NonCopyable
		{
			size_t		index;	// Index
			voidptr_t	ptr;	// Value
			size_t		size;	// Size

			Record(size_t index, voidptr_t ptr, size_t size);

			friend ML_SERIALIZE(std::ostream & out, const Record & value);
		};

		using RecordMap = typename HashMap<voidptr_t, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		voidptr_t allocate(size_t size);

		void deallocate(voidptr_t & ptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto currentID() const -> const size_t & { return m_currentID; }

		inline auto records() const -> const RecordMap & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<MemoryManager>;

		MemoryManager();
		~MemoryManager();

		RecordMap m_records;
		size_t m_currentID;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_MANAGER_HPP_