#ifndef _ML_FUNCTION_HPP_
#define _ML_FUNCTION_HPP_

#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Function
		: public ITrackable
	{
	public:
		virtual ~Function() {}
		
		virtual void run() = 0;
		
		inline void operator()() { return run(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename F>
	class VoidFun final : public Function
	{
		F	m_fun;

	public:
		VoidFun(F fun)
			: m_fun(fun)
		{
		}

		inline void run() override { return m_fun(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename F, typename A>
	class ArgFun final : public Function
	{
		F	m_fun;
		A	m_arg;

	public:
		ArgFun(F fun, A arg)
			: m_fun(fun)
			, m_arg(arg)
		{
		}

		inline void run() override { return m_fun(m_arg); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename F = void(T::*)()>
	class MemberFun final : public Function
	{
		F	m_fun;
		T *	m_obj;

	public:
		MemberFun(F fun, T * obj)
			: m_fun(fun)
			, m_obj(obj)
		{
		}

		inline void run() override { return (m_obj->*m_fun)(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename A, typename F = void(T::*)(A)>
	class MemberArgFun final : public Function
	{
		F	m_fun;
		T *	m_obj;
		A	m_arg;

	public:
		MemberArgFun(F fun, T * obj, A arg)
			: m_fun(fun)
			, m_obj(obj)
			, m_arg(arg)
		{
		}

		inline void run() override { return (m_obj->*m_fun)(m_arg); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FUNCTION_HPP_