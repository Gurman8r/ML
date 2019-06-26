#ifndef _ML_METADATA_VALUE_HPP_
#define _ML_METADATA_VALUE_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API MetadataValue final
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Type
		{
			META_None = -1,
			META_Bool,
			META_Double,
			META_Float,
			META_Int,
			META_Long,
			META_String,

			MAX_METADATA_TYPE
		};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataValue();
		explicit MetadataValue(const bool value);
		explicit MetadataValue(const float64_t value);
		explicit MetadataValue(const float_t value);
		explicit MetadataValue(const int32_t value);
		explicit MetadataValue(const long value);
		explicit MetadataValue(const String & value);
		MetadataValue(const MetadataValue & copy);
		~MetadataValue();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	asBool	() const;
		float64_t	asDouble() const;
		float_t	asFloat	() const;
		int32_t	asInt	() const;
		long	asLong	() const;
		String	asString() const;

		inline auto getData() const -> const String & { return m_data; }
		inline auto getType() const -> const Type & { return m_type; }

		template <
			class T
		> inline MetadataValue & operator=(const T & value)
		{
			return ((*this) = MetadataValue(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool	() const { return asBool();		}
		inline operator float64_t	() const { return asDouble();	}
		inline operator float_t	() const { return asFloat();	}
		inline operator int32_t	() const { return asInt();		}
		inline operator long	() const { return asLong();		}
		inline operator String	() const { return asString();	}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String	m_data;
		Type	m_type;

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
		case MetadataValue::META_Long	: return out << value.asLong();
		case MetadataValue::META_String	: return out << value.asString();
		default: 
			return out;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_VALUE_HPP_