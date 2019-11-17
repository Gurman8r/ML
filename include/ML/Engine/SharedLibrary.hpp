#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API SharedLibrary final
		: public Newable
		, public Disposable
		, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using map_type			= typename Map<String, voidptr_t>;
		using const_iterator	= typename map_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		SharedLibrary();
		explicit SharedLibrary(const String & filename);
		SharedLibrary(SharedLibrary && copy);
		~SharedLibrary();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		
		bool loadFromFile(const String & filename);
		
		voidptr_t loadFunction(const String & name);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Out, class ... Args
		> inline Out callFunction(const String & name, Args && ... args)
		{
			using Fun = Out(*)(Args...);
			Fun fun{ reinterpret_cast<Fun>(loadFunction(name)) };
			return (fun
				? static_cast<Out>(fun(std::forward<Args>(args)...))
				: static_cast<Out>(nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto instance()	const -> voidptr_t { return m_instance; }
		inline auto filename()	const -> const String & { return m_filename; }
		inline auto functions() const -> const map_type & { return m_functions; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const { return (bool)(m_instance); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		voidptr_t	m_instance;
		String		m_filename;
		map_type	m_functions;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_