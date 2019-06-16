#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API SharedLibrary final
		: public IObject
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
	public:
		using map_type = typename Map<String, void *>;
		using const_iterator = typename map_type::const_iterator;

	public:
		SharedLibrary();
		explicit SharedLibrary(const String & filename);
		SharedLibrary(SharedLibrary && copy);
		~SharedLibrary();

	public:
		bool	dispose() override;
		bool	loadFromFile(const String & filename) override;
		void *	loadFunction(const String & name);

	public:
		template <class Out, class ... Args> 
		inline Out callFunction(const String & name, Args && ... args)
		{
			using Fun = Out(*)(Args...);
			Fun fun;
			return ((fun = reinterpret_cast<Fun>(loadFunction(name)))
				? (static_cast<Out>(fun((args)...)))
				: (static_cast<Out>(nullptr)));
		}

	public:
		inline const void *		instance()	const { return m_instance;	}
		inline const String &	filename()	const { return m_filename;	}
		inline const map_type &	functions() const { return m_functions; }

	public:
		inline operator bool() const { return (bool)(m_instance); }

	private:
		void *		m_instance;
		String		m_filename;
		map_type	m_functions;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_