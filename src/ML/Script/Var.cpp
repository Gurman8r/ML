#include <ML/Script/Var.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/StringUtility.hpp>
#include <ML/Script/Operator.hpp>
#include <ML/Script/Interpreter.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Var::Ptr::Ptr()
		: index(0)
		, name(String())
	{
	}
	
	Var::Ptr::Ptr(int32_t index, const String & name)
		: index(index)
		, name(name)
	{
	}
	
	Var::Ptr::Ptr(const Ptr & copy)
		: index(copy.index)
		, name(copy.name)
	{
	}
	
	Var * Var::Ptr::get() const
	{
		if (StringUtility::IsName(name))
		{
			if (Var * v = ML_Runtime.getVar(index, name))
			{
				return v;
			}
		}
		return nullptr;
	}

	ML_SERIALIZE(ostream & out, const Var::Ptr & value)
	{
		if (Var * var = value.get())
		{
			out << (*var);
		}
		else
		{
			out << (FG::White | BG::Red) << value.name;
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const String Var::TypeNames[Var::MAX_VAR_TYPE] =
	{
		"void",
		"bool",
		"float",
		"int",
		"pointer",
		"string",
		"array"
		"func",
		"struct",
	};

	/* * * * * * * * * * * * * * * * * * * * */

	Var::Var()
		: m_scope(0)
		, m_typeID(Var::Void)
		, m_data(TokenList())
	{
	}

	Var::Var(const TokenList & token)
		: Var()
	{
		(*this) = makeRecursive(token);
	}

	Var::Var(const int32_t & type, const TokenList & data)
		: m_scope(0)
		, m_typeID(type)
		, m_data(data)
	{
	}

	Var::Var(const Var & copy)
		: m_scope(copy.m_scope)
		, m_typeID(copy.m_typeID)
		, m_data(copy.m_data)
	{
	}

	Var::~Var()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Var::compareTokens(const TokenList& value) const
	{
		return tokensValue() == value;
	}

	bool Var::compareType(const int32_t& type) const
	{
		return getTypeID() == type;
	}


	bool Var::isValid() const
	{
		if (m_changed)
		{	m_changed = false;

			switch (getTypeID())
			{
			case Var::Func:		return !isEmptyValue();
			case Var::Bool:		return isBoolType();
			case Var::Float:	return isFloatType();
			case Var::Integer:	return isIntType();
			case Var::Pointer:	return isPointerType();
			
			case Var::Str:
			default:
				return true;
			}
		}
		return true;
	}
		 
	bool Var::isArrayType() const
	{
		return compareType(Var::Arr);
	}
		 
	bool Var::isBasicType() const
	{
		return getTypeID() >= Var::MIN_BASIC && getTypeID() < Var::MAX_BASIC;
	}
		 
	bool Var::isBoolType() const
	{
		return isNameType() && StringUtility::IsBool(textValue());
	}
		 
	bool Var::isComplexType() const
	{
		return getTypeID() >= Var::MIN_COMPLEX && getTypeID() < Var::MAX_COMPLEX;
	}
		 
	bool Var::isEmptyValue() const
	{
		return m_data.empty();
	}
		 
	bool Var::isErrorType() const
	{
		return tokensValue().front(Token::Error);
	}
		 
	bool Var::isFloatType() const
	{
		return tokensValue().front(Token::Float) && StringUtility::IsDecimal(textValue());
	}
		 
	bool Var::isFuncType() const
	{
		return compareType(Var::Func) && isValid();
	}
		 
	bool Var::isIntType() const
	{
		return tokensValue().front(Token::Int) && StringUtility::IsInt(textValue());
	}
		 
	bool Var::isNameType() const
	{
		return m_data.front(Token::Name) && StringUtility::IsName(textValue());
	}
		 
	bool Var::isNullValue() const
	{
		return isVoidType() || tokensValue().empty();
	}

	bool Var::isNumericType() const
	{
		return isBoolType() || isIntType() || isFloatType();
	}
		 
	bool Var::isPointerType() const
	{
		return compareType(Var::Pointer) && isNameType();
	}
		 
	bool Var::isStringType() const
	{
		return compareType(Var::Str) || tokensValue().front(Token::Str);
	}

	bool Var::isStructType() const
	{
		return compareType(Var::Struct);
	}

	bool Var::isVoidType() const
	{
		return compareType(Var::Void) || tokensValue().front(Token::Empty);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool		Var::boolValue() const
	{
		if (isBoolType())
		{
			return StringUtility::ToBool(textValue());
		}
		else if (isIntType())
		{
			return intValue();
		}
		else if (isFloatType())
		{
			return floatValue();
		}
		else if (isStringType())
		{
			return !stringValue().empty();
		}
		else if (isNameType())
		{
			return pointerValue()->boolValue();
		}
		return (!isEmptyValue() && !isErrorType() && !isVoidType());
	}

	TokenList	Var::dataValue() const
	{
		return m_data;
	}

	float		Var::floatValue() const
	{
		return isValid() ? StringUtility::ToFloat(textValue()) : 0;
	}

	Var			Var::elemValue(size_t i) const
	{
		if (!isEmptyValue())
		{
			List<Var> list;
			if (!(list = listValue()).empty())
			{
				if (i < list.size())
				{
					return list[i];
				}
				else
				{
					return Var().errorValue("Var::Arr : Cannot access element at index {0}", i);
				}
			}
			else if (isStringType())
			{
				const String str = stringValue();
				if (i < str.size())
				{
					return Var().stringValue(String(1, str[i]));
				}
			}
			else
			{
				return Var().errorValue("Var::Arr : Cannot access element on type \"{0}\"", 
					getTypeName());
			}
		}
		return Var().errorValue("Var::Arr : Cannot access element on empty data");
	}

	String		Var::errorValue() const
	{
		return isErrorType() ? (textValue()) : String();
	}

	int32_t		Var::intValue() const
	{
		return isValid() ? StringUtility::ToInt(textValue()) : 0;
	}

	List<Var>	Var::listValue() const
	{
		List<Var> out;
		if (!isEmptyValue())
		{
			if (isArrayType())
			{
				for (const Token & t : dataValue())
				{
					out.push_back(makeSingle(t));
				}
			}
		}
		return out;
	}

	Var::Ptr	Var::pointerValue() const
	{
		return Ptr(m_scope, (textValue()));
	}

	int32_t		Var::sizeOfValue() const
	{
		if (isArrayType())
		{
			return (int32_t)tokensValue().size();
		}
		else if (isStringType())
		{
			return (int32_t)stringValue().size();
		}
		else if (isNumericType())
		{
			return intValue();
		}
		return 0;
	}
	
	String		Var::stringValue() const
	{
		return isValid() ? (textValue()) : String();
	}

	String		Var::textValue() const
	{
		return dataValue().str();
	}

	TokenList	Var::tokensValue() const
	{
		if (compareType(Var::Func))
		{
			return dataValue();
		}
		else if (isPointerType())
		{
			if (Var * var = (*pointerValue()))
			{
				return var->tokensValue();
			}
		}
		return dataValue();
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	Var & Var::arrayValue(const TokenList & value)
	{
		return setType(Var::Arr).dataValue(value);
	}
		  
	Var & Var::boolValue(const bool & value)
	{
		return setType(Var::Bool).dataValue(Token(Token::Name, (value ? "true" : "false")));
	}

	Var & Var::dataValue(const Token & value)
	{
		return dataValue(TokenList(value));
	}

	Var & Var::dataValue(const TokenList & value)
	{
		m_changed = !compareTokens(value);
		m_data = value;
		return (*this);
	}

	Var & Var::elemValue(size_t index, const Token & value)
	{
		if (isArrayType() || isStringType())
		{
			if (tokensValue().inRange(index))
			{
				m_changed = true;
				m_data[index] = value;
			}
		}
		return (*this);
	}
		  
	Var & Var::errorValue(const String & value)
	{
		return setType(Var::Error).dataValue({ Token::Error, value });
		//return voidValue().dataValue(Token(Token::Error, value));
	}
		  
	Var & Var::floatValue(const float & value)
	{
		return setType(Var::Float).dataValue({ Token::Float, std::to_string(value) });
	}
		  
	Var & Var::funcValue(const TokenList & value)
	{
		return setType(Var::Func).dataValue(value);
	}
		  
	Var & Var::intValue(const int32_t & value)
	{
		return setType(Var::Integer).dataValue({ Token::Int, std::to_string(value) });
	}
 		  
	Var & Var::nullValue()
	{
		return voidValue().dataValue(TokenList());
	}
		  
	Var & Var::pointerValue(const Ptr & value)
	{
		return setType(Var::Pointer).dataValue({ Token::Name, value.name });
	}
		  
	Var & Var::stringValue(const String & value)
	{
		return setType(Var::Str).dataValue({ Token::Str, value });
	}

	Var & Var::structValue(const TokenList & value)
	{
		return setType(Var::Struct).dataValue(value);
	}
	  
	Var & Var::voidValue()
	{
		return setType(Var::Void).dataValue({ Token::Empty, " " });
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var & Var::print()
	{
		cout << (*this);
		return (*this);
	}

	ML_SERIALIZE(ostream & out, const Var & v)
	{
		out << FMT();

		if (!v.isValid())
		{
			return out << (FG::Black | BG::Red) << v.textValue() << FMT();
		}

		switch (v.getTypeID())
		{
		case Var::Bool: return out 
			<< (FG::Cyan | BG::Black)
			<< (v.boolValue() ? "true" : "false")
			<< FMT();

		case Var::Float: return out 
			<< (FG::Yellow | BG::Black)
			<< v.floatValue() << "f"
			<< FMT();

		case Var::Integer: return out 
			<< (FG::Yellow | BG::Black)
			<< v.intValue()
			<< FMT();

		case Var::Pointer: return out 
			<< (FG::Gray | BG::Black)
			<< v.pointerValue()
			<< FMT();

		case Var::Str: return out 
			<< (FG::Magenta | BG::Black)
			<< '\"' << v.stringValue() << '\"'
			<< FMT();

		case Var::Func: return out 
			<< (FG::White | BG::DarkGray) << "[](" << FMT()
			<< v.listValue()
			<< (FG::White | BG::DarkGray) << ")" << FMT();

		case Var::Arr: return out 
			<< (FG::Black | BG::Yellow) << "[ " << FMT()
			<< v.listValue()
			<< (FG::Black | BG::Yellow) << "]" << FMT();

		case Var::Struct: return out 
			<< (FG::White | BG::DarkGray) << "$(" << FMT()
			<< v.listValue()
			<< (FG::White | BG::DarkGray) << ")" << FMT();

		case Var::Void: return out 
			<< (FG::Black | BG::White) << v.textValue();
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var Var::makeSingle(const Token & tok)
	{
		switch (tok.type)
		{
		case Token::Str:
			return Var().stringValue(tok.data);
		case Token::Int:
			return Var().intValue(std::stoi(tok.data));
		case Token::Float:
			return Var().floatValue(std::stof(tok.data));
		case Token::Name:
			return StringUtility::IsBool(tok.data)
				? Var().boolValue(StringUtility::ToBool(tok.data))
				: Var().pointerValue(Var::Ptr(0, tok.data));
		default:
			return Var().errorValue(tok.str());
		}
	}

	Var Var::makeRecursive(const TokenList & toks)
	{
		switch (toks.size())
		{
		case 0:
			return Var().nullValue();
		case 1:
			return Var::makeSingle(toks.front());
		default:
			return Var().arrayValue(toks);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Var::And(const Var & other) const
	{
		return boolValue() && other.boolValue();
	}
		 
	bool Var::Or(const Var & other) const
	{
		return boolValue() && other.boolValue();
	}
		 
	bool Var::Equals(const Var & other) const
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue() == other.intValue();
			case Var::Float: return intValue() == other.floatValue();
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Integer: return floatValue() == other.intValue();
			case Var::Float: return floatValue() == other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue()->Equals(*(*other.pointerValue()));
			}
			else if (!lp && rp)
			{
				return Equals(*(*other.pointerValue()));
			}
			else if (lp && !rp)
			{
				return pointerValue()->Equals(other);
			}
		}

		return textValue() == other.textValue();
	}
		 
	bool Var::Less(const Var & other) const
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue() < other.intValue();
			case Var::Float: return intValue() < other.floatValue();
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Integer: return floatValue() < other.intValue();
			case Var::Float: return floatValue() < other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue()->Less(*(*other.pointerValue()));
			}
			else if (!lp && rp)
			{
				return Less(*(*other.pointerValue()));
			}
			else if (lp && !rp)
			{
				return pointerValue()->Less(other);
			}
		}

		return textValue() < other.textValue();
	}


	Var & Var::Add(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() + other.intValue());
			case Var::Float: return intValue(intValue() + (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() + other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() + other.floatValue());
			case Var::Integer: return floatValue(floatValue() + (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() + other.floatValue());
			}

			// String
		case Var::Str:
			switch (other.getTypeID())
			{
			case Var::Str:
			default:
				return stringValue(stringValue() + other.stringValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool rp = other.compareType(Var::Pointer);
		if (lp || rp)
		{
			bool lg = isPointerType();
			bool rg = other.isPointerType();
			if (lg && rg)
			{
				return pointerValue()->Add(*(*other.pointerValue()));
			}
			else if (!lg && rg)
			{
				return Add(*(*other.pointerValue()));
			}
			else if (lg && !rg)
			{
				return Set(*pointerValue().get()).Add(other);
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "+", other.getTypeID(), other));
	}
		  
	Var & Var::Div(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() / other.intValue());
			case Var::Float: return intValue(intValue() / (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() / other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() / other.floatValue());
			case Var::Integer: return floatValue(floatValue() / (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() / other.floatValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue()->Div(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Div(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Div(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "/", other.getTypeID(), other));
	}
		  
	Var & Var::Mod(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() % other.intValue());
			case Var::Float: return intValue(intValue() % (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() % other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return intValue((int32_t)floatValue() % (int32_t)other.floatValue());
			case Var::Integer: return intValue((int32_t)floatValue() % other.intValue());
			default:
				if (other.isFloatType())
					return intValue((int32_t)floatValue() % (int32_t)other.floatValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue()->Mod(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Mod(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Mod(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "%", other.getTypeID(), other));
	}
		  
	Var & Var::Mul(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() * other.intValue());
			case Var::Float: return intValue(intValue() * (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() * other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() * other.floatValue());
			case Var::Integer: return floatValue(floatValue() * (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() * other.floatValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue()->Mul(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Mul(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Mul(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "*", other.getTypeID(), other));
	}
		  
	Var & Var::Pow(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue((int32_t)pow(intValue(), other.intValue()));
			case Var::Float: return intValue((int32_t)pow(intValue(), (int32_t)other.floatValue()));
			default:
				if (other.isIntType())
					return intValue((int32_t)pow(intValue(), other.intValue()));
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(powf(floatValue(), other.floatValue()));
			case Var::Integer: return floatValue(powf(floatValue(), (float)other.intValue()));
			default:
				if (other.isFloatType())
					return floatValue(powf(floatValue(), other.floatValue()));
			}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue()->Pow(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Pow(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Pow(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "^", other.getTypeID(), other));
	}
		  
	Var & Var::Sub(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() - other.intValue());
			case Var::Float: return intValue(intValue() - (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() - other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() - other.floatValue());
			case Var::Integer: return floatValue(floatValue() - (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() - other.floatValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue()->Sub(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Sub(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Sub(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "-", other.getTypeID(), other));
	}
		  
	Var & Var::Set(const Var & other)
	{
		if (Var * v = other.pointerValue().get())
		{
			return Set(*v);
		}
		return setType(other.getTypeID()).dataValue(other.tokensValue());
	}


	/* * * * * * * * * * * * * * * * * * * * */

	bool operator&&(const Var & lhs, const Var & rhs)	{ return lhs.And(rhs); }
	bool operator||(const Var & lhs, const Var & rhs)	{ return lhs.Or(rhs); }

	Var & operator+=(Var & lhs, const Var & rhs)		{ return lhs.Add(rhs); }
	Var & operator-=(Var & lhs, const Var & rhs)		{ return lhs.Sub(rhs); }
	Var & operator*=(Var & lhs, const Var & rhs)		{ return lhs.Mul(rhs); }
	Var & operator/=(Var & lhs, const Var & rhs)		{ return lhs.Div(rhs); }
	Var & operator^=(Var & lhs, const Var & rhs)		{ return lhs.Pow(rhs); }
	Var & operator%=(Var & lhs, const Var & rhs)		{ return lhs.Mod(rhs); }

	Var operator+(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Add(rhs); }
	Var operator-(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Sub(rhs); }
	Var operator*(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Mul(rhs); }
	Var operator/(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Div(rhs); }
	Var operator^(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Pow(rhs); }
	Var operator%(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Mod(rhs); }

	Var & Var::operator=(const Var & other)		{ return Set(other); }
	Var & Var::operator=(bool value)			{ return boolValue(value); }
	Var & Var::operator=(float value)			{ return floatValue(value); }
	Var & Var::operator=(double value)			{ return floatValue((float)value); }
	Var & Var::operator=(int32_t value)			{ return intValue(value); }
	Var & Var::operator=(const Ptr & value)		{ return pointerValue(value); }
	Var & Var::operator=(const String & value)	{ return stringValue(value); }
	Var & Var::operator=(CString value)			{ return stringValue(value); }
	Var & Var::operator=(char value)			{ return stringValue(String(1, value)); }
	
	/* * * * * * * * * * * * * * * * * * * * */
}