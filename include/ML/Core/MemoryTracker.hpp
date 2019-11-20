#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/Singleton.hpp>

#define ML_MemoryTracker ::ml::MemoryTracker::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MemoryTracker final : public Singleton<MemoryTracker>
	{	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		struct Record final
		{
			size_t index;	// Index
			size_t size;	// Size
			void * value;	// Value

			Record() = default;

			std::type_info const & target_type() const;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using RecordTable = typename Dict<void *, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NEVER_INLINE auto current() const -> size_t { return m_current; }

		ML_NEVER_INLINE auto records() const -> RecordTable const & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<MemoryTracker>;
		friend struct Trackable;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MemoryTracker() = default;
		~MemoryTracker();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t		m_current{ 0 };
		RecordTable	m_records{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * allocate(size_t size);

		void deallocate(void * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~Trackable() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NEVER_INLINE std::type_info const & target_type() const
		{
			return typeid(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NEVER_INLINE void * operator new(size_t size)
		{
			return ML_MemoryTracker.allocate(size);
		}

		ML_NEVER_INLINE void * operator new[](size_t size)
		{
			return ML_MemoryTracker.allocate(size);
		}

		ML_NEVER_INLINE void operator delete(void * value)
		{
			return ML_MemoryTracker.deallocate(value);
		}

		ML_NEVER_INLINE void operator delete[](void * value)
		{
			return ML_MemoryTracker.deallocate(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_