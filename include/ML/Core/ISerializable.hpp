#ifndef _ML_I_SERIALIZABLE_HPP_
#define _ML_I_SERIALIZABLE_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API ISerializable
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using TypeInfo = typename const std::type_info &;


	public: // Virtual
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual void serialize(std::ostream & out) const
		{
			out << this->GetTypeName();
		}
		
		inline virtual void deserialize(std::istream & in)
		{
		}


	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline TypeInfo GetTypeInfo()	const { return typeid(*this);				}
		inline CString	GetTypeName()	const { return GetTypeInfo().name();		}
		inline size_t	GetTypeHash()	const { return GetTypeInfo().hash_code();	}

		inline SStream	ToStream()		const { SStream s; s << (*this); return s;	}
		inline String	ToString()		const { return ToStream().str();			}
		inline CString	ToCString()		const { return ToString().c_str();			}


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend std::ostream & operator<<(std::ostream & out, const ISerializable & value)
		{
			value.serialize(out); return out;
		}

		inline friend std::istream & operator>>(std::istream & in, ISerializable & value)
		{
			value.deserialize(in); return in;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_SERIALIZABLE_HPP_