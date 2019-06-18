#ifndef _ML_PREFERENCES_HPP_
#define _ML_PREFERENCES_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/IObject.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/IReadable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Prefs final
		: public IObject
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
	public:
		Prefs();
		explicit Prefs(const String & filename);
		Prefs(Prefs && copy);
		~Prefs();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		bool		GetBool		(const String & section, const String & name, bool default_value) const;
		double		GetDouble	(const String & section, const String & name, double default_value) const;
		float		GetFloat	(const String & section, const String & name, float default_value) const;
		int32_t		GetInt		(const String & section, const String & name, int32_t default_value) const;
		uint32_t	GetUint		(const String & section, const String & name, uint32_t default_value) const;
		String		GetString	(const String & section, const String & name, const String & default_value) const;
	
	public:
		inline operator bool() const { return m_ini; }

	private:
		void * m_ini;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PREFERENCES_HPP_