#ifndef _ML_COMMAND_HPP_
#define _ML_COMMAND_HPP_

#include <ML/Script/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Command final
		: public ITrackable
	{
	public:
		using fn_args = Args & ;
		using fn_type = Var(*)(fn_args);

	public:
		Command();
		Command(const String & name, fn_type func);
		Command(const Command & copy);
		~Command() {};

		inline const String & name() const { return m_name; }
		
		inline Var operator()(fn_args args)	{ return m_func(args); }

	private:
		String	m_name;
		fn_type	m_func;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COMMAND_HPP_