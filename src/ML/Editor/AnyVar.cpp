#include <ML/Editor/AnyVar.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	AnyVar::AnyVar()
		: m_type(T_None)
		, m_data(NULL)
		, m_size(0)
	{
	}

	AnyVar::AnyVar(const AnyVar & copy)
		: m_type(copy.m_type)
		, m_data(copy.m_data)
		, m_size(copy.m_size)
	{
	}

	AnyVar::~AnyVar()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool AnyVar::dispose()
	{
		if (m_data)
		{
			delete[] m_data;
			m_type = T_None;
			m_data = NULL;
			m_size = 0;
		}
		return (!m_data);
	}

	void AnyVar::serialize(std::ostream & out) const
	{
		switch (m_type)
		{
		case T_Int:
			break;
		case T_Float:
			if (const float * f = getFloat())
			{
				for (size_t i = 0; i < m_size; i++)
				{
					out << f[i] << " ";
				}
			}
			break;
		default:
			break;
		}
		
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * AnyVar::allocate(const size_t size)
	{
		if (!m_data && (!m_size && (m_size = size)))
		{
			return m_data = (std::malloc(m_size));
		}
		else
		{
			return NULL;
		}
	}

	float * AnyVar::allocFloat(const size_t size)
	{
		if ((m_type != T_Float) && !m_size && !m_data)
		{
			m_type = T_Float;
			return reinterpret_cast<float *>(allocate(sizeof(float) * size));
		}
		else
		{
			return NULL;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const float * AnyVar::getFloat() const
	{
		if ((m_type == T_Float) && (m_size) && (m_data))
		{
			return reinterpret_cast<const float *>(m_data);
		}
		else
		{
			return NULL;
		}
	}

	const int32_t * AnyVar::getInt() const
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	float * AnyVar::setFloat(const float * value)
	{
		if ((m_type == T_Float) && (m_size) && (m_data))
		{
			for (size_t i = 0; i < m_size; i++)
			{
				reinterpret_cast<float *>(m_data)[i] = value[i];
			}
			return reinterpret_cast<float *>(m_data);
		}
		else
		{
			return NULL;
		}
	}

	int32_t * AnyVar::setInt(const int32_t * value)
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}