#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <ML/Engine/SharedLibrary.hpp>
#include <ML/Engine/PluginAPI.hpp>

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
		Plugin();
		~Plugin();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		inline const SharedLibrary & lib() const { return m_lib; }
		inline SharedLibrary &		 lib()		 { return m_lib; }

	private:
		SharedLibrary m_lib;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_