#ifndef _ML_PREFERENCES_HPP_
#define _ML_PREFERENCES_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Newable.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// INI Reader
	struct ML_ENGINE_API Preferences final : public Newable, public NonCopyable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Preferences();
		explicit Preferences(const String & filename);
		~Preferences() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool saveToFile(const String & filename) const;
		bool saveToFile() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::set<String> sections() const;
		Map<String, String> values() const;

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
		ptr_t<void> m_ini;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PREFERENCES_HPP_