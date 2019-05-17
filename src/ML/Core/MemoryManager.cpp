#include <ML/Core/MemoryManager.hpp>
#include <ML/Core/Debug.hpp>

#define CHUNK_SIZE	sizeof(ml::Chunk)	// Size of Chunk
#define PTR_SIZE	sizeof(ml::uint8_t *)	// Size of npos pointer
#define OFFSET (CHUNK_SIZE + PTR_SIZE)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void *	MemoryManager::allocate(size_t size)
	{
		//if (!m_head && (m_head = writeChunk(m_used, size))) // create head
		//{
		//	m_prev = m_head;
		//
		//	m_used += size;
		//
		//	return m_head->npos;
		//}
		if (!m_head && (m_head = appendChunk(size))) // create head
		{
			m_tail = m_head;
		
			return m_head->npos;
		}
		else if (Chunk * e = findEmpty(size)) // return free empty newChunk
		{
			if (e->size > size + OFFSET)
			{
				splitChunk(e, size);
			}
			else
			{
				e->size = (size + OFFSET);
				e->free = false;
			}
			return e->npos;
		}
		else if (Chunk * n = appendChunk(size)) // push back new newChunk
		{
			m_tail->next = n;

			n->prev = m_tail;

			m_tail = n;

			return m_tail->npos;
		}
		return NULL;
	}

	bool	MemoryManager::free(void * value)
	{
		if (Chunk * chunk = readChunk(value))
		{
			chunk->free = true;

			mergeNext(chunk);

			mergePrev(chunk);

			return true;
		}
		return false;
	}

	bool	MemoryManager::prime(uint8_t * data, size_t size)
	{
		if (!good() && (data && size))
		{
			m_data = data;
			m_size = size;
			m_used = 0x00;
			m_head = NULL;
			m_tail = NULL;

			for (size_t i = 0; i < size; i++)
			{
				m_data[i] = 0;
			}
		}
		return good();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	void	MemoryManager::serialize(std::ostream & out) const
	{
		out << "Bytes Used: "
			<< "( " << m_used << " / " << m_size << " ) "
			<< "( " << (((double)m_used / m_size) * 100.0) << " % )"
			<< endl;

		if (Chunk * chunk = m_head)
		{
			while (chunk)
			{
				serializeChunk(out, (*chunk));

				chunk = chunk->next;
			}
		}
		else
		{
			out << "Empty" << endl;
		}
	}

	void	MemoryManager::serializeChunk(std::ostream & out, const Chunk & c) const
	{
		const std::type_info & info(typeid(c));
		out << FG::White << "[ " << FG::Gray << info.name() << FG::White << " ]"
			<< FG::White << " { " << FG::Green << "getSize: " << FG::Yellow << std::setw(4) 
			<< (c.size - OFFSET) << " +" << OFFSET 
			<< FG::White << " | " << (c.free ? FG::Cyan : FG::Red) << (c.free ? "free" : "used")
			<< FG::White << " | " << FG::Green << "addr: " << FG::Yellow << (&c)
			<< FG::White << " | " << FG::Green << "prev: " << FG::Yellow << (c.prev)
			<< FG::White << " | " << FG::Green << "next: " << FG::Yellow << (c.next)
			<< FG::White << " | " << FG::Green << "npos: " << FG::Yellow << (&c.npos)
			<< FG::White << " }"
			<< FMT()
			<< endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	bool	MemoryManager::isValidChunk(Chunk * value) const
	{
		return good()
			&& (value) 
			&& (value >= m_head)
			&& (((void *)value) <= (&m_tail->npos));
	}

	void *	MemoryManager::getBytes(size_t addr, size_t total)
	{
		return &m_data[(addr + total)];
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	Chunk * MemoryManager::writeChunk(size_t addr, size_t size)
	{
		if (good() && size)
		{
			const size_t total = (size + OFFSET);

			if ((addr + total) < m_size)
			{
				if (Chunk * chunk = (Chunk *)getBytes(addr, total))
				{
					chunk->size = total;
					chunk->free = false;
					chunk->prev = NULL;
					chunk->next = NULL;

					if (chunk->size > size)
					{
						splitChunk(chunk, size);
					}

					return chunk;
				}
			}
		}
		return NULL;
	}

	Chunk * MemoryManager::appendChunk(size_t size)
	{
		if (Chunk * chunk = writeChunk(m_used, size))
		{
			m_used += (size + OFFSET);

			return chunk;
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Chunk * MemoryManager::findEmpty(size_t size) const
	{
		if (good() && size)
		{
			Chunk * chunk = m_head;
			while (chunk)
			{
				if ((chunk->size >= (size + OFFSET)) && chunk->free)
				{
					return chunk;
				}
				chunk = chunk->next;
			}
			return chunk;
		}
		return NULL;
	}

	Chunk * MemoryManager::readChunk(void * value) const
	{
		if (good() && value)
		{
			if (size_t addr = (size_t)value - (CHUNK_SIZE - PTR_SIZE))
			{
				if (Chunk * chunk = (Chunk *)(addr))
				{
					if (isValidChunk(chunk))
					{
						return chunk;
					}
				}
			}
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	Chunk *	MemoryManager::mergePrev(Chunk * value) const
	{
		if (good())
		{
			if (value && (value->prev && (value->prev)->free))
			{
				(value->prev)->size += value->size;

				(value->prev)->next = value->next;

				if (value->next)
				{
					(value->next)->prev = value->prev;
				}
				
				return value;
			}
		}
		return NULL;
	}

	Chunk *	MemoryManager::mergeNext(Chunk * value) const
	{
		if (good())
		{
			if (value && (value->next && (value->next)->free))
			{
				value->size += (value->next)->size;

				value->next = (value->next)->next;

				if ((value->next)->next)
				{
					((value->next)->next)->prev = value;
				}

				return value;
			}
		}
		return NULL;
	}

	Chunk *	MemoryManager::splitChunk(Chunk * value, size_t size)
	{
		if (good() && (value && size))
		{
			if (Chunk * newChunk = (Chunk *)(&value->npos + size))
			{
				newChunk->size = value->size - size;
				newChunk->free = true;

				newChunk->prev = value;
				newChunk->next = value->next;

				if (newChunk->next)
				{
					(newChunk->next)->prev = newChunk;
				}

				value->size = size;
				value->free = false;
				value->next = newChunk;
				return value;
			}
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}