#include <ML/Core/MetadataValue.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	bool MetadataValue::asBool() const
	{
		return
			(m_data) &&
			(m_data == "1") ||
			(StringUtility::ToLower(m_data) == "true");
	}

	float64_t MetadataValue::asDouble() const
	{
		try
		{
			return std::stoi(m_data);
		}
		catch (std::invalid_argument &)
		{
			return 0.0;
		}
	}

	float_t MetadataValue::asFloat() const
	{
		try
		{
			return std::stof(m_data);
		}
		catch (std::invalid_argument &)
		{
			return 0.0f;
		}
	}

	int32_t MetadataValue::asInt() const
	{
		try
		{
			return std::stoi(m_data);
		}
		catch (std::invalid_argument &)
		{
			return 0;
		}
	}

	String MetadataValue::asString() const
	{
		return m_data;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}