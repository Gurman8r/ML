#ifndef _ML_METADATA_VALUE_HPP_
#define _ML_METADATA_VALUE_HPP_

#include <ML/Core/ITrackable.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MetadataValue final
		: public ITrackable
	{
	public:
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

	public:
		MetadataValue();
		explicit MetadataValue(const bool value);
		explicit MetadataValue(const double value);
		explicit MetadataValue(const float value);
		explicit MetadataValue(const int32_t value);
		explicit MetadataValue(const long value);
		explicit MetadataValue(const String & value);
		MetadataValue(const MetadataValue & copy);
		~MetadataValue();

	public:
		bool	asBool	() const;
		double	asDouble() const;
		float	asFloat	() const;
		int32_t	asInt	() const;
		long	asLong	() const;
		String	asString() const;

	public:
		inline auto getData() const -> const String & { return m_data; }
		inline auto getType() const -> const Type & { return m_type; }

	public:
		template <class T>
		inline MetadataValue & operator=(const T & value)
		{
			return ((*this) = MetadataValue(value));
		}

		inline operator bool	() const { return asBool();		}
		inline operator double	() const { return asDouble();	}
		inline operator float	() const { return asFloat();	}
		inline operator int32_t	() const { return asInt();		}
		inline operator long	() const { return asLong();		}
		inline operator String	() const { return asString();	}

	public:
		void serialize(std::ostream & out) const override;

	private:
		String	m_data;
		Type	m_type;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_VALUE_HPP_