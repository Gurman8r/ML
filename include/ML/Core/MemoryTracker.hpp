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

		using RecordTable = typename HashMap<void *, Record *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto current() const -> size_t { return m_current; }

		inline auto records() const -> RecordTable const & { return m_records; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<MemoryTracker>;
		friend struct Trackable;
		
		size_t		m_current; // Current Allocation
		RecordTable	m_records; // Allocation Records

		MemoryTracker() : m_current{ 0 }, m_records{}
		{
		}

#if (ML_DEBUG)
		~MemoryTracker();
#else
		~MemoryTracker()
		{
			ML_ASSERT("MEMORY LEAKS DETECTED" && m_records.empty());
		}
#endif

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * allocate(size_t size)
		{
			auto value{ ML_IMPL_NEW(size) };
			return m_records.insert({
				value, ::new Record { m_current++, size, value }
			}).first->second->value;
		}

		inline void deallocate(void * value)
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
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~Trackable() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::type_info const & target_type() const { return typeid(*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * operator new(size_t size) { return ML_MemoryTracker.allocate(size); }

		inline void * operator new[](size_t size) { return ML_MemoryTracker.allocate(size); }

		inline void operator delete(void * value) { return ML_MemoryTracker.deallocate(value); }

		inline void operator delete[](void * value) { return ML_MemoryTracker.deallocate(value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	std::type_info const & MemoryTracker::Record::target_type() const
	{
		return (static_cast<Trackable const *>(this->value))->target_type();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_