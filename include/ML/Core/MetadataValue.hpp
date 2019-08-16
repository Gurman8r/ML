#ifndef _ML_META_VALUE_HPP_
#define _ML_META_VALUE_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Can be used to interpret strings as other types
	struct MetadataValue final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataValue()
			: m_data {}
		{
		}

		MetadataValue(bool value)
			: m_data { std::to_string(value) }
		{
		}

		MetadataValue(float64_t value)
			: m_data { std::to_string(value) }
		{
		}

		MetadataValue(float32_t value)
			: m_data { std::to_string(value) }
		{
		}

		MetadataValue(int32_t value)
			: m_data { std::to_string(value) }
		{
		}

		MetadataValue(uint32_t value)
			: m_data { std::to_string(value) }
		{
		}

		MetadataValue(const String & value)
			: m_data { value }
		{
		}

		MetadataValue(const MetadataValue & copy)
			: m_data { copy.m_data }
		{
		}

		~MetadataValue() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline MetadataValue & operator=(const T & value)
		{
			return ((*this) = MetadataValue(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const String & getData() const { return m_data; }

		inline auto asBool()	const -> bool			{ return alg::to_bool(asString()); }
		inline auto asDouble()	const -> float64_t		{ return alg::to_double(asString()); }
		inline auto asFloat()	const -> float32_t		{ return alg::to_float(asString()); }
		inline auto asInt()		const -> int32_t		{ return alg::to_int(asString()); }
		inline auto asString()	const -> String			{ return getData(); }
		inline auto asUint()	const -> uint32_t		{ return alg::to_uint(asString()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool		() const { return asBool();		}
		inline operator float64_t	() const { return asDouble();	}
		inline operator float32_t	() const { return asFloat();	}
		inline operator int32_t		() const { return asInt();		}
		inline operator uint32_t	() const { return asUint();		}
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

#endif // !_ML_META_VALUE_HPP_