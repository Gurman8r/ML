#ifndef _ML_MEMORY_MANAGER_HPP_
#define _ML_MEMORY_MANAGER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/I_Singleton.hpp>

#define ML_Memory		::ml::MemoryManager::getInstance()
#define ML_new(size)	ML_Memory.allocate(size)
#define ML_delete(ptr)	ML_Memory.deallocate(ptr)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MemoryManager final : public I_Singleton<MemoryManager>
	{	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Record final : public I_NonCopyable
		{
			size_t	index;	// Index
			void *	ptr;	// Value
			size_t	size;	// Size

			Record(size_t index, void * ptr, size_t size);

			friend ML_SERIALIZE(Ostream & out, const Record & value);
		};

		using RecordMap = typename Tree<void *, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * allocate(size_t size);
		void deallocate(void *& ptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto allocID() const -> const size_t & { return m_allocID; }
		inline auto records() const -> const RecordMap & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend I_Singleton<MemoryManager>;

		MemoryManager();
		~MemoryManager();

		RecordMap m_records;
		size_t m_allocID;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_MANAGER_HPP_