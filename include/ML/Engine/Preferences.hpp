#ifndef _ML_PREFERENCES_HPP_
#define _ML_PREFERENCES_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Disposable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// INI Reader
	struct ML_ENGINE_API Preferences final : public Trackable, public NonCopyable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Preferences();
		explicit Preferences(String const & filename);
		~Preferences() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(String const & filename);
		bool saveToFile(String const & filename) const;
		bool saveToFile() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::set<String> sections() const;
		Map<String, String> values() const;

		bool		set_string	(String const & section, String const & name, String const & value);
		bool		get_bool	(String const & section, String const & name, bool dv) const;
		float64_t	get_double	(String const & section, String const & name, float64_t dv) const;
		float_t		get_float	(String const & section, String const & name, float_t dv) const;
		int32_t		get_int		(String const & section, String const & name, int32_t dv) const;
		uint32_t	get_uint	(String const & section, String const & name, uint32_t dv) const;
		String		get_string	(String const & section, String const & name, String const & dv) const;
	
		inline operator bool() const { return m_ini; }

		inline auto filename() const -> String const & { return m_filename; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: 
		String m_filename;
		void * m_ini;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PREFERENCES_HPP_