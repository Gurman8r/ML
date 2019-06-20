#ifndef _ML_OPERATOR_HPP_
#define _ML_OPERATOR_HPP_

#include <ML/Script/Export.hpp>
#include <ML/Core/INewable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Operator final
		: public INewable
		, public IComparable<Operator>
		, public IComparable<int32_t>
	{
	public:
		enum : int32_t
		{
			OP_INVALID,	//	" "	
			OP_SET,		//	"="	
			OP_POW_SET,	//	"^="	
			OP_MUL_SET,	//	"*="	
			OP_DIV_SET,	//	"/="	
			OP_MOD_SET,	//	"%="	
			OP_ADD_SET,	//	"+="	
			OP_SUB_SET,	//	"-="	
			OP_EQU,		//	"=="	
			OP_NEQ,		//	"!="	
			OP_LT,		//	"<"	
			OP_GT,		//	">"	
			OP_LTE,		//	"<="	
			OP_GTE,		//	">="
			OP_AND,		//	"&&"	
			OP_OR,		//	"||"	
			OP_POW,		//	"^"	
			OP_MUL,		//	"*"	
			OP_DIV,		//	"/"	
			OP_MOD,		//	"%"	
			OP_ADD,		//	"+"	
			OP_SUB,		//	"-"	
		};
	public:
		using const_iterator = typename HashMap<String, int32_t>::const_iterator;

		static const HashMap<String, int32_t> Names;

	public:
		Operator(int32_t type);
		Operator(const String & text);
		Operator(const String & lhs, const String & rhs);
		Operator(const Operator & copy);
		~Operator() {}

	public:
		int32_t type;

		inline operator bool() const { return (type != OP_INVALID); }

	public:
		bool equals(const Operator & value) const override;
		bool equals(const int32_t & value) const override;

		bool lessThan(const Operator & value) const override;
		bool lessThan(const int32_t & value) const override;

		void serialize(OStream & out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OPERATOR_HPP_