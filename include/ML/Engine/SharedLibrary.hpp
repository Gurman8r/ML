#ifndef _ML_SHARED_LIBRARY_HPP_
#define _ML_SHARED_LIBRARY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API SharedLibrary final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
	public:
		using FunctionMap = typename Map<String, void *>;

	public:
		SharedLibrary();
		explicit SharedLibrary(const String & filename);
		SharedLibrary(SharedLibrary && copy);
		~SharedLibrary();

	public:
		SharedLibrary & operator=(SharedLibrary && copy);

	public:
		bool	dispose() override;
		bool	loadFromFile(const String & filename) override;
		void *	loadFunction(const String & name);

	public:
		template <
			class Result,
			class ... Args
		> inline Result callFun(const String & name, Args && ... args)
		{
			using Fun = Result(*)(Args...);
			Fun fun;
			return ((fun = reinterpret_cast<Fun>(loadFunction(name)))
				? (static_cast<Result>(fun((args)...)))
				: (static_cast<Result>(NULL)));
		}

	public:
		inline const void *			instance()	const { return m_instance;	}
		inline const String &		filename()	const { return m_filename;	}
		inline const FunctionMap &	functions() const { return m_functions; }

	public:
		inline operator bool() const { return (bool)(m_instance); }

	private:
		void *		m_instance;
		String		m_filename;
		FunctionMap m_functions;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_