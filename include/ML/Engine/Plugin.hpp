#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <ML/Engine/PluginAPI.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IReadable.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Plugin final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
	public:
		using FunctionMap = typename Map<String, void *>;

	public:
		Plugin();
		explicit Plugin(const String & filename);
		Plugin(Plugin && copy);
		~Plugin();

	public:
		Plugin & operator=(Plugin && copy);

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		void * loadFunction(const String & name);

		template <class Out, class ... Args> 
		inline Out callFun(const String & name, Args && ... args)
		{
			using Fun = Out(*)(Args...);
			Fun fun;
			return ((fun = reinterpret_cast<Fun>(loadFunction(name)))
				? (static_cast<Out>(fun((args)...)))
				: (static_cast<Out>(NULL)));
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

#endif // !_ML_PLUGIN_HPP_