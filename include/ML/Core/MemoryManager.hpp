#ifndef _ML_MEMORY_MANAGER_HPP_
#define _ML_MEMORY_MANAGER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>

#define ML_Memory ::ml::MemoryManager::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MemoryManager final : public Singleton<MemoryManager>
	{	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Record final
		{
			size_t		index;	// Index
			size_t		size;	// Size
			void *	ptr;	// Value

			Record() = default;

			const std::type_info & get_type_info() const;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using RecordMap = typename HashMap<void *, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * allocate(size_t size);

		void deallocate(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto current() const -> size_t { return m_current; }

		inline auto records() const -> const RecordMap & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<MemoryManager>;
		MemoryManager();
		~MemoryManager();
		size_t m_current;
		RecordMap m_records;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_MANAGER_HPP_