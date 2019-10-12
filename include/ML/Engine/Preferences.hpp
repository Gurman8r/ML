#ifndef _ML_PREFERENCES_HPP_
#define _ML_PREFERENCES_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Writable.hpp>
#include <ML/Core/I_Readable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// INI Reader
	struct ML_ENGINE_API Preferences final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Writable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Preferences();
		explicit Preferences(const Preferences & copy);
		explicit Preferences(const String & filename);
		~Preferences() { dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;
		bool saveToFile() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::set<String> sections() const;
		Tree<String, String> values() const;

		bool		set_string	(const String & section, const String & name, const String & value);
		bool		get_bool	(const String & section, const String & name, bool dv) const;
		float64_t	get_double	(const String & section, const String & name, float64_t dv) const;
		float_t		get_float	(const String & section, const String & name, float_t dv) const;
		int32_t		get_int		(const String & section, const String & name, int32_t dv) const;
		uint32_t	get_uint	(const String & section, const String & name, uint32_t dv) const;
		String		get_string	(const String & section, const String & name, const String & dv) const;
	
		inline operator bool() const { return m_ini; }

		inline auto filename() const -> const String & { return m_filename; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		String m_filename;
		void * m_ini;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PREFERENCES_HPP_