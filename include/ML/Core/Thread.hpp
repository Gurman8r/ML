#ifndef _ML_THREAD_HPP_
#define _ML_THREAD_HPP_

#include <ML/Core/Duration.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>

#define ML_THREAD_CONDITIONS(FUN) \
std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<FUN>>, std::thread>>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Thread final
		: public I_Newable
		, public I_Disposable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Thread();
		~Thread();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool alive() const;
		bool joinable() const;
		bool dispose() override;
		void sleep(const Duration & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, 
			class ... Args, 
			class = ML_THREAD_CONDITIONS(Fun)
		> inline std::thread * launch(Fun && fun, Args && ... args)
		{
			return ((alive())
				? (nullptr)
				: (m_thr = new std::thread(fun, std::forward<Args>(args)...))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: std::thread * m_thr;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_THREAD_HPP_