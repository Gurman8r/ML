#ifndef _ML_THREAD_HPP_
#define _ML_THREAD_HPP_

#include <ML/Core/Duration.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/ITrackable.hpp>

#define ML_THREAD_CONDITIONS(FUN) \
std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<FUN>>, std::thread>>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Thread final
		: public ITrackable
		, public IDisposable
		, public INonCopyable
	{
	public:
		Thread();
		~Thread();

	public:
		bool alive() const;
		bool joinable() const;
		bool dispose() override;
		void sleep(const Duration & value);

	public:
		template <
			class Fun, 
			class ... Args, 
			class = ML_THREAD_CONDITIONS(Fun)
		> inline std::thread * launch(Fun && fun, Args && ... args)
		{
			return ((alive())
				? (NULL)
				: (m_thr = new std::thread(fun, (args)...))
			);
		}

	private:
		std::thread * m_thr;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_THREAD_HPP_