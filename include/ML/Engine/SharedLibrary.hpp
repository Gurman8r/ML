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
		SharedLibrary();
		~SharedLibrary();

	public:
		bool	dispose() override;
		bool	loadFromFile(const String & filename) override;
		void *	loadFunction(const String & name);

	public:
		template <
			class Out,
			class ... Args
		> inline Out callFunction(const String & name, Args && ... args)
		{
			using Fun = Out(*)(Args...);
			static Fun fun;
			return ((fun = reinterpret_cast<Fun>(loadFunction(name)))
				? (static_cast<Out>(fun((args)...)))
				: (static_cast<Out>(NULL)));
		}

	public:
		inline const String &	filename() const { return m_filename; }
		inline const void *		instance() const { return m_instance; }

	private:
		String m_filename;
		void * m_instance;

		mutable Map<String, void *> m_fun;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHARED_LIBRARY_HPP_