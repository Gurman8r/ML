#ifndef _ML_META_VALUE_HPP_
#define _ML_META_VALUE_HPP_

#include <ML/Core/Newable.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// A string that can be interpreted as other types
	struct MetadataValue final : public Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataValue()
			: m_data {}
		{
		}

		explicit MetadataValue(bool value)
			: m_data { std::to_string(value) }
		{
		}

		explicit MetadataValue(float64_t value)
			: m_data { std::to_string(value) }
		{
		}

		explicit MetadataValue(float32_t value)
			: m_data { std::to_string(value) }
		{
		}

		explicit MetadataValue(int32_t value)
			: m_data { std::to_string(value) }
		{
		}

		explicit MetadataValue(uint32_t value)
			: m_data { std::to_string(value) }
		{
		}

		explicit MetadataValue(const String & value)
			: m_data { value }
		{
		}

		MetadataValue(const MetadataValue & copy)
			: m_data{ copy.m_data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto asBool()	const -> bool		{ return util::to_bool(asString()); }
		inline auto asDouble()	const -> float64_t	{ return util::to_f64(asString()); }
		inline auto asFloat()	const -> float32_t	{ return util::to_f32(asString()); }
		inline auto asInt()		const -> int32_t	{ return util::to_i32(asString()); }
		inline auto asString()	const -> String		{ return m_data; }
		inline auto asUint()	const -> uint32_t	{ return util::to_u32(asString()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool()		const { return asBool();	}
		inline operator float64_t() const { return asDouble();	}
		inline operator float32_t() const { return asFloat();	}
		inline operator int32_t()	const { return asInt();		}
		inline operator uint32_t()	const { return asUint();	}
		inline operator String()	const { return asString();	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: String m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const MetadataValue & value)
	{
		return out << value.asString();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_META_VALUE_HPP_