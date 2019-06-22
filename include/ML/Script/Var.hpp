#ifndef _ML_VAR_HPP_
#define _ML_VAR_HPP_

#include <ML/Script/TokenList.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Var final
		: public I_Newable
		, public I_Comparable<Var>
	{
	public:
		enum VarType : int32_t
		{
			Error = -1,

			//- Basic - - -//
			MIN_BASIC,
			
			Void = MIN_BASIC,
			Bool,
			Float,
			Integer,
			Pointer,
			Str,
			MAX_BASIC,

			//- Complex - -//
			MIN_COMPLEX,
			Arr = MIN_COMPLEX,
			Func,
			Struct,
			MAX_COMPLEX,

			//- - - - - - -//
			MAX_VAR_TYPE
		};

	public:
		struct Ptr : public I_Newable
		{
			String		name;
			int32_t		index;

			Ptr();
			Ptr(int32_t index, const String & name);
			Ptr(const Ptr & copy);

			Var * get() const;

			friend ML_SERIALIZE(ostream & out, const Var::Ptr & value);

			inline Var * operator->() const { return get(); }
			inline Var * operator*() const { return get(); }
		};

		static const String TypeNames[Var::MAX_VAR_TYPE];
		
		inline friend ML_SERIALIZE(ostream & out, const VarType & rhs)
		{
			return out << Var::TypeNames[rhs];
		}

	public:
		Var();
		Var(const TokenList & token);
		Var(const int32_t & type, const TokenList & data);
		Var(const Var & other);
		~Var();

	public:
		inline int32_t	getScope()		const { return m_scope; }
		inline int32_t	getTypeID()		const { return m_typeID; }
		inline String	getTypeName()	const { return TypeNames[getTypeID()]; }

		inline Var & setScope(int32_t value)
		{
			m_scope = value;
			return (*this);
		}
		inline Var & setType(const int32_t & value)
		{
			m_changed = !compareType(value);
			m_typeID = value;
			return (*this);
		}

	public: // Check Functions

		bool	compareTokens(const TokenList & value) const;
		bool	compareType(const int32_t & type) const;

		bool	isValid() const;
		bool	isArrayType() const;
		bool	isBasicType() const;
		bool	isBoolType() const;
		bool	isComplexType() const;
		bool	isEmptyValue() const;
		bool	isErrorType() const;
		bool	isFloatType() const;
		bool	isFuncType() const;
		bool	isIntType() const;
		bool	isNameType() const;
		bool	isNullValue() const;
		bool	isNumericType() const;
		bool	isPointerType() const;
		bool	isStringType() const;
		bool	isStructType() const;
		bool	isVoidType() const;

		inline operator bool() const { return !isErrorType(); }

	public: // Data Functions
		bool		boolValue() const;
		TokenList	dataValue() const;
		float		floatValue() const;
		Var			elemValue(size_t i) const;
		String		errorValue() const;
		int32_t		intValue() const;
		List<Var>	listValue() const;
		Ptr			pointerValue() const;
		int32_t		sizeOfValue() const;
		String		stringValue() const;
		String		textValue() const;
		TokenList	tokensValue() const;

	public:
		Var & arrayValue(const TokenList & value);
		Var & boolValue(const bool & value);
		Var & dataValue(const Token & value);
		Var & dataValue(const TokenList & value);
		Var & elemValue(size_t index, const Token & value);
		Var & errorValue(const String & value);
		Var & floatValue(const float & value);
		Var & funcValue(const TokenList & value);
		Var & intValue(const int32_t & value);
		Var & nullValue();
		Var & pointerValue(const Ptr & value);
		Var & stringValue(const String & value);
		Var & structValue(const TokenList & value);
		Var & voidValue();
		
		template <class T, typename ... A>
		inline Var & errorValue(const String & fmt, const T & arg0, const A &... args)
		{
			return errorValue(String::Format(fmt, arg0, (args)...));
		};

		template <class T, typename ... A>
		inline Var & stringValue(const String & fmt, const T & arg0, const A &... args)
		{
			return stringValue(String::Format(fmt, arg0, (args)...));
		};


	public: // Serialization
		Var & print();

		friend ML_SERIALIZE(ostream & out, const Var & value);

	public: // Factory
		static Var makeSingle(const Token & tok);
		static Var makeRecursive(const TokenList & toks);

	public: // Operator Functions
		bool	And(const Var & other) const;	// &&
		bool	Or(const Var & other) const;	// ||
		bool	Equals(const Var & other) const;// ==
		bool	Less(const Var & other) const;	// <

		Var &	Add(const Var & other); // +
		Var &	Div(const Var & other); // -
		Var &	Mod(const Var & other); // %
		Var &	Mul(const Var & other); // *
		Var &	Pow(const Var & other); // ^
		Var &	Sub(const Var & other); // -
		Var &	Set(const Var & other); // =


	public: // I_Comparable
		inline bool equals(const Var & other)	const override { return Equals(other); }
		inline bool lessThan(const Var & other)	const override { return Less(other); }

	public: // Operators
		friend bool operator&&(const Var & lhs, const Var & rhs);
		friend bool operator||(const Var & lhs, const Var & rhs);

		friend Var & operator+=(Var & lhs, const Var & rhs);
		friend Var & operator-=(Var & lhs, const Var & rhs);
		friend Var & operator*=(Var & lhs, const Var & rhs);
		friend Var & operator/=(Var & lhs, const Var & rhs);
		friend Var & operator^=(Var & lhs, const Var & rhs);
		friend Var & operator%=(Var & lhs, const Var & rhs);

		friend Var	operator+(const Var & lhs, const Var & rhs);
		friend Var	operator-(const Var & lhs, const Var & rhs);
		friend Var	operator*(const Var & lhs, const Var & rhs);
		friend Var	operator/(const Var & lhs, const Var & rhs);
		friend Var	operator^(const Var & lhs, const Var & rhs);
		friend Var	operator%(const Var & lhs, const Var & rhs);

		Var & operator=(const Var & rhs);
		Var & operator=(bool value);
		Var & operator=(float value);
		Var & operator=(double value);
		Var & operator=(int32_t value);
		Var & operator=(const Ptr & value);
		Var & operator=(const String & value);
		Var & operator=(CString value);
		Var & operator=(char value);

	private:
		TokenList	m_data;
		int32_t		m_scope;
		int32_t		m_typeID;

		mutable bool m_changed = true;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VAR_HPP_