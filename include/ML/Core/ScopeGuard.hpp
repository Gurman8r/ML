#ifndef _ML_SCOPE_GUARD_HPP_
#define _ML_SCOPE_GUARD_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>
#include <ML/Core/NonCopyable.hpp>

#define ML_SCOPE_EXIT \
	auto ML_ANON(ML_SCOPE_EXIT_STATE) \
	= ::ml::detail::ScopeGuardOnExit() + [&]() noexcept

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct ScopeGuard;

	template <> struct ScopeGuard<> final { ScopeGuard() = delete; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class Fun
	> struct ScopeGuard<Fun> final : public NonCopyable
	{
		Fun m_fun;

		explicit ScopeGuard(Fun && fun) : m_fun{ std::forward(fun) }
		{
		}

		~ScopeGuard()
		{
			m_fun();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class ScopeGuardOnExit {};

		template <
			class Fun
		> inline ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun && fun)
		{
			return ScopeGuard<Fun>{ std::forward(fun) };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCOPE_GUARD_HPP_