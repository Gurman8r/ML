#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API SharedLibrary final : public Trackable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Functions = typename std::map<String, void *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary();
		explicit SharedLibrary(String const & filename);
		SharedLibrary(SharedLibrary && copy) noexcept;
		~SharedLibrary() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		
		bool loadFromFile(String const & filename);
		
		void * loadFunction(String const & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Out, class ... Args
		> inline Out callFunction(String const & name, Args && ... args)
		{
			using Fun = Out(*)(Args...);
			Fun fun{ reinterpret_cast<Fun>(loadFunction(name)) };
			return (fun
				? static_cast<Out>(fun(std::forward<Args>(args)...))
				: static_cast<Out>(nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto instance()	const -> void * { return m_instance; }
		inline auto filename()	const -> String const & { return m_filename; }
		inline auto functions() const -> Functions const & { return m_functions; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return (bool)(m_instance); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void *		m_instance;
		String		m_filename;
		Functions	m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_