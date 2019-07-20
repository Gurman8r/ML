#ifndef _ML_META_VALUE_HPP_
#define _ML_META_VALUE_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// A string with builtin conversion functions
	struct MetaValue final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetaValue()
			: m_data(String())
		{
		}

		explicit MetaValue(const bool value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetaValue(const float64_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetaValue(const float32_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetaValue(const int32_t value)
			: m_data(std::to_string(value))
		{
		}

		explicit MetaValue(const String & value)
			: m_data(value)
		{
		}

		MetaValue(const MetaValue & copy)
			: m_data(copy.m_data)
		{
		}

		~MetaValue() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline MetaValue & operator=(const T & value)
		{
			return ((*this) = MetaValue(value));
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

	inline ML_SERIALIZE(Ostream & out, const MetaValue & value)
	{
		return out << value.getData();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_META_VALUE_HPP_