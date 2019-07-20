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

		MetadataValue()
			: m_data(String())
		{
		}

		explicit MetadataValue(const bool value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const float64_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const float32_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const int32_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetadataValue(const String & value)
			: m_data(value)
		{
		}

		MetadataValue(const MetadataValue & copy)
			: m_data(copy.m_data)
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool		() const { return asBool();		}
		inline operator float64_t	() const { return asDouble();	}
		inline operator float32_t	() const { return asFloat();	}
		inline operator int32_t		() const { return asInt();		}
		inline operator String		() const { return asString();	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: String m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const MetadataValue & value)
	{
		return out << value.getData();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_VALUE_HPP_