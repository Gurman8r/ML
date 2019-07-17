#ifndef _ML_METADATA_VALUE_HPP_
#define _ML_METADATA_VALUE_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct MetadataValue final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum TypeID : int32_t
		{
			META_None = -1,
			META_Bool,
			META_Double,
			META_Float,
			META_Int,
			META_String,

			MAX_METADATA_TYPE
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataValue()
			: m_type(META_None)
			, m_data(String())
		{
		}

		explicit MetadataValue(const bool value)
			: m_type(META_Bool)
			, m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const float64_t value)
			: m_type(META_Double)
			, m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const float32_t value)
			: m_type(META_Float)
			, m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const int32_t value)
			: m_type(META_Int)
			, m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const String & value)
			: m_type(META_String)
			, m_data(value)
		{
		}

		MetadataValue(const MetadataValue & copy)
			: m_type(copy.m_type)
			, m_data(copy.m_data)
		{
		}

		~MetadataValue()
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline MetadataValue & operator=(const T & value)
		{
			return ((*this) = MetadataValue(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto asBool	()	const -> bool			{ return alg::to_bool(getData()); }
		inline auto asDouble()	const -> float64_t		{ return alg::to_double(getData()); }
		inline auto asFloat	()	const -> float32_t		{ return alg::to_float(getData()); }
		inline auto asInt	()	const -> int32_t		{ return alg::to_int(getData()); }
		inline auto asString()	const -> String			{ return getData(); }
		inline auto getData	()	const -> const String & { return m_data; }
		inline auto getType	()	const -> TypeID			{ return m_type; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool		() const { return asBool();		}
		inline operator float64_t	() const { return asDouble();	}
		inline operator float32_t	() const { return asFloat();	}
		inline operator int32_t		() const { return asInt();		}
		inline operator String		() const { return asString();	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String	m_data;
		TypeID	m_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const MetadataValue & value)
	{
		switch (value.getType())
		{
		case MetadataValue::META_Bool	: return out << value.asBool();
		case MetadataValue::META_Double	: return out << value.asDouble();
		case MetadataValue::META_Float	: return out << value.asFloat();
		case MetadataValue::META_Int	: return out << value.asInt();
		case MetadataValue::META_String	: return out << value.asString();
		default: 
			return out;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_VALUE_HPP_