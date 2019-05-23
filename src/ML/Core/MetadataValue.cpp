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

	MetadataValue::MetadataValue(const double value)
		: m_type(META_Double)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const float value)
		: m_type(META_Float)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const int32_t value)
		: m_type(META_Int)
		, m_data(std::to_string(value))
	{
	}

	MetadataValue::MetadataValue(const long value)
		: m_type(META_Long)
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

	double MetadataValue::asDouble() const
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

	float MetadataValue::asFloat() const
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

	long MetadataValue::asLong() const
	{
		try
		{
			return std::stol(m_data);
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

	void MetadataValue::serialize(std::ostream & out) const
	{
		switch (m_type)
		{
		case MetadataValue::META_Bool	: out << asBool(); break;
		case MetadataValue::META_Double	: out << asDouble(); break;
		case MetadataValue::META_Float	: out << asFloat(); break;
		case MetadataValue::META_Int	: out << asInt(); break;
		case MetadataValue::META_Long	: out << asLong(); break;
		case MetadataValue::META_String	: out << asString(); break;
		default:
			out << GetTypeName();
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}