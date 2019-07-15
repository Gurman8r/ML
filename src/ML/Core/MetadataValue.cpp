#include <ML/Core/MetadataValue.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MetadataValue::MetadataValue()
		: m_type(META_None)
		, m_data(String())
	{
	}

	MetadataValue::MetadataValue(const bool value)
		: m_type(META_Bool)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const float64_t value)
		: m_type(META_Double)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const float_t value)
		: m_type(META_Float)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const int32_t value)
		: m_type(META_Int)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const String & value)
		: m_type(META_String)
		, m_data(value)
	{
	}
	
	MetadataValue::MetadataValue(const MetadataValue & copy)
		: m_type(copy.m_type)
		, m_data(copy.m_data)
	{
	}

	MetadataValue::~MetadataValue()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}